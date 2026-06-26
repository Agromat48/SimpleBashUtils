#include "s21_grep.h"

struct option const long_flags[] = {{NULL, 0, NULL, 0}};

int ReadTemplateFromFile(regex_t *templates, int *templates_counter,
                         Flags flags, char *filename) {
  int is_error = 0;
  char buffer[1024];

  FILE *fp = fopen(filename, "r");
  is_error = !fp;

  while (!is_error && fgets(buffer, (int)sizeof(buffer), fp)) {
    int buffer_length = (int)strlen(buffer);
    if (buffer_length > 0 && buffer[buffer_length - 1] == '\n')
      buffer[--buffer_length] = '\0';
    regcomp(&templates[(*templates_counter)++], buffer,
            flags.i ? REG_ICASE : REG_EXTENDED);
  }

  if (!is_error) fclose(fp);
  return is_error;
}

void FreeTemplates(regex_t *templates, int count) {
  int i = 0;
  while (i < count) {
    regfree(&templates[i]);
    i++;
  }
}

int FlagsProcessing(regex_t *templates, int templates_counter,
                    const char *filename, Flags flags) {
  int is_error = 0;
  int line_count = 0;
  int match_count = 0;
  char line[4096];

  FILE *fp = fopen(filename, "r");
  is_error = !fp;

  while (!is_error && feof(fp) == 0 && fgets(line, 4096, fp)) {
    char match = 0;
    int buffer_length = (int)strlen(line);
    line_count++;

    int i = 0;
    while (i < templates_counter) {
      int line_position = 0;
      regmatch_t regmatch;
      int exec_res =
          regexec(&templates[i], &line[line_position], 1, &regmatch, 0);

      while (exec_res == 0) {
        if (flags.o && !flags.v && !flags.c && !flags.l) {
          if (!line_position && !flags.h) printf("%s:", filename);
          if (!line_position && flags.n) printf("%d:", line_count);
          int k = regmatch.rm_so;
          while (k < regmatch.rm_eo) {
            printf("%c", line[line_position + k]);
            k++;
          }
          printf("\n");
        }
        match = 1;
        if (regmatch.rm_eo == 0) {
          exec_res = 1;
        } else {
          line_position += regmatch.rm_eo;
          exec_res =
              regexec(&templates[i], &line[line_position], 1, &regmatch, 0);
        }
      }
      i++;
    }

    if (flags.v) match = !match;
    match_count += match;

    if (match && !flags.c && !flags.l && (!flags.o || flags.v)) {
      if (!flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", line_count);
      int j = 0;
      while (j < buffer_length && line[j] != '\n') {
        printf("%c", line[j]);
        j++;
      }
      printf("\n");
    }
  }

  if (!is_error && flags.c && !flags.l) {
    if (!flags.h) printf("%s:", filename);
    printf("%d\n", match_count);
  }

  if (flags.l && !is_error && match_count > 0) printf("%s\n", filename);
  if (!is_error) fclose(fp);
  return is_error;
}

int main(int argc, char *argv[]) {
  regex_t templates[1024];
  Flags flags = {0};
  int is_error = 0;
  int template_counter = 0;
  int opt = 0;

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", long_flags, NULL)) !=
             -1 &&
         !is_error) {
    if (opt == 'e') {
      regcomp(&templates[template_counter++], optarg,
              flags.i ? REG_ICASE : REG_EXTENDED);
      flags.e = 1;
    } else if (opt == 'i') {
      flags.i = 1;
    } else if (opt == 'v') {
      flags.v = 1;
    } else if (opt == 'c') {
      flags.c = 1;
    } else if (opt == 'l') {
      flags.l = 1;
    } else if (opt == 'n') {
      flags.n = 1;
    } else if (opt == 'h') {
      flags.h = 1;
    } else if (opt == 's') {
      flags.s = 1;
    } else if (opt == 'f') {
      flags.f = 1;
      is_error =
          ReadTemplateFromFile(templates, &template_counter, flags, optarg);
    } else if (opt == 'o') {
      flags.o = 1;
    } else {
      is_error = 1;
      fprintf(stderr, GREP_ERROR);
    }
  }

  if (!is_error && (optind + 1 - (flags.f || flags.e)) < argc) {
    if (!(flags.f || flags.e))
      regcomp(&templates[template_counter++], argv[optind++],
              flags.i ? REG_ICASE : REG_EXTENDED);
    if (optind == argc - 1) flags.h = 1;

    int i = optind;
    while (!is_error && i < argc) {
      if (FlagsProcessing(templates, template_counter, argv[i], flags) &&
          !flags.s)
        fprintf(stderr, GREP_ERROR);
      i++;
    }
  } else if (!is_error) {
    fprintf(stderr, GREP_ERROR);
  }

  FreeTemplates(templates, template_counter);
  return is_error;
}