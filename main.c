
/******************************************************************************\
*                                                                              *
* will be developed separately on local disk, to be migrated to a dll function *
*                                                                              *
\******************************************************************************/

#include "version.h"

#define VER_A                       "0.0.12-A20"
// save!


// developed in another project...
#define USE_I_O_FILES_DIRECT // put this before the #include "interpretMacros.h" always!!!
#include "interpretMacros.h"


/** Main function: make more readable by doing separeate functions from different pieces of code
 *  Yes, this is a challenge, I know! */
int main(int argc, char** argv)
{
    FILE *flog;
    FILE * fp;
    FILE *fout;

    flog = fopen("debug_log.sql", "w");

    SHOW_VERSION(VER_A)
    if(argc>1)
    {
        fp = fopen(argv[1], "r");
        if(argc>2)
        {
            fout = fopen(argv[2], "w");
        }
        else
        {
            char* szFNm = (char*)malloc(0x100);
            memset(szFNm, 0, 0x100);
            memcpy(szFNm, argv[2], strrchr(argv[2], '.')-argv[2]);
            strcat(szFNm, "_out.sql");
            fout = fopen(szFNm, "w");
            free(szFNm);
        }
    }
    else
    {
        fp = fopen(SRC_SQL_SAMPLE_FILE, "r");
        fout = fopen(DST_SQL_OUTPUT_FILE, "w");
    }
    if (fp == NULL)
    {
        printf("file not found: \"%s\"\n", argv[1]);
        return 32;
    }
    if (fout == NULL)
    {
        printf("file not found: \"%s\"\n", argv[2]);
        return 34;
    }
    
    // proceed function
#ifdef USE_I_O_FILES_DIRECT    
    proceed(fp, 0, fout, 0, flog, 0);
#else
    fclose(fp);
    char* buf = 0;
    long bufLen;
    fp = fopen (argv[1], "rb");
    if(fp)
    {
        fseek (f, 0, SEEK_END);
        len = ftell (f);
        fseek (f, 0, SEEK_SET);
        buf = malloc (len);
        if(buf)
        {
            fread (buf, 1, bufLen, f);
        }
        fclose (f);
    }
    
#endif
    // close file(s) and free resources:
    fclose(fout);
    fclose(fp);
    fclose(flog);

    printf("Game Over\n");
    return 0;
}

/******************************************************************************\
*                                                                              *
*                                                                              *
*                                                                              *
\******************************************************************************/

