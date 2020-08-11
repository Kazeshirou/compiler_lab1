# Файл содержит общие настройки для компиляции и линковки.

# --0. Выбираем место хранения долгоживущих файлов.

# Папка *.d содержит флаги компиляции предыдущей сборки (.flags),
# а также файлы с указанием зависиомстей (.d). Файлы остаются с предыдущих
# компиляций и на их основе решается - требуется ли перекомпиляция исходника.

# Папка *.objs содержит объектные файлы на основе которыйх линкуется
# конечный бинарник.

# INTERNAL_BUILD_PATH это часть пути основывется на значении BUILD самого
# главного makefile'а.
DEPS_PATH := .internals/$(INTERNAL_BUILD_PATH).$(TARGET).d
OBJS_PATH := .internals/$(INTERNAL_BUILD_PATH).$(TARGET).objs

# --1. Выбираем уровень оптимизации
# Если не задан другой уровень оптимизации то по умолчанию -Os
ifneq (,$(filter $(shell echo $(OPTIMIZATION) | tr A-Z a-z),off false disable))
OPTIMIZATION_LVL := -O0
else
OPTIMIZATION := on
OPTIMIZATION_LVL := -Os
endif

# --2. Выбираем тип сборки debug/release
# Включение отладочных функций (DEBUG/RELEASE сборка)
ifneq (,$(filter $(shell echo $(DEBUG) | tr A-Z a-z),on true enable))
TYPE_OF_BUILD = DEBUG
DEFS += DEBUG_BUILD
else
TYPE_OF_BUILD = RELEASE
DEFS += RELEASE_BUILD
endif

# --3. Выбираем с каким стандартом собирать
# Если никто не сказал обратного то мы всегда собираем с 17
ifndef CPP_STANDARD
CPP_STANDARD := -std=c++17
# Так как собираем под 17 стандарт register - deprecated
DEFS += register=
endif

# --4. Выбираем включать ли flto. Вычисляемое в момент запроса значение
# FLTO_FLAG = $(if $(filter %OPTIM=off %OPTIMIZATION=off,$(MAKEFLAGS)),,-flto)
# На текущий момент принято решение убрать совсем flto
FLTO_FLAG = $(if $(filter %OPTIM=off %OPTIMIZATION=off,$(MAKEFLAGS)),,)

# --5. Добавим define в код в соответствии с значением BUILD_TYPE в upper_case
ifdef BUILD_TYPE
DEFS += BUILD_TYPE_$(shell echo $(BUILD_TYPE) | tr a-z A-Z)
endif

# --6. Добавим define в код для event_logger
ifneq (,$(filter $(shell echo $(LOGGER) | tr A-Z a-z),on true enable))
DEFS += EVENT_LOGGER
endif
