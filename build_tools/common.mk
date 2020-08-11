# Общие настройки для сборки проектов
.PHONY: clean
clean: clean_all
	rm -rf bin

.PHONY: clean_all
clean_all:
	rm -rf .internals

# Выведем переменную BUILD
ifndef BUILD
export BUILD := $(MAKECMDGOALS)
endif

# Если имя для итогового файла не задали установим default
ifndef TARGET
export TARGET := default
endif

# Место для хранение файлов сборки (.d/.flags/.o)
# основывается на BULID из главного makefile'а.
ifndef INTERNAL_BUILD_PATH
export INTERNAL_BUILD_PATH := $(BUILD)
endif

####### Линковка библиотеки libvs, если она установлена ####### 

SEARCH_VSCAN_LIB = $(shell find -type f -name "libvs*")
LDFLAGS += $(if $(SEARCH_VSCAN_LIB),-Lport/unix/vs_can/libs -lm -lvs_can_api_x86-64, )
export LDFLAGS
