# Файл осуществляет непосредственную линковку объектных файлов в бинарник.

# Необходимые переменные для совершения линковки:
# LD         - Линковщик (бинарник).
# OBJDUMP    - Бинарник используется для генерации асемблерного файла (man objdump).
# READELF    - Бинарник не используется (man readelf).
# PRINT_SIZE - Юниксовая команда для вывода текущего размера.
# CFLAGS     - Сишные флаги линковки.
# LDFLAGS    - Общие флаги линковки.

include build_tools/version.mk
include build_tools/settings.mk

# Собираем объектники для компиляции.
OBJS := $(shell find $(OBJS_PATH) -name "*.o" )

# Пути расположения конечных файлов.
BINARY_FILE := bin/$(BUILD)_$(TARGET).elf
MAP_FILE := bin/$(BUILD)_$(TARGET).map
ASM_FILE := bin/$(BUILD)_$(TARGET).asm

# Добавляем общие флаги линковки
CFLAGS += -Wall -g3 -Wextra -Werror
LDFLAGS += -Xlinker --gc-sections
LDFLAGS += -o $@ -Wl,-Map="$(MAP_FILE)" $(MEMORY_PRINT)
LDFLAGS	+= -lm 

# Добавляем зависимость бинарника от мейкфайов, которые встречались
# до текущего момента
COMMON_OBJ_DEPS := $(MAKEFILE_LIST)

$(BINARY_FILE): $(OBJS) $(COMMON_OBJ_DEPS)
	@mkdir -p bin
	@echo [LD] $@
	@echo
	@$(MESSAGE)
	@$(LD) $(CFLAGS) $(OBJS) $(LDFLAGS)
	@echo 'Size summary:'
	@$(PRINT_SIZE)