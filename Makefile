CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

all: s21_cat

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c

test: s21_cat
	bash test_cat.sh

clean:
	rm -f s21_cat *.o

rebuild: clean all

.PHONY: all clean rebuild test check fix