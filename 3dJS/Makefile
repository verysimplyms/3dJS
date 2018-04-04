TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),3DS)
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif

    ifeq ($(strip $(DEVKITARM)),)
        $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
    endif
endif

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source
LIBRARY_DIRS := $(DEVKITPRO)/libctru
LIBRARIES := ctru m

NAME  := 3dJS

TITLE := 3dJS interpreter - beta 1
DESCRIPTION := JavaScript interpreter for 3DS
AUTHOR := XorTroll
PRODUCT_CODE := CTR-P-AJSP
UNIQUE_ID := 0xABFF

SYSTEM_MODE := 64MB
SYSTEM_MODE_EXT := Legacy

BANNER_AUDIO := Sfx.wav
BANNER_IMAGE := Banner.png
ICON := Icon.png

include buildtools/make_base
