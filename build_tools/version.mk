COMMIT := $(shell git rev-parse --verify --short HEAD)
BRANCH := $(shell git branch | grep \* | sed -e 's/[(*)]//g' | sed 's/^[ \t]*//')
CHANGES := $(shell git status --porcelain)

version:
	@echo Building version
	@echo Branch: $(BRANCH)
	@echo Commit: $(COMMIT)
	@mkdir -p .version
	@echo "#define PROJECT_BRANCH \"$(BRANCH)\"" > .version/version.h
	@echo "#define PROJECT_COMMIT \"$(COMMIT)\"" >> .version/version.h
	@echo "#define PROJECT_CHANGES \"$(CHANGES)\"" >> .version/version.h