LIBMAST1C0RE=$(MAST1C0RE)/sdk

# Addresses
TEXT	?= 0x400000
DATA	?= 0x3d8000
ABI		?= 0x3d0000

# Variables
SYSTEM          ?= PCSX2
FIRMWARE        ?= 0.00
EBOOT_VERSION   ?= 1.01

FIRMWARE_UNDER  =  $(subst .,_,$(FIRMWARE))
FIRMWARE_DASH   =  $(subst .,-,$(FIRMWARE))
FIRMWARE_NUM    =  $(subst .,,$(FIRMWARE))
EBOOT_NUM       =  $(subst .,,$(EBOOT_VERSION))

# Binaries
PREFIX			= mips64r5900el-ps2-elf-
CPP				= $(PREFIX)g++
SAVE_EXPLOIT	?= okrager

# Directories
BDIR = bin
ODIR = build
SDIR = src

# Files
CPPFILES	= $(wildcard $(SDIR)/*.cpp $(SDIR)/*/*.cpp)
OBJS		= $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CPPFILES))

# Save files
ifeq ($(SYSTEM), PCSX2)
	SAVE_IN		?= $(BDIR)/clean/Mcd001.ps2
	SAVE_OUT	?= $(BDIR)/$(SYSTEM)/$(FIRMWARE_UNDER)/$(SAVE_EXPLOIT)/Mcd001.ps2
else ifeq ($(SYSTEM), $(filter $(SYSTEM),PS4 PS5))
	SAVE_IN		?= $(BDIR)/clean/VMC0.card
	SAVE_OUT	?= $(BDIR)/$(SYSTEM)/$(FIRMWARE_UNDER)/$(SAVE_EXPLOIT)/VMC0.card
endif

# Flags
LINKFLAGS	= -Wl,-z,max-page-size=0x1,--section-start=.MIPS.abiflags=$(ABI)
CPPFLAGS	= -Tdata=$(DATA) -Ttext=$(TEXT) -mno-gpopt -nostartfiles -nostdlib -nodefaultlibs -ffreestanding $(LINKFLAGS) -I$(LIBMAST1C0RE)/include -I. -D$(SYSTEM)=1 -DFIRMWARE=$(FIRMWARE_NUM) -DEBOOT_VERSION=$(EBOOT_NUM)

# Target
TARGET = $(shell basename $(CURDIR))-$(SYSTEM)-$(FIRMWARE_DASH)-$(SAVE_EXPLOIT).elf

all: compile save

save: $(SAVE_IN) $(SAVE_OUT)
	$(SAVE_EXPLOIT) $(SAVE_IN) $(SAVE_OUT) $(BDIR)/$(TARGET)
	rm $(BDIR)/$(TARGET)

compile: sdk $(ODIR) $(BDIR) $(OBJS) crt0
	$(CPP) $(CPPFLAGS) $(ODIR)/crt0.o $(OBJS) -L$(LIBMAST1C0RE) -l:mast1c0re.a -o $(BDIR)/$(TARGET)

sdk:
	make -B -C $(LIBMAST1C0RE) SYSTEM=$(SYSTEM) FIRMWARE=$(FIRMWARE_NUM) EBOOT=$(EBOOT_NUM) clean
	make -B -C $(LIBMAST1C0RE) SYSTEM=$(SYSTEM) FIRMWARE=$(FIRMWARE_NUM) EBOOT=$(EBOOT_NUM)

crt0:
	$(CPP) $(CPPFLAGS) -c $(LIBMAST1C0RE)/crt0.S -o $(ODIR)/crt0.o

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

$(BDIR) $(ODIR) $(SAVE_IN) $(SAVE_OUT):
	@mkdir -p $(shell dirname $@)

.PHONY: clean
clean:
	rm -rf $(BDIR)/$(TARGET) $(ODIR) $(BDIR)/$(SYSTEM)/$(FIRMWARE)