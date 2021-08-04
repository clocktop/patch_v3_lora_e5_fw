##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.14.1] date: [Tue Aug 03 17:04:07 PDT 2021]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = patch_v3_lora_e5


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/adc.c \
Src/dma.c \
Src/user_diskio.c \
Src/app_fatfs.c \
Src/stm32_lpm_if.c \
Src/sys_app.c \
Src/sys_debug.c \
Src/sys_sensors.c \
Src/app_lorawan.c \
Src/CayenneLpp.c \
Src/lora_app.c \
Src/lora_info.c \
Src/radio_board_if.c \
Src/timer_if.c \
Src/adc_if.c \
Src/usart_if.c \
Src/rtc.c \
Src/spi.c \
Src/subghz.c \
Src/usart.c \
Src/stm32wlxx_it.c \
Src/stm32wlxx_hal_msp.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_adc.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_adc_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_ll_adc.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rcc.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rcc_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_flash.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_flash_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_gpio.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_dma.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_dma_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_pwr.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_pwr_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_cortex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_exti.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rtc.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rtc_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_spi.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_spi_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_subghz.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_tim.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_tim_ex.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_uart.c \
Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_uart_ex.c \
Src/system_stm32wlxx.c \
Middlewares/Third_Party/FatFs/src/diskio.c \
Middlewares/Third_Party/FatFs/src/ff.c \
Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
Middlewares/Third_Party/FatFs/src/option/syscall.c \
Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.c \
Middlewares/Third_Party/LoRaWAN/Crypto/soft-se.c \
Middlewares/Third_Party/LoRaWAN/Crypto/cmac.c \
Middlewares/Third_Party/LoRaWAN/Crypto/lorawan_aes.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionKR920.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionCN779.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionEU433.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionRU864.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/Region.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionUS915.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionEU868.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionBaseUS.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionCN470.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionCommon.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionIN865.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionAS923.c \
Middlewares/Third_Party/LoRaWAN/Mac/Region/RegionAU915.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacParser.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacCommands.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacSerializer.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMac.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacConfirmQueue.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacAdr.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacCrypto.c \
Middlewares/Third_Party/LoRaWAN/Mac/LoRaMacClassB.c \
Middlewares/Third_Party/LoRaWAN/LmHandler/NvmDataMgmt.c \
Middlewares/Third_Party/LoRaWAN/LmHandler/LmHandler.c \
Middlewares/Third_Party/LoRaWAN/Utilities/utilities.c \
Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver/radio_driver.c \
Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver/radio_fw.c \
Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver/radio.c \
Utilities/trace/adv_trace/stm32_adv_trace.c \
Utilities/misc/stm32_mem.c \
Utilities/misc/stm32_systime.c \
Utilities/misc/stm32_tiny_sscanf.c \
Utilities/misc/stm32_tiny_vsnprintf.c \
Utilities/sequencer/stm32_seq.c \
Utilities/timer/stm32_timer.c \
Utilities/lpm/tiny_lpm/stm32_lpm.c  

# ASM sources
ASM_SOURCES =  \
startup_stm32wle5xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DCORE_CM4 \
-DUSE_HAL_DRIVER \
-DSTM32WLE5xx


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32WLxx_HAL_Driver/Inc \
-IDrivers/STM32WLxx_HAL_Driver/Inc/Legacy \
-IUtilities/trace/adv_trace \
-IUtilities/misc \
-IUtilities/sequencer \
-IUtilities/timer \
-IUtilities/lpm/tiny_lpm \
-IMiddlewares/Third_Party/FatFs/src \
-IMiddlewares/Third_Party/LoRaWAN/LmHandler/Packages \
-IMiddlewares/Third_Party/SubGHz_Phy \
-IMiddlewares/Third_Party/SubGHz_Phy/stm32_radio_driver \
-IDrivers/CMSIS/Device/ST/STM32WLxx/Include \
-IMiddlewares/Third_Party/LoRaWAN/Crypto \
-IMiddlewares/Third_Party/LoRaWAN/Mac/Region \
-IMiddlewares/Third_Party/LoRaWAN/Mac \
-IMiddlewares/Third_Party/LoRaWAN/LmHandler \
-IMiddlewares/Third_Party/LoRaWAN/Utilities \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32WLE5XX_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***