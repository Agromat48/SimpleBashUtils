CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

all: s21_cat s21_grep

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c

s21_grep: s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

test: all
	bash test_cat.sh
	bash test_grep.sh

clean:
	rm -f s21_cat s21_grep *.o

rebuild: clean all

.PHONY: all clean rebuild test
