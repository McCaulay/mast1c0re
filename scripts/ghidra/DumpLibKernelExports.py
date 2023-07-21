# Dump exported addresses from libkernel_sys.sprx to libkernel.hpp
#@author McCaulay
#@category PlayStation

import re
import os

# Append an incrementing number on the name until it is unique
names = []
def getUniqueName(name):
    if name not in names:
        return name

    i = 2
    while name + str(i) in names:
        i += 1
    return name + str(i)

# Convert the function name into a define key
def getDefineKey(name):
    name = re.sub('[^A-Za-z0-9_]+', '', name) # Remove non-ASCII characters
    name = re.sub(r"([A-Z][a-z]+)", r"_\1", name) # sceKernelSendNotificationRequest -> sce_Kernel_Send_Notification_Request
    name = name.upper() # SCE_KERNEL_SEND_NOTIFICATION_REQUEST
    name = 'LIB_KERNEL_' + name # LIB_KERNEL_SCE_KERNEL_SEND_NOTIFICATION_REQUEST
    name = getUniqueName(name)
    names.append(name)
    return name

# Get the relative address in the current program
def getRelativeAddress(address):
    return address.getOffset() - currentProgram.getImageBase().getOffset()

# Write file as "libkernel.hpp"
filepath = os.path.join(os.getcwd(), 'libkernel.hpp')
print('Writing to ' + filepath)
with open(filepath, 'w') as f:
    f.write('#pragma once\n\n')
    f.write('#if (defined(PS?) && PS?) && defined(FIRMWARE) && FIRMWARE == ?\n')

    # Loop symbols
    for sym in currentProgram.getSymbolTable().getSymbolIterator():
        # Dump exported function addresses
        if sym.getSource() == ghidra.program.model.symbol.SourceType.IMPORTED and sym.getSymbolType() == ghidra.program.model.symbol.SymbolType.FUNCTION and not sym.isExternal():
            f.write('    #define ' + getDefineKey(sym.getName().encode('ascii', 'ignore')) + ' 0x' + ("%x" % getRelativeAddress(sym.getAddress())) + '\n')
    f.write('#endif')
print('Finished, don\'t forget to update the console definition (eg: PS4) and firmware definition (eg: FIRMWARE == 1070) in the output file')