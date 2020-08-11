# Файл осуществляет непосредственную компиляцию исходников в объектные файлы.

# Необходимые переменные для совершения компиляции:
# AS       - Компилятор ассемблерных файлов.
# CC       - Компилятор C файлов.
# CPP      - Компилятор C++ файлов.
# SOURCES  - Список исходных файлов относительно директории текущего мейкфайла
# DIRS     - Список диркеторий, где происходит поиск заголовочных файлов.
# DEFS     - Дефайны(#define), которые передаются компилятору.
# BFLAGS   - Общие флаги передаваемые компилятору при обработке C и C++ исходников.
# CFLAGS   - Флаги передаваемые компилятору при обработке C исходников
# CPPFLAGS - Флаги передаваемые компилятору при обработке C++ исходников

include build_tools/settings.mk
include build_tools/prebuild.mk

# Вычисляем имена объектникиов основываясь на исходниках.
# (Заменяя суффиксы на .o)
TMP_OBJ_NAMES = $(SOURCES:.c=.o)
TMP_OBJ_NAMES2 = $(TMP_OBJ_NAMES:.s=.o)
TMP_OBJ_NAMES3 = $(TMP_OBJ_NAMES2:.S=.o)
TMP_OBJ_NAMES4 = $(TMP_OBJ_NAMES3:.cpp=.o)

# Вычисляем конечные пути расположения объектников и файлов с зависимостями
OBJS = $(addprefix $(OBJS_PATH)/,$(TMP_OBJ_NAMES4))
DEPS = $(addprefix $(DEPS_PATH)/,$(TMP_OBJ_NAMES4:.o=.d))

BFLAGS += $(OPTIMIZATION_LVL) $(FLTO_FLAG) -Wall -g3 -Wextra -Werror -ffunction-sections
CPPFLAGS += $(CPP_STANDARD)

# Переменная содержит флаги для генерации файла с зависимостями обрабатываемого исходника.
# Файл с зависимостями имеет путь: $(DEPS_PATH)/_path_to_source_/_source_name_.Td
DEPFLAGS = -MMD -MP -MF $(DEPS_PATH)/$*.Td
DIRFLAGS = $(addprefix -I,$(DIRS))
DEFFLAGS = $(addprefix -D,$(DEFS))

# Функция перемещает временные файлы с зависимостями (*.Td) в конечные *.d
# мануал: http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Никто не понимает смысл этих действий, но если вы поймете, то расскажите нам.
POSTCOMPILE = @mv -f $(DEPS_PATH)/$*.Td $(DEPS_PATH)/$*.d && touch $@

# Добавляем зависимость объектных файлов от мейкфайов,
# которые встречались до текущего момента.
COMMON_OBJ_DEPS := $(MAKEFILE_LIST)

# Инклюдим файлы с зависимостями (*.d).
-include $(DEPS)

# Правила создания объектных файлов из исходников.
$(OBJS_PATH)/%.o: %.c $(COMMON_OBJ_DEPS) $(DEPS_PATH)/%.d
	@echo [CC] $<
	@mkdir -p $(dir $(DEPS_PATH)/$<)
	@mkdir -p $(dir $(OBJS_PATH)/$<)
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(DIRFLAGS) $(DEFFLAGS) -c $< -o $@
	@$(POSTCOMPILE)

$(OBJS_PATH)/%.o: %.cpp $(COMMON_OBJ_DEPS) $(DEPS_PATH)/%.d $(DEPS_PATH)/%.flags
	@echo [CPP] $<
	@mkdir -p $(dir $(DEPS_PATH)/$<)
	@mkdir -p $(dir $(OBJS_PATH)/$<)
	@$(CPP) $(CPPFLAGS) $(DEPFLAGS) $(DIRFLAGS) $(DEFFLAGS) -c $< -o $@
	@$(POSTCOMPILE)
	
# Сообщаем мейку, чтобы не удалял наши файлы с зависимостями после
# окончания их использования.
$(DEPS_PATH)/%.d: ;
.PRECIOUS: $(DEPS_PATH)/%.d
