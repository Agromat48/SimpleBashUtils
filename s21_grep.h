#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define GREP_ERROR "usage: s21_grep [-ivclnhso] [-e pattern] [-f file] file\n"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

struct option const long_flags[];

int ReadTemplateFromFile(regex_t *templates, int *templates_counter,
                         Flags flags, char *filename);
void FreeTemplates(regex_t *templates, int template_count);
int FlagsProcessing(regex_t *templates, int templates_counter,
                    const char *filename, Flags flags);

#endif  // SRC_GREP_S21_GREP_H_