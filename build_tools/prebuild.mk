# Файл описывает правило следящее за необходимостью обновления объектных файлов.

# Если текущий объектный файл был скомпилирован с другими флагами компиялции,
# то его нужно перекомпилировать заного.


# --1. Проверка содержимого файла с флагами
# Переменная агрегирует текущие флаги компиляции
CURRENT_COMPILE_FLAGS = $(TYPE_OF_BUILD) $(CPP) $(CPPFLAGS) $(DEPFLAGS) $(DIRFLAGS) $(DEFFLAGS) -c $*.cpp

# Проверка строки в файле на соответствие с текущими флагами компиляции.
MATCHING_SEARCH = $(shell grep -s -F '$(CURRENT_COMPILE_FLAGS)' $(DEPS_PATH)/$1.flags)

# Функция сверяет текущие флаги компиляции с флагами компиляции объектника($1).
# Если флаги совпадают, то файл нам подходит, иначе говорим явно перекомпилировать.
MAKE_CHECK_ABOUT_FLAGS =  $(if $(MATCHING_SEARCH),,force)


# --2. Описание правила, следящего за обновлением объектников.
# Если объектник был сокмпилирован с другими флагами, то записываем текущие
# флаги в соответствующий файл тем самым сохраняя текущие флаги для дальнейшей
# следующих проверок и тригеря перекомпиляцию объектника.
.SECONDARY:
.SECONDEXPANSION:
$(DEPS_PATH)/%.flags: $$(call MAKE_CHECK_ABOUT_FLAGS,%)
	@mkdir -p $(dir $@) && echo '$(CURRENT_COMPILE_FLAGS)' > $@

# force нужен чтобы таргет, который от него зависит выполнялся всегда.
# В нашем случае если MAKE_CHECK_ABOUT_FLAGS вернет force, то правило выполнится (иначе нет).
.PHONY: force
force:
