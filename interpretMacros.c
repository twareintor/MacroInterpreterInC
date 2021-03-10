
#include "interpretMacros.h"

//////////// helper functions (to be moved in separate modules)


void testPrintVariables(char pszVars[][0x02][LINE_BUF_LEN_FIXED], const size_t varCount, const char* szFileName)
{
    // test print variables:
    int i;
    const char* szVarDatOutputFile = szFileName;
    FILE* fvars;
    fvars = fopen(szVarDatOutputFile, "w");
    for(i=0; i<varCount; i++)
    {
        if(pszVars[i][0])
        {
            if(strlen(pszVars[i][0]))
            {
                if('@'==*pszVars[i][0] || 1) // only the not removed varialbes
                {
                    fprintf(fvars, "0x%08x|%s|%s|\n", i, pszVars[i][0], pszVars[i][1]);
                }
            }
        }
    }
    fclose(fvars);
}    


int ignoreThis(int* ignoreLine, int nestLev)
{
    int ret = 0; // ignore this line: when all lines, of all nested levels are to be ignored...
    int nit; // nest iterator
    for(nit = nestLev; nit>0; nit--)
    {
        ret+=ignoreLine[nit]; // if something 
    }
    return ret>0; // to be pure 1 instead of "more than zero"
}

/* it was a tryout to refactor the code in order to work with buffers instead of files */
void proceed(FILE* fp, const size_t fplen, FILE* fout, const size_t foutlen, FILE* flog, const size_t floglen)
{
    char pszVars[VARIABLE_MAX_COUNT][0x02][LINE_BUF_LEN_FIXED]; // max. 0x400 variables {name, value} all string
    int i;
    for(i=0; i<VARIABLE_MAX_COUNT; i++)
    {
        memset(pszVars[i][0], 0, LINE_BUF_LEN_FIXED);
        memset(pszVars[i][1], 0, LINE_BUF_LEN_FIXED);
    }
    char * line  = NULL;
    int ilin = 0; // line number in the source file, NOT ONLY for debug for "#GOTO" directive too // #DEBUG209348 // for DEBUG209348// 
    i = 0; // will be reused(?) yes, is reused(!)
    size_t len = 0;
    unsigned ignoreLine[0x100] = {0}; // 0 and the next line will be _not_ignored_ or 1 and will be ignored
    unsigned nestLev = 0; // ... at its corresponding level
    while ((getline(&line, &len, fp)) != -1) // till to the end
    {
        // BEGIN OF PREPARATIVE TASKS 
        ilin++; // iterator in line: byte-by-byte iterator inside the line
        // first and first: replace the already load variables in the line we'we read
        // // // printf("%s,  %d, %d\n", line, len, strlen(line));
        size_t linelen = len;
        char* szLine = (char*)malloc(LINE_BUF_LEN_FIXED); // make a copy of the current line
        memset(szLine, 0, LINE_BUF_LEN_FIXED);
        memcpy(szLine, line, linelen); // in replacing variables with values we have reallocation:
        printf("%s", szLine); // ... because in this respect, it must be allocated with malloc()
        int j;
        char* line0 = line;
        char* linit = szLine; // remember that szLine is modified, line not!
        // ~~END OF PREPARATIVE TASKS 
        // // new function: // interpretLine(linit, len, pszVars, sizeof(pszVars), fin, fout, flog);
        // process now: 
        while(' '==*linit || '\t'==*linit) linit++;
        if('#'==linit[0]) // is a directive
        {
            // first process the #IFDEF|#IFNDEF\#THEN\#ELSE\#ENDIF
            if(!memcmp("#IF", linit, 2))
            {
                // BEGIN OF: PROCESS DIRECTIVES TASKS(#IF#THEN#ELSE) ////////////////////////////////////////////////////////
                int isDef = !memcmp("DEF", linit+3, 3);
                int isNDef = !memcmp("NDEF", linit+3, 4);
                if(isDef || isNDef) // one of them must be
                {
                    nestLev++; // next nested level;
                    ignoreLine[nestLev] = isDef; // starting by ignoring till we found that is defined: 1 if def, 0 if ndef
                    linit+=strlen("#IFDEF"); linit++;
                    while(' '==*linit || '\t'==*linit) linit++;
                    if('@'==*linit) // is a convention compliant variable
                    {
                        char* varName = linit;
                        while(' '!=*linit && '\t'!=*linit && '\n'!=*linit) linit++;
                        // we reached the beginning of the value or the end of the linit
                        int foundInList = 0; // becmoes 1 if the varName is defined
                        int j;
                        for(j=0; j<i; j++)
                        {
                            if(!strlen(pszVars[j][0])) break; 
                            if(linit-varName==strlen(pszVars[j][0]) && !memcmp(pszVars[j][0], varName, linit-varName))
                            {
                                foundInList = 1;
                                break;
                            }
                        }
                        // if found, don't ignore...
                        if(foundInList) ignoreLine[nestLev] = !isDef; // 1 if def, 1 if ndef W!!!!!!!!!!!!!!!!
                    }
                }
                // ~~END OF: PROCESS DIRECTIVES TASKS(#IF#THEN#ELSE) ////////////////////////////////////////////////////////
            }
            else if(!memcmp("#ELSE", linit, 5))
            {
                // BEGIN OF: PROCESS DIRECTIVES TASKS(#ELSE) /////////////////////////////////////////////////////
                ignoreLine[nestLev] = !ignoreLine[nestLev];
                // ~~END OF: PROCESS DIRECTIVES TASKS(#ELSE) /////////////////////////////////////////////////////                
            }
            else if(!memcmp("#ENDIF", linit, 6))
            {
                // BEGIN OF: PROCESS DIRECTIVES TASKS(#ENDIF) /////////////////////////////////////////////////////
                ignoreLine[nestLev] = 0; // just reset it, to be (re)used for next met nested #IF#THEN#ELSE
                nestLev--;
                // ~~END OF: PROCESS DIRECTIVES TASKS(#ENDIF) /////////////////////////////////////////////////////
            }
            else // is a variable definition
            {
                // BEGIN OF: PROCESS MACRO DEFINITIONS TASKS() ////////////////////////////////////////////////////
                if(!ignoreThis(ignoreLine, nestLev)) // if not to be ignored...
                {
                    // treat variable definitions and replacements, if the line is not to be ignored!!!!!!!
                    if(!memcmp("#DEFINE", linit, 7))
                    {
                        // BEGIN OF: PROCESS MACRO DEFINITIONS TASKS(#DEFINE) /////////////////////////////////////
                        linit+=strlen("#DEFINE"); linit++;
                        while(' '==*linit || '\t'==*linit) linit++;
                        if('@'==*linit) // is a convention compliant variable
                        {
                            char* varName = linit;
                            while(' '!=*linit && '\t'!=*linit && '\n'!=*linit) linit++;
                            // we reached the beginning of the value or the end of the linit
                            int redefined = 0;
                            /**** BEGIN OF: now check if the variable is defined already ... ****/
                            int j;
                            for(j = 0; j<i /* the current position of set variables */; j++)
                            {
                                if(linit-varName==strlen(pszVars[j][0]) && 0==strncmp(pszVars[j][0], varName, linit-varName)) 
                                { // found! NO REDEFINTION ALLOWED
                                    if(flog) fprintf(flog, "/* *** REDEFINTION OF: \"%s\" \t !!!ERROR!!!! **** */\n", pszVars[j][0]);
                                    redefined = 1; // we can spare an integer, checking (j<i-1)
                                    break;
                                }
                            }
                            /**** BEGIN OF: now check if the variable is defined already ... ****/
                            if(!redefined) // then define it // or "if(j<i-1){}"
                            {
                                memcpy(pszVars[i][0], varName, linit-varName); // variable name copied in buffer...
                                if(linit) // if still characters in the linit (not its end!)
                                    while(' '==*linit || '\t'==*linit) linit++; // advance through blanks etc.
                                if(linit) // if still characters, then we reached the variable value...
                                {
                                    strcpy(pszVars[i][1], linit); // load variable value in variable table
                                    char* szTrim = pszVars[i][1]; // and trim it...
                                    while(*++szTrim); --szTrim; // .. trim to the end
                                    while(' '==*szTrim || '\t'==*szTrim || '\n'==*szTrim || '\r'==*szTrim) --szTrim; 
                                    ++szTrim; *szTrim = 0; // this trimms all blanks, tabs etc at the end of the value
                                }
                                // then reset to the beginning again (required??)
                                i++; // increment the index for the next variable WARNING!!! i is reused!!!
                            }
                        }
                        // ~~END OF: PROCESS MACRO DEFINITIONS TASKS(#DEFINE) /////////////////////////////////////
                    }
                    else if(!memcmp("#UNDEF", linit, 6))
                    {
                        // is a variable to be removed from the table defined, try:
                        // BEGIN OF: PROCESS MACRO DEFINITIONS TASKS(#UNDEF) //////////////////////////////////////
                        linit+=strlen("#UNDEF"); linit++;
                        while(' '==*linit || '\t'==*linit) linit++;
                        if('@'==*linit) // is a convention compliant variable
                        { // search for it in the table and replace its first char with whatever
                            char* varName = linit;
                            while(' '!=*linit && '\t'!=*linit && '\n'!=*linit) linit++;
                            // we reached the beginning of the value or the end of the linit
                            int j;
                            for(j = 0; j<i /* the current position of set variables */; j++)
                            {
                                if(linit-varName==strlen(pszVars[j][0]) && 0==strncmp(pszVars[j][0], varName, linit-varName)) 
                                {   // found! remove!
                                    if(flog) fprintf(flog, "/* *** REMOVING \"%s\" ... **** */\n", pszVars[j][0]);
                                    pszVars[j][0][0] = '#'; // replace as making it a comment
                                }
                            }
                        }
                        // ~~END OF: PROCESS MACRO DEFINITIONS TASKS(#UNDEF) //////////////////////////////////////
                    }
                    else
                    {
                        // do nothing, is a macro-comment or unrecognized macro-expression 
                    }
                }
                // ~~END OF: PROCESS MACRO DEFINITIONS TASKS() ////////////////////////////////////////////////////
            }
            fprintf(fout, "\n"); // print a new line anyway, in order to preserve the number of lines
        }
        else // is not a directive, is to be used in the final command string:
        {
            if(!ignoreThis(ignoreLine, nestLev)) // if not to be ignored...
            {
                
                /**** BEGIN OF REPLACING VARIABLE NAMES WITH THEIR VALUES */
                for(j=0; j<i; j++)
                {
                    replaceWholeWord(szLine, strlen(szLine), pszVars[j][0], pszVars[j][1], '\'');
                }
                /**** ~~END OF REPLACING VARIABLE NAMES WITH THEIR VALUES */
                // then print in the output file:                
                fprintf(fout, "%s", szLine); // what is not directive... write in output file
            }
            else
            {
                // fprintf(fout, "\n"); // print a new line anyway -- no need to do it
            }
        }
        // logging results:
        if(flog) if(ignoreThis(ignoreLine, nestLev)) fprintf(flog, "/* %s */", szLine); else fprintf(flog, "%s", szLine);
        // free resources
        if(szLine) free(szLine);            
        line = line0; // reset the line to its original beginning anyway // doesn't needed anymore!
    }
    if (line) free(line); // why? it was assigned by us? // See documentation for files in C
    testPrintVariables(pszVars, VARIABLE_MAX_COUNT, VAR_DAT_OUTPUT_FILE);
}
