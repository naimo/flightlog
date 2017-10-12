CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

INCLUDES = inc
INCLUDES += STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
INCLUDES += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Inc
INCLUDES += STM32CubeF3/Drivers/CMSIS/Include

CFLAGS = -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -O0 -g
CFLAGS += -specs=nano.specs -DSTM32F303xC -DUSE_FULL_LL_DRIVER
CFLAGS += $(addprefix -I, $(INCLUDES))

LDFLAGS = -mcpu=cortex-m4 -mlittle-endian -mthumb
LDFLAGS += -specs=nosys.specs
LDFLAGS += -DSTM32F303xC -TSTM32F303CC_FLASH.ld -Wl,--gc-sections -g

OBJS_DIR = objs

CSRC = src/main.c
CSRC += src/retarget.c
CSRC += src/i2c.c
CSRC += src/hmc5x83.c
CSRC += src/mpu6050.c
CSRC += src/ms5607.c
CSRC += src/system_stm32f3xx.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_gpio.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_utils.c
CSRC += STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usart.c

ASRC = src/startup_stm32f303xc.s

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