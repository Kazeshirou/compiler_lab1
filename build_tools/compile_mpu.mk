# Файл описывает специфичные переменные для компиляции под unix.
# Полный список переменных с описанием можно найти в файле осуществляющем
# непосредственно компиляцию build_tools/compile_common.mk

CPP :=  g++

CFLAGS += $(BFLAGS)
CPPFLAGS += $(BFLAGS) -pthread

include build_tools/compile_common.mk

# Главное правило, которое требует объектные файлы.
all test: $(OBJS)
