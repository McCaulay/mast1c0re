#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <cstdint>
#include <offsets/ps/libkernel.hpp>

#define BREAKOUT_BUSY_TIMEOUT 10000

// Restore Registers
#define BREAKOUT_RESTORE_RBX 0x0000001000000090
#define BREAKOUT_RESTORE_R14 0x0000008000000000
#define BREAKOUT_RESTORE_R15 0x0000001030000090
#define BREAKOUT_RESTORE_RBP_OFF 0xc7b10
#define BREAKOUT_RESTORE_RSP_OFF 0xc7ab8

// PS2 ELF Loader Shared Data
#define BREAKOUT_SHARED 0x0f00000

// Leaks
#define BREAKOUT_PARTIAL_POINTER_OVERWRITE_RET 0x91
#define BREAKOUT_STACK_DIFF 0xc7aa8 // 0x7EECAFAA8 - 0x7EEBE8000

// Stage 1 ROP Chain (EBOOT.BIN)
#define STAGE_1 0x60F0000

// Stage 2 ROP Chain (PS2 Memory)
// Must have large space before and after
// Before: PS4/5 Stack during ROP execution
// After:  ROP Chain
#define ROP_CHAIN 0x0e00000

// Helpers
#define NATIVE(address) PS::Breakout::toNative(address)
#define NATIVE_TO_PVAR(address) PS::Breakout::fromNative(address)
#define VAR_TO_NATIVE(var) NATIVE((uint32_t)(&var))
#define PVAR_TO_NATIVE(var) NATIVE((uint32_t)(var))

#define DEREF(address) PS::Breakout::readMemoryU64(address)

#define EBOOT(address) PS::Breakout::eboot(address)
#define STACK(address) PS::Breakout::stack(address)
#ifdef LIB_KERNEL_SCE_KERNEL_USLEEP
    #define LIBKERNEL(address) PS::Breakout::libKernel(address)
#endif
#define GADGET(address) EBOOT(address)

namespace PS
{
    class Breakout
    {
        private:
            struct Shared
            {
                uint32_t ebootDiff;
                uint64_t stackAddress;
                uint64_t libKernelAddress;

                uint64_t gadgetRet;
                uint64_t gadgetPopRaxRet;
                uint64_t gadgetPopRbxRet;
            };
        public:
            static void init();
            static void restore();

            static uint64_t readMemoryU64(uint64_t address);

            static uint64_t call(uint64_t address);
            static uint64_t call(uint64_t address, uint64_t rdi);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t stack1);
            static uint64_t call(uint64_t address, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t stack1, uint64_t stack2);

            #ifdef LIB_KERNEL_SYS_RET
            static uint64_t syscall(int32_t index);
            static uint64_t syscall(int32_t index, uint64_t rdi);
            static uint64_t syscall(int32_t index, uint64_t rdi, uint64_t rsi);
            static uint64_t syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx);
            static uint64_t syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
            static uint64_t syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
            static uint64_t syscall(int32_t index, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
            #endif
        private:
            static void resetSStatusIndex();
            static void sStatusBufferOverflow(uint8_t* overflow, uint32_t size);
            static void resetNStatusIndex();
            static void setOOBindex(uint32_t index);
            static uint32_t writeOOB(uint32_t index, uint8_t value);
            static uint32_t writeOOB(uint32_t index, uint16_t value);
            static uint32_t writeOOB(uint32_t index, uint32_t value);
            static uint32_t writeOOB(uint32_t index, uint64_t value);
            static void restoreReadHandler();
            static void restoreWriteHandler();
            static uint32_t callGadgetAndGetResult(uint32_t gadget, uint32_t gadgetSize);
            static uint32_t leakEboot();
            static uint64_t leakStack();
            static void setupGadgetWithArgument(uint32_t gadget);
            static void setupROP();

            static void resetChain();
            static uint32_t pushChain(uint64_t value);
            static void executeChain();
            static uint64_t executeAndGetResult(uint64_t address);

            static void getRAX(uint64_t* value);
            static void setRAX(uint64_t rax);
            static void setRBX(uint64_t rbx);
            static void setRCX(uint64_t rcx);
            static void setRDX(uint64_t rdx);
            static void setRDI(uint64_t rdi);
            static void setRSI(uint64_t rsi);
            static void setR8(uint64_t r8);
            static void setR13(uint64_t r13);
            static void setR9(uint64_t r9);
        private:
            static PS::Breakout::Shared* shared;
            static uint32_t nStatusIndex;

            static uint32_t chainIndex;
            static uint64_t* chain;

            static char tempVar[256];
        public: // inline
            static inline uint64_t toNative(uint32_t address)
            {
                return (address == 0 ? 0 : ((uint64_t)address | (uint64_t)0x8000000000));
            }

            static inline uint32_t fromNative(uint64_t address)
            {
                return (uint32_t)(address & 0xFFFFFFFF);
            }

            static inline uint32_t eboot(uint32_t address)
            {
                return address + PS::Breakout::shared->ebootDiff;
            }

            static inline uint64_t stack(uint64_t address)
            {
                return address + PS::Breakout::shared->stackAddress;
            }

            #ifdef LIB_KERNEL_SCE_KERNEL_USLEEP
            static inline uint64_t libKernel(uint64_t address)
            {
                return address + PS::Breakout::shared->libKernelAddress;
            }
            #endif
    };
}
#endif