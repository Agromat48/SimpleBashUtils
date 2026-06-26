#include "s21_cat.h"

struct option const cat_long_flags[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}};

void PrintChar(int c, CatFlags flags) {
  int show_nonprint = flags.v || flags.e;
  if (c == '\n') {
    if (flags.e) printf("$");
    printf("\n");
  } else if (c == '\t') {
    if (flags.t) {
      printf("^I");
    } else {
      printf("%c", c);
    }
  } else if (show_nonprint && c == 127) {
    printf("^?");
  } else if (show_nonprint && c >= 128) {
    printf("M-");
    PrintChar(c - 128, flags);
  } else if (show_nonprint && c < 32) {
    printf("^%c", c + 64);
  } else {
    printf("%c", c);
  }
}

int ProcessFile(const char *filename, CatFlags flags, int *line_num,
                int *prev_blank) {
  int is_error = 0;
  FILE *fp = fopen(filename, "r");

  if (!fp) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
    is_error = 1;
  }

  int c = 0;
  int at_line_start = 1;

  if (!is_error) c = fgetc(fp);

  while (!is_error && c != EOF) {
    int cur_blank = (c == '\n' && at_line_start);

    if (!(flags.s && cur_blank && *prev_blank)) {
      if (at_line_start) {
        if (!cur_blank) {
          if (flags.b) {
            printf("%6d\t", ++(*line_num));
          } else if (flags.n) {
            printf("%6d\t", ++(*line_num));
          }
        } else if (flags.n && !flags.b) {
          printf("%6d\t", ++(*line_num));
        }
      }
      PrintChar(c, flags);
      *prev_blank = cur_blank;
    }

    at_line_start = (c == '\n');
    c = fgetc(fp);
  }

  if (!is_error) fclose(fp);
  return is_error;
}

int main(int argc, char *argv[]) {
  CatFlags flags = {0};
  int is_error = 0;
  int opt = 0;

  while ((opt = getopt_long(argc, argv, "beEnstTv", cat_long_flags, NULL)) !=
             -1 &&
         !is_error) {
    if (opt == 'b') {
      flags.b = 1;
    } else if (opt == 'e') {
      flags.e = 1;
      flags.v = 1;
    } else if (opt == 'E') {
      flags.e = 1;
    } else if (opt == 'n') {
      flags.n = 1;
    } else if (opt == 's') {
      flags.s = 1;
    } else if (opt == 't') {
      flags.t = 1;
      flags.v = 1;
    } else if (opt == 'T') {
      flags.t = 1;
    } else if (opt == 'v') {
      flags.v = 1;
    } else {
      is_error = 1;
      fprintf(stderr, CAT_ERROR);
    }
  }

  if (flags.b) flags.n = 0;

  if (!is_error && optind >= argc) {
    is_error = 1;
    fprintf(stderr, CAT_ERROR);
  }

  int line_num = 0;
  int prev_blank = 0;
  int i = optind;

  while (!is_error && i < argc) {
    if (ProcessFile(argv[i], flags, &line_num, &prev_blank)) is_error = 1;
    i++;
  }

  return is_error;
}