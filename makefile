SRCD := src
OUTD := build
SRC := $(wildcard $(SRCD)/*.c)
OBJ := $(patsubst $(SRCD)/%.c, $(OUTD)/%.o, $(SRC))
TEST := $(OUTD)/main
TARG := $(OUTD)/libimp.a
TARGN:= imp
FLAGS := -Iincludes -L$(OUTD) -l$(TARGN)

$(TEST): $(TARG) main.c
	gcc -o $@ main.c $(FLAGS)

$(OUTD)/%.o: $(SRCD)/%.c
	gcc -c -o $@ $< $(FLAGS)

$(TARG): $(OBJ)
	ar rcs $@ $^


