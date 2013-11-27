APP = kernel
OBJDIR = build
AUXFILES = Makefile link.ld floppy.img cd.iso

# Pick output [floppy|iso|both]
OUTPUT = both

# Location to mount floppy.img
FLOPPYDIR=/mnt/floppy

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD := 1
VERSION_DATE := $(shell date +'%Y-%m-%d')

AS = nasm
#CC = gcc
CC = gcc-4.8.2 # Cross Compiler
LD = ld

ASFLAGS = -f elf
CFLAGS = -std=c99 -m32 -nodefaultlibs -nostdlib -nostdinc -fno-builtin -ffreestanding -I src/include \
			-Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wstrict-prototypes \
			-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable \
			-DVERSION_MAJOR=$(VERSION_MAJOR) -DVERSION_MINOR=$(VERSION_MINOR) -DVERSION_BUILD=$(VERSION_BUILD) -DVERSION_DATE=\"$(VERSION_DATE)\"
# -Wpedantic -Wconversion
LDFLAGS = -m elf_i386 -T link.ld

AFILES := $(shell find src/ -name *.s | sed 's/src\///')
CFILES := $(shell find src/ -name *.c | sed 's/src\///')
FILES = $(AFILES) $(CFILES)
ADIRS := $(shell find src/ -name '*.s' -exec dirname {} \; | sed 's/src//' | uniq)
CDIRS := $(shell find src/ -name '*.c' -exec dirname {} \; | sed 's/src//' | uniq)
DIRS = $(ADIRS) $(CDIRS)

AOBJS := $(patsubst %.s, $(OBJDIR)/%.o, $(AFILES))
COBJS := $(patsubst %.c, $(OBJDIR)/%.o, $(CFILES))
OBJS = $(AOBJS) $(COBJS)

.PHONY: all clean clean-floppy build-floppy build-iso build-both todo dist backup cloc $(OBJDIR)

all: $(OBJDIR) $(OBJDIR)/$(APP) build-$(OUTPUT)
	@echo Done

$(OBJDIR):
	@$(shell for dir in $(DIRS); do mkdir -p $(OBJDIR)/$$dir; done)

$(OBJDIR)/$(APP): $(OBJS)
	@echo Linking $(APP)
	@$(LD) $(LDFLAGS) -o $(OBJDIR)/$(APP) $(OBJS)

$(OBJDIR)/%.o: src/%.s
	@echo Assembling $<
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.o: src/%.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning
	-@-rm -rf $(OBJDIR)/* simpleos.tgz

clean-floppy:
	@echo Cleaning floppy.img
	@losetup /dev/loop0 floppy.img
	@mkdir -p $(FLOPPYDIR)
	@mount /dev/loop0 $(FLOPPYDIR)
	@rm -f $(FLOPPYDIR)/$(APP)
	@sync
	@umount /dev/loop0
	@losetup -d /dev/loop0

build-floppy:
	@echo Updating floppy.img
	@losetup /dev/loop0 floppy.img
	@mkdir -p $(FLOPPYDIR)
	@mount /dev/loop0 $(FLOPPYDIR)
	@cp $(OBJDIR)/$(APP) $(FLOPPYDIR)/$(APP)
	@sync
	@umount /dev/loop0
	@losetup -d /dev/loop0

build-iso:
	@echo Creating cd.iso
	@rm -f cd.iso
	@mkdir -p iso/boot/grub
	@cp /usr/share/grub/x86_64-redhat/stage2_eltorito iso/boot/grub/
	@printf "timeout 0\n\ntitle   SimpleOS\nroot    (cd)\nkernel  /kernel\n" > iso/boot/grub/grub.conf
	@cp $(OBJDIR)/$(APP) iso/$(APP)
	@mkisofs -quiet -R -l -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -V SimpleOS -o cd.iso iso
	@rm -rf iso/

build-both: build-floppy build-iso

todo:
	@for file in $(FILES); do fgrep -H -e TODO -e FIXME src/$$file; done; true

dist: clean all build-both
	@echo Building simpleos.tgz
	@tar czf simpleos.tgz --exclude='*.suo' --exclude='*.vcxproj.user' build/ src/ $(AUXFILES)

backup: dist
	@echo Saving to backup/simpleos-$(shell date +%Y-%m-%d).tgz
	@mkdir -p backup/
	@mv simpleos.tgz backup/simpleos-$(shell date +%Y-%m-%d).tgz

cloc:
	@cloc --quiet src/ $(AUXFILES) | tail -n +3
