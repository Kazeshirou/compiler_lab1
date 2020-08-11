# Файл описывает специфичные переменные для линковки бинарника под микроконтроллер.
# Полный список переменных с описанием можно найти в файле осуществляющем
# непосредственно линковку: build_tools/link_common.mk

LD := g++
OBJDUMP := objdump
READELF := readelf
SIZE := size

# зачем --no-as-needed - https://gcc.gnu.org/ml/gcc-help/2017-03/msg00081.html
CFLAGS = -Wl,--no-as-needed -pthread

ifeq ($(BUILD),test)
LDFLAGS += -lgcov -lgtest -lgmock
endif

MEMORY_PRINT :=
MESSAGE :=
PRINT_SIZE = $(SIZE) --format=berkeley $(BINARY_FILE)

include build_tools/link_common.mk

all: $(BINARY_FILE) $(ASM_GEN)
