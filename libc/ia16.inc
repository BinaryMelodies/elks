# Makefile include for ia16-elf-gcc build

INCLUDES=-I$(TOPDIR)/include -I$(TOPDIR)/libc/include -I$(TOPDIR)/elks/include
DEFINES=-D__LIBC__
LIBOBJS=$(OBJS)

INCS=$(INCLUDES)
SDEFS=$(DEFINES)
CDEFS=$(DEFINES)

ARCH=-ffreestanding -fno-inline -melks -mtune=i8086
ifeq "" "$(filter -mcmodel=%,$(MULTILIB))"
ARCH+=-mcmodel=small -mno-segment-relocation-stuff
endif

CC=ia16-elf-gcc
AS=ia16-elf-as
AR=ia16-elf-ar
LD=ia16-elf-ld

CFLAGS=$(ARCH) $(INCLUDES) $(CDEFS) -Wall -Os $(MULTILIB)
CFLAGS+=-Wextra -Wtype-limits -Wno-unused-parameter -Wno-sign-compare
CFLAGS+=$(MULTILIB)
ASFLAGS=--32-segelf -mtune=i8086
LDFLAGS=-mtune=i8086
# Used in subdirs to quickly create a library archive without a symbol index
ARFLAGS_SUB=cqS

ifdef MULTISUBDIR
LIBC=$(TOPDIR)/libc/build-ml/$(MULTISUBDIR)/libc.a
CRT0=$(TOPDIR)/libc/build-ml/$(MULTISUBDIR)/crt0.o
endif
LIB_CPU=i86
LIB_OS=ELKS

.S.o:
	$(CC) -E $(MULTILIB) $(INCS) $(SDEFS) -MD -o $*.tmp $<
	$(AS) $(ASFLAGS) -o $*.o $*.tmp
	rm -f $*.tmp

ifdef MULTISUBDIR
$(TOPDIR)/libc/build-ml/$(MULTISUBDIR)/%.o: %.S
	$(CC) -E $(MULTILIB) $(INCS) $(SDEFS) -MD -o $@.tmp $<
	$(AS) $(ASFLAGS) -o $@ $@.tmp
	rm -f $@.tmp
endif

.s.o:
	$(AS) $(ASFLAGS) -o $*.o $<

.c.o:
	$(CC) $(CFLAGS) -c -MD -o $*.o $<

-include *.d
