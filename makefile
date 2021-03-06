include build_tools/common.mk

# Имя для результирующего запускаемого файла
TARGET := lab1

.PHONY: unix
unix::
	@echo - Building $(TARGET) for $(BUILD)
	@make all -j4 --makefile=sources/sources.mk
	@echo - Executing linker
	@make all -j4 --makefile=build_tools/link_mpu.mk

.PHONY: test
test::
	@echo - Building $(TARGET) for $(BUILD)
	@make all -j4 --makefile=sources/sources.mk BUILD=test
	@echo - Executing linker
	@make all -j4 --makefile=build_tools/link_mpu.mk

.PHONY: graph
graph::
	@dot -Tpng bin/nfa.dot -o bin/nfa.png
	@dot -Tpng bin/dfa.dot -o bin/dfa.png