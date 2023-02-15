#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
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
} opt;

void parser(int argc, char *argv[], opt *options, char *e_flags);
void output(char *argv[], opt *options, int *str_count, int *flag_for_l,
            char *e_flags, int many_files, int optind_for_o);

int main(int argc, char *argv[]) {
  int str_count = 0, flag_for_l = 0, many_files = 0, files_count = 0;
  char e_flags[1000] = {
      0,
  };
  FILE *input2;
  opt options = {0};
  if (argc > 2) {
    parser(argc, argv, &options, e_flags);

    if (options.e == 0 && options.f == 0) {
      strcat(e_flags, argv[optind]);
    } else {
      e_flags[strlen(e_flags) - 1] = '\0';
    }

    if (options.e == 0 && options.f == 0) {
      if (optind + 1 != argc - 1) many_files = 1;
    } else {
      if (optind + options.e + options.f != argc - 1) many_files = 1;
    }
    int if_e_and_f_zero = 0;
    if (options.e == 0 && options.f == 0) {
      if_e_and_f_zero = 1;
    }
    int optind_for_o = optind;
    for (; optind + options.e + options.f + if_e_and_f_zero < argc; optind++) {
      input2 =
          fopen(argv[optind + options.e + options.f + if_e_and_f_zero], "r");
      flag_for_l = 0;
      output(argv, &options, &str_count, &flag_for_l, e_flags, many_files,
             optind_for_o);

      if (options.c == 1 && options.l == 0) {
        if (many_files == 1) {
          if (files_count == 0) {
            if (input2) {
              printf("%s:%d\n",
                     argv[optind + options.e + options.f + if_e_and_f_zero],
                     str_count);
            }
            files_count++;
            str_count = 0;
          } else {
            if (input2) {
              printf("%s:%d\n",
                     argv[optind + options.e + options.f + if_e_and_f_zero],
                     str_count);
            }
            files_count++;
            str_count = 0;
          }
        } else {
          if (input2) {
            printf("%d\n", str_count);
          }
        }

      } else if (options.c == 0 && options.l == 1) {
        if (flag_for_l == 1) {
          if (files_count == 0) {
            printf("%s\n",
                   argv[optind + options.e + options.f + if_e_and_f_zero]);
            files_count++;
          } else {
            printf("%s\n",
                   argv[optind + options.e + options.f + if_e_and_f_zero]);
            files_count++;
          }
        }
      } else if (options.c == 1 && options.l == 1) {
        if (flag_for_l == 1) {
          if (many_files == 1) {
            if (files_count == 1) {
              if (input2) {
                printf("%s:1\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
                printf("%s\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
              }
              files_count++;
            } else {
              if (input2) {
                printf("%s:1\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
                printf("%s\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
              }
              files_count++;
            }

          } else {
            printf("1\n");
            if (input2) {
              printf("%s\n",
                     argv[optind + options.e + options.f + if_e_and_f_zero]);
            }
          }

        } else {
          if (many_files == 1) {
            if (files_count == 1) {
              if (input2) {
                printf("%s:0\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
              }
              files_count++;
            } else {
              if (input2) {
                printf("%s:0\n",
                       argv[optind + options.e + options.f + if_e_and_f_zero]);
              }
              files_count++;
            }

          } else {
            if (input2) {
              printf("0\n");
            }
          }
        }
      }
      fclose(input2);
    }
  } else {
    fprintf(stderr, "s21_grep: error\n");
  }
  fclose(input2);
  return 0;
}

void parser(int argc, char *argv[], opt *options, char *e_flags) {
  int opt;
  int opt_index;
  const struct option long_options[] = {{"regexp", 0, 0, 'e'},
                                        {"ignore-case", 0, 0, 'i'},
                                        {"invert-match", 0, 0, 'v'},
                                        {"count", 0, 0, 'c'},
                                        {"files-with-matches", 0, 0, 'l'},
                                        {"line-number", 0, 0, 'n'},
                                        {"no-filename", 0, 0, 'h'},
                                        {"no-messages", 0, 0, 's'},
                                        {"file", 0, 0, 'f'},
                                        {"only-matching", 0, 0, 'o'},
                                        {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "eivclnhsfo", long_options,
                            &opt_index)) != -1) {
    switch (opt) {
      case 'e':

        strcat(e_flags, argv[optind]);

        strcat(e_flags, "|");
        options->e++;

        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f++;
        char lineBuffer[513];

        FILE *fp1;

        fp1 = fopen(argv[optind], "r");

        if (fp1) {
          while (fgets(lineBuffer, 512, fp1)) {
            if (lineBuffer[0] != '\0') {
              if (lineBuffer[strlen(lineBuffer) - 1] == '\n' &&
                  strlen(lineBuffer) > 1) {
                lineBuffer[strlen(lineBuffer) - 1] = '|';
              }

              strcat(e_flags, lineBuffer);
              if (strlen(lineBuffer) == 1 && lineBuffer[0] == '\n') {
                strcat(e_flags, "|");
              }
            }
          }
          if (e_flags[strlen(e_flags) - 1] != '|') {
            strcat(e_flags, "|");
          }

        } else {
          fprintf(stderr, "s21_grep: error\n");
        }
        fclose(fp1);

        break;
      case 'o':
        options->o = 1;
        break;
      default:
        printf("usage eivclnhsfo");
        exit(-1);
        break;
    }
  }
}

void output(char *argv[], opt *options, int *str_count, int *flag_for_l,
            char *e_flags, int many_files, int optind_for_o) {
  int count_of_str = 0, BUFFER_LENGTH = 1024;
  size_t count1_o = 0, count2_o = 0, num_o;
  char lineBuffer[BUFFER_LENGTH + 1];
  regex_t preg;
  regmatch_t part_str[1];
  if (options->i == 1) {
    regcomp(&preg, e_flags, REG_ICASE | REG_EXTENDED);
  } else {
    regcomp(&preg, e_flags, REG_EXTENDED);
  }
  FILE *fp;
  if (options->e == 0 && options->f == 0) {
    fp = fopen(argv[optind + 1], "r");
  } else {
    fp = fopen(argv[optind + options->e + options->f], "r");
  }
  if (fp != NULL) {
    while (fgets(lineBuffer, BUFFER_LENGTH, fp)) {
      ++count_of_str;
      if (options->v == 0) {
        if (regexec(&preg, lineBuffer, 1, part_str, 0) == 0) {
          if (options->c == 0 && options->l == 0) {
            if (many_files == 1 && options->h != 1) {
              if (options->e == 0 && options->f == 0) {
                printf("%s:", argv[optind + 1]);
              } else {
                printf("%s:", argv[optind + options->e + options->f]);
              }
            }
            if (options->n == 1) {
              printf("%d:", count_of_str);
            }
            if (options->o == 1) {
              if (strlen(argv[optind_for_o]) != 0) {
                char mas_o[strlen(argv[optind_for_o]) + 1],
                    mas_ol[strlen(argv[optind_for_o]) + 1];
                strcpy(mas_o, argv[optind_for_o]);
                strcpy(mas_ol, argv[optind_for_o]);
                count1_o = 0, count2_o = 0, num_o = 0;
                while (count1_o < strlen(lineBuffer)) {
                  while (count2_o < strlen(argv[optind_for_o])) {
                    if (options->i == 0) {
                      if (lineBuffer[count1_o] == mas_o[count2_o]) {
                        num_o++;
                        count2_o++;
                        count1_o++;
                      } else {
                        count1_o++;
                        break;
                      }
                    } else {
                      if (lineBuffer[count1_o] == mas_o[count2_o] ||
                          (int)lineBuffer[count1_o] + 32 ==
                              (int)mas_o[count2_o] ||
                          (int)lineBuffer[count1_o] - 32 ==
                              (int)mas_o[count2_o]) {
                        mas_ol[num_o] = lineBuffer[count1_o];
                        num_o++, count2_o++, count1_o++;
                      } else {
                        strcpy(mas_o, argv[optind_for_o]);
                        count1_o++;
                        break;
                      }
                    }
                  }
                  if (num_o == strlen(argv[optind_for_o])) {
                    if (options->i == 0) {
                      printf("%s\n", mas_o);
                    } else {
                      printf("%s\n", mas_ol);
                    }
                  }
                  count2_o = 0;
                  num_o = 0;
                }
              }
            } else {
              printf("%s", lineBuffer);
              if (lineBuffer[strlen(lineBuffer) - 1] != '\n') {
                printf("\n");
              }
            }
          } else {
            *(str_count) += 1;
            *(flag_for_l) = 1;
          }
        }
      } else {
        if (regexec(&preg, lineBuffer, 0, part_str, 0) != 0) {
          if (options->c == 0 && options->l == 0) {
            if (many_files == 1 && options->h != 1) {
              if (options->e == 0 && options->f == 0) {
                printf("%s:", argv[optind + 1]);
              } else {
                printf("%s:", argv[optind + options->e + options->f]);
              }
            }
            if (options->n == 1) {
              printf("%d:", count_of_str);
            }
            printf("%s", lineBuffer);
            if (lineBuffer[strlen(lineBuffer) - 1] != '\n') {
              printf("\n");
            }
          } else {
            *(str_count) += 1;
            *(flag_for_l) = 1;
          }
        }
      }
    }
    fclose(fp);

  } else if (fp == NULL && options->s != 1) {
    fprintf(stderr, "s21_grep: error\n");
    fclose(fp);
  }
  regfree(&preg);
}
