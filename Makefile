LIBS= -lm
FLAGS= -D_POSIX_C_SOURCE=200809L -Wall -std=c99 -pedantic
OUT= ./out

.PHONY: all
all: $(OUT) $(OUT)/ff-singen $(OUT)/ff-cosgen $(OUT)/ff-mul

$(OUT):
	mkdir -p $@

$(OUT)/ff-singen: ff-singen.c
	gcc $(LIBS) $(FLAGS) -o $@ $<

$(OUT)/ff-cosgen: ff-cosgen.c
	gcc $(LIBS) $(FLAGS) -o $@ $<

$(OUT)/ff-mul: ff-mul.c
	gcc $(LIBS) $(FLAGS) -o $@ $<
