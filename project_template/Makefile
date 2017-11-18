######################################
# host os
######################################
OS := $(shell uname)

######################################
# target
######################################
TARGET = cc2650_project_template

include ../makedef

######################################
# building variables
######################################
# debug build?
DEBUG := 1
# optimization
OPT := -O0


#######################################
# paths
#######################################
# source path
SOURCES_DIR =  \
Application/User \
Drivers \
Application \
Application/MAKEFILE \
Middlewares/USB_Device_Library \
Drivers/STM32F1xx_HAL_Driver \
Drivers/CMSIS \
Middlewares

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# cc26xxware source
######################################
CC26XXWARE_SRC = \
$(CC26XXWARE_DRVLIB)/adi.c \
$(CC26XXWARE_DRVLIB)/aon_batmon.c \
$(CC26XXWARE_DRVLIB)/aon_event.c \
$(CC26XXWARE_DRVLIB)/aon_ioc.c \
$(CC26XXWARE_DRVLIB)/aon_rtc.c \
$(CC26XXWARE_DRVLIB)/aon_wuc.c \
$(CC26XXWARE_DRVLIB)/aux_adc.c \
$(CC26XXWARE_DRVLIB)/aux_smph.c \
$(CC26XXWARE_DRVLIB)/aux_tdc.c \
$(CC26XXWARE_DRVLIB)/aux_timer.c \
$(CC26XXWARE_DRVLIB)/aux_wuc.c \
$(CC26XXWARE_DRVLIB)/ccfgread.c \
$(CC26XXWARE_DRVLIB)/chipinfo.c \
$(CC26XXWARE_DRVLIB)/cpu.c \
$(CC26XXWARE_DRVLIB)/crypto.c \
$(CC26XXWARE_DRVLIB)/ddi.c \
$(CC26XXWARE_DRVLIB)/debug.c \
$(CC26XXWARE_DRVLIB)/driverlib_release.c \
$(CC26XXWARE_DRVLIB)/event.c \
$(CC26XXWARE_DRVLIB)/flash.c \
$(CC26XXWARE_DRVLIB)/gpio.c \
$(CC26XXWARE_DRVLIB)/prcm.c \
$(CC26XXWARE_DRVLIB)/pwr_ctrl.c \
$(CC26XXWARE_DRVLIB)/rfc.c \
$(CC26XXWARE_DRVLIB)/rom_crypto.c \
$(CC26XXWARE_DRVLIB)/setup.c \
$(CC26XXWARE_DRVLIB)/smph.c \
$(CC26XXWARE_DRVLIB)/ssi.c \
$(CC26XXWARE_DRVLIB)/osc.c \
$(CC26XXWARE_DRVLIB)/ioc.c \
$(CC26XXWARE_DRVLIB)/sys_ctrl.c \
$(CC26XXWARE_DRVLIB)/systick.c \
$(CC26XXWARE_DRVLIB)/timer.c \
$(CC26XXWARE_DRVLIB)/trng.c \
$(CC26XXWARE_DRVLIB)/uart.c \
$(CC26XXWARE_DRVLIB)/udma.c \
$(CC26XXWARE_DRVLIB)/vims.c \
$(CC26XXWARE_DRVLIB)/watchdog.c \

######################################
# your application source
######################################
APP_DIR=.
APP_SRC=														\
$(APP_DIR)/main.c                   \
$(APP_DIR)/startup_gcc.c            \
$(APP_DIR)/ccfg.c


C_SOURCES = $(CC26XXWARE_SRC) $(APP_SRC)

# ASM sources
ASM_SOURCES =


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS := -DDRIVERLIB_NOROM
# C_DEFS :=


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I$(CC26XXWARE_DRVLIB) \
-I$(CC26XXWARE_INC)


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Werror -fno-strict-aliasing -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fno-strict-aliasing -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
#CFLAGS += -g -gdwarf-2
CFLAGS += -g3
endif


# Generate dependency information
#CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
CFLAGS += -MMD -MF .dep/$(*F).d -std=c99


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT := $(CC26XXWARE_LDS)

# libraries
LIBS := -lc -lm -lnosys
LIBDIR :=
#LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -u _printf_float
LDFLAGS = $(MCU) -nostartfiles -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -u _printf_float

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
	-rm -fR .dep $(BUILD_DIR)

#######################################
# flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).bin
	$(STLINKPATH)/st-flash write $< 0x08000000

#######################################
# openocd
#######################################
openocd: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/jlink.cfg -f target/cc26xx.cfg
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
