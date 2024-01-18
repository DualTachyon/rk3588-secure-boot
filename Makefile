TARGET := enable-sb

OBJS =
OBJS += entry.o
OBJS += main.o
OBJS += exceptions.o
OBJS += libc.o
OBJS += otp.o
OBJS += printf.o
OBJS += putchar.o
OBJS += swd.o
OBJS += uart.o
OBJS += vbar.o

USE_CLANG ?= 1

ifeq ($(USE_CLANG),0)
CC = aarch64-linux-gnu-gcc
else
ifeq ($(OS),Windows_NT)
CC = clang --target=aarch64
else
CC = clang --target=aarch64 -isystem /usr/aarch64-linux-gnu/include
endif
endif

AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy
RM = rm -f

CFLAGS = -c -O2 -Wall -Werror -std=c11 -MMD
ifeq ($(USE_CLANG),1)
CFLAGS += -Weverything
CFLAGS += -mno-unaligned-access
endif
CFLAGS += -U_FORTIFY_SOURCE
CFLAGS += -mgeneral-regs-only
CFLAGS += -fno-stack-protector
CFLAGS += -fno-builtin-printf
CFLAGS += -fno-builtin
CFLAGS += -Wno-reserved-macro-identifier
CFLAGS += -Wno-reserved-identifier
CFLAGS += -DPRINTF_INCLUDE_CONFIG_H
CFLAGS += -Ihw

DEPS = $(OBJS:.o=.d)

all: $(TARGET).bin

#../rk-tools/rk-packsign --usb --key private.pem -i $(TARGET).bin -o $(TARGET).usb.bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $^ -o $@ -T linker.ld

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) -c -D__ASSEMBLY__ $< -o $@

-include $(DEPS)

clean:
	$(RM) $(TARGET).usb.bin $(TARGET).bin $(TARGET).elf $(OBJS) $(DEPS)

