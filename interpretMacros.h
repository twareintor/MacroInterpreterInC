
#ifndef _INTERPRET_MACROS_H_918237498127398411829374812738541235127358123412
#define _INTERPRET_MACROS_H_918237498127398411829374812738541235127358123412

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textUtil.h"
#include "processLine.h"


#define LINE_BUF_LEN_FIXED 0x400
#define VARIABLE_MAX_COUNT 0x400

#define TODO    "\nTODO: split the main() function in small functions -- \n"

// #define SRC_SQL_SAMPLE_FILE "sample.sql" // "sample_old.sql"
#define SRC_SQL_SAMPLE_FILE "sample_old.sql" // "sample.sql"
#define DST_SQL_OUTPUT_FILE "output.sql"
#define VAR_DAT_OUTPUT_FILE "variables.dat"

/** Helper function which returns logical sum of the values of the array ignoreLine[nestLev]  */
int ignoreThis(int* ignoreLine, int nestLev);

/** the procedure that makes all the work: use with care */
void proceed(FILE* fp, const size_t fplen, FILE* fout, const size_t foutlen, FILE* flog, const size_t floglen);

/** the procedure that prints all variables in a file for testing purpose */
void testPrintVariables(char pszVars[][0x02][LINE_BUF_LEN_FIXED], const size_t varCount, const char*szFileName);

#endif//_INTERPRET_MACROS_H_918237498127398411829374812738541235127358123412

