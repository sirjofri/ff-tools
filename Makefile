include config.mk

CFLAGS_ADD= -D_POSIX_C_SOURCE=200809L -std=c99 -pedantic
SRC= ff-singen.c ff-cosgen.c ff-mul.c
TOOLS= $(OUT)/ff-singen $(OUT)/ff-cosgen $(OUT)/ff-mul

.PHONY: all
all: options $(OUT) $(TOOLS)

.PHONY: options
options:
	@echo "LIBS   = $(LIBS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CC     = $(CC)"
	@echo "OUT    = $(OUT)"

.PHONY: clean
clean:
	@rm $(TOOLS)

.PHONY: dist
dist: clean
	@echo " → make distribution pack"
	@mkdir -p ff-tools-$(VERSION)
	@cp -R Makefile Readme.md config.mk $(SRC) ff-tools-$(VERSION)
	@tar -cf ff-tools-$(VERSION).tar ff-tools-$(VERSION)
	@gzip ff-tools-$(VERSION).tar
	@rm -rf ff-tools-$(VERSION)

$(OUT):
	@mkdir -p $@

$(OUT)/ff-singen: ff-singen.c
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ $<
	@echo " → $@ compiled"

$(OUT)/ff-cosgen: ff-cosgen.c
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ $<
	@echo " → $@ compiled"

$(OUT)/ff-mul: ff-mul.c
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ $<
	@echo " → $@ compiled"
