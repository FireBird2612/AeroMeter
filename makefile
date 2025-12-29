TARGET = EnvMonitor

# Define your target processor
CPU = cortex-m4

.PHONY: all clean size directory	flash

# Linker Script Location
LD_SCRIPT = linker-script/stm32f446xe_flash.ld

# Misc
FLASH = 0x08000000
DEV = /dev/tty.usbmodem1102

# Toolchains
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
SZ = arm-none-eabi-size

# Directories
BUILD_DIR = build/
OBJ_DIR = $(BUILD_DIR)obj/
SRC_DIR = src/
ASM_DIR = $(SRC_DIR)

# Include paths
INCLUDES = \
-Ivendor/CMSIS/inc \
-Ivendor/CMSIS/ARM \
-Ivendor/CMSIS/ARM/inc	\
-Ivendor/HAL/inc \
-Iinc

# Source files
SRCS = \
startup-file/startup_stm32f446xx.S \
	src/main.c	\
	src/sys_init.c	\
	src/monotonic_clk.c	\
	src/ulogger.c	\
	src/syscalls.c


DEFINES = \
	-DSTM32F446xx \
	-DOSCIN_FEQ=16000000	\
	-DOSCEX_FEQ=8000000

# Assembly directives
ASFLAGS = -c
ASFLAGS += -Wall
ASFLAGS += -mcpu=$(CPU)
ASFLAGS += -mfloat-abi=hard
ASFLAGS += -mfpu=fpv4-sp-d16
ASFLAGS += -mthumb
ASFLAGS += -O0

# Compilation flags
CFLAGS = -W -Wall --std=gnu99 -g3 -O0
CFLAGS += -mcpu=$(CPU) -mthumb
CFLAGS += -mfloat-abi=hard
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -mtune=cortex-m4
CFLAGS += -MD -MP

# Linker flags
LDFLAGS = -mcpu=$(CPU) -mthumb
LDFLAGS += -mfloat-abi=hard
LDFLAGS += -mfpu=fpv4-sp-d16
LDFLAGS += -specs=nano.specs
LDFLAGS += -lc -lm
LDFLAGS += -lgcc
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--script=$(LD_SCRIPT)
LDFLAGS += -lgcc
LDFLAGS += -Wl,--gc-sections

# Combine flags
CFLAGS += $(INCLUDES) $(DEFINES)

# Object files
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(subst .c,.o, $(subst .S,.o, $(SRCS)))))

# Targets
all: directory $(BUILD_DIR)$(TARGET).elf $(BUILD_DIR)$(TARGET).hex $(BUILD_DIR)$(TARGET).bin size

directory:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

$(BUILD_DIR)$(TARGET).elf: $(OBJS)
	@echo "LD $@"
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(BUILD_DIR)$(TARGET).hex: $(BUILD_DIR)$(TARGET).elf
	@echo "OC $@"
	@$(OC) -O ihex $^ $@

$(BUILD_DIR)$(TARGET).bin: $(BUILD_DIR)$(TARGET).elf
	@echo "OC $@"
	@$(OC) -O binary $^ $@

# Compilation rules
$(OBJ_DIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "CC $@"
	@$(CC) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)%.o: vendor/CMSIS/src/%.c
	@mkdir -p $(dir $@)
	@echo "CC $@"
	@$(CC) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)%.o: startup-file/%.S
	@mkdir -p $(dir $@)
	@echo "AS $@"
	@$(CC) $(ASFLAGS) $(CFLAGS) $< -c -o $@

size: $(BUILD_DIR)$(TARGET).elf
	@echo "size:"
	@$(SZ) $^

clean:
	@echo "clean"
	@-rm -rf $(BUILD_DIR)

flash:
	st-flash	write	$(BUILD_DIR)$(TARGET).bin	0x08000000

# Include dependency files
-include $(wildcard $(OBJ_DIR)*.d)
