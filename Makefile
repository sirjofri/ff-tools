include config.mk

VERSION=0.01pre01
DATE=June 06, 2018

CFLAGS_ADD= -D_POSIX_C_SOURCE=200809L -std=c99 -pedantic -Wall -g
TOOLS=$(shell ls ff-*.c | sed 's/\.c//g')
DEPS= tools.h

.PHONY: all
all: options $(OUT) $(addprefix $(OUT)/,$(TOOLS)) $(OUT)/tools.l

.PHONY: options
options:
	@echo "LIBS   = $(LIBS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CC     = $(CC)"
	@echo "OUT    = $(OUT)"

.PHONY: clean
clean:
	@rm -rf $(OUT) 2>/dev/null >/dev/null
	@echo " → Cleaned workspace directory"

install: $(addprefix $(PREFIX)/bin/,$(TOOLS))

$(PREFIX)/bin/%: $(OUT)/%.o tools.o
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ $< tools.o
	@echo " → Installed tools"

.PHONY: dist
dist: clean
	@echo " → make distribution pack"
	@mkdir -p ff-tools-$(VERSION)
	@cp -R Makefile Readme.md config.mk *.c *.h ff-tools-$(VERSION)
	@tar -cf ff-tools-$(VERSION).tar ff-tools-$(VERSION)
	@gzip ff-tools-$(VERSION).tar
	@rm -rf ff-tools-$(VERSION)

.PHONY: test
test: all
	@echo " → testing binaries"
	@./test.sh $(OUT)

demo.png: demo.sh
	@echo " → creating $@"
	@./$< $(OUT) | ff2png > $@

$(OUT):
	@mkdir -p $@

$(OUT)/tools.l: tools.l.in
	@cat $< | sed -e "s/VNUM/$(VERSION)/g" | sed -e "s/DATE/$(DATE)/g" > $@
	@echo " → $@ compiled"

$(OUT)/tools.o: tools.c tools.h
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ -c $<
	@echo " → $@ compiled"

$(OUT)/%.o: %.c $(DEPS)
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ -c $<
	@echo " → $@ compiled"

$(OUT)/%: $(OUT)/%.o tools.o
	@$(CC) $(LIBS) $(CFLAGS) $(CFLAGS_ADD) -o $@ $< tools.o
