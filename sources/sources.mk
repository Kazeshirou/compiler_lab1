DIRS += $(shell find sources/ -type d -not -path "sources/tests/*")

SOURCES += $(shell find sources/common/ -name "*.cpp" -not -path "sources/tests/*")

ifeq ($(BUILD), unix)
SOURCES += sources/main.cpp
endif

ifeq ($(BUILD), test)
DIRS += $(shell find sources/tests/ -type d)
SOURCES +=$(shell find sources/tests/ -name "*.cpp")
endif

include build_tools/compile_mpu.mk