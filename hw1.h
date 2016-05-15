#ifndef HW1_H
#define HW1_H

#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* USAGE STATEMENT */
#define USAGE(name) do {                                                                             \
        fprintf(stderr,                                                                              \
            "\n%s ./mstat [OPTION]\n\n"               												 \
            "./mstat -h                     Displays this help menu.\n\n"                            \
            "./mstat -i [-u]                Displays statistics about\n"                             \
            "                               instruction types.\n\n"                                  \
            "./mstat -r [-u]                Displays information about\n"                            \
            "                               the registers.\n\n"                                      \
            "./mstat -o [-u]                Displays number and\n"                                   \
            "                               percentages of opcodes used.\n\n"                        \
            "Optional flags:\n\n"																     \
            "-u                             Displays human readable headers\n"                       \
            "                               for the different outputs.\n\n"                          \
            ,(name));   																			 \
    } while(0);

/* PROTOTYPES */
int instr_func(void);
int registr_func(void);
int opcode_func(void);

#endif /* HW1.H */

