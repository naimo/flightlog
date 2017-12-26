CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

VPATH:=$(VPATH):$(ROOT_DIR)/src

INCLUDES = src
INCLUDES += STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
INCLUDES += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Inc
INCLUDES += STM32CubeF3/Drivers/CMSIS/Include

include $(ROOT_DIR)/src/src.mk

CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_gpio.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_utils.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usart.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_spi.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_rcc.c

CFLAGS = -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -O0 -g
CFLAGS += -specs=nano.specs -DSTM32F303xC -DUSE_FULL_LL_DRIVER
CFLAGS += $(addprefix -I, $(INCLUDES))

LDFLAGS = -mcpu=cortex-m4 -mlittle-endian -mthumb
LDFLAGS += -specs=nosys.specs
LDFLAGS += -DSTM32F303xC -TSTM32F303CC_FLASH.ld -Wl,--gc-sections -g

OBJS_DIR = objs

ASRC = src/system/startup_stm32f303xc.s

OBJS = $(addprefix $(OBJS_DIR)/, $(CSRC:%.c=%.o) $(ASRC:%.s=%.o))

all : $(OBJS_DIR)/main.hex

$(OBJS_DIR)/main.hex : $(OBJS_DIR)/main.elf
	$(OBJCOPY) -I ihex $^ $@

$(OBJS_DIR)/main.elf : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS_DIR)/%.o : %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -r $(OBJS_DIR)/

flash : $(OBJS_DIR)/main.hex
	stm32flash -b 230400 -w objs/main.hex -v -g 0x0 /dev/ttyUSB0