all : flash

TARGET:=u2hts_v003j4

TOUCH_CONTROLLER ?= ./u2hts_touch_controllers/gt9xx.c

ADDITIONAL_C_FILES+=./rv003usb/rv003usb/rv003usb.S \
					./rv003usb/rv003usb/rv003usb.c \
					./u2hts_core/u2hts_core.c \
					./u2hts_v003j4_impl.c \
					./CH32V00x_lib_i2c/lib_i2c.c \
					$(TOUCH_CONTROLLER)
EXTRA_CFLAGS:= -I./rv003usb/rv003usb -I./u2hts_core -I./ch32v003fun_libs -I./CH32V00x_lib_i2c -DU2HTS_LOG_LEVEL=-1 -DU2HTS_MAX_TPS=10 -DU2HTS_ENABLE_COMPACT_REPORT
LINKER_SCRIPT:=./ch32v003_flash.ld
TARGET_MCU?=CH32V003
include ch32fun/ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean