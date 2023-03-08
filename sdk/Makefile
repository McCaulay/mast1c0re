# Binaries
PREFIX	= mips64r5900el-ps2-elf-
CPP		= $(PREFIX)g++
AR		= $(PREFIX)ar

# Variables
SYSTEM          ?= PCSX2
FIRMWARE        ?= 0.00
EBOOT_VERSION   ?= 1.01

FIRMWARE_NUM    =  $(subst .,,$(FIRMWARE))
EBOOT_NUM       =  $(subst .,,$(EBOOT_VERSION))

# Directories
ODIR = build
SDIR = src
IDIR = include

# Flags
LINKFLAGS	= -Wl,-z,max-page-size=0x1
CPPFLAGS	= -mno-gpopt -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -I$(IDIR) $(LINKFLAGS) -D$(SYSTEM)=1 -DFIRMWARE=$(FIRMWARE_NUM) -DEBOOT_VERSION=$(EBOOT_NUM)

# Files
CPPFILES	= $(wildcard $(SDIR)/*.cpp $(SDIR)/*/*.cpp $(SDIR)/*/*/*.cpp $(SDIR)/*/*/*/*.cpp $(SDIR)/*/*/*/*/*.cpp $(SDIR)/*/*/*/*/*/*.cpp)
OBJS		= $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CPPFILES))

# Target
TARGET = mast1c0re.a

# Rules
all: $(TARGET)

$(TARGET): $(ODIR) $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

$(ODIR):
	@mkdir -p $(shell dirname $@)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(ODIR)