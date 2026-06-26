#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define CAT_ERROR "usage: s21_cat [-benstv] [file ...]\n"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct CatFlags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} CatFlags;

struct option const cat_long_flags[];

void PrintChar(int c, CatFlags flags);
int ProcessFile(const char *filename, CatFlags flags, int *line_num,
                int *prev_blank);

#endif  // SRC_CAT_S21_CAT_H_