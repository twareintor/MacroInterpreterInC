
#include "textUtil.h"


const int isCharInsideWord(const char c)
{
    return c>='a'&& c<='z' || c>='A'&& c<='Z' || c>='0'&&c<='9' || '_'==c;
}


char* replaceWholeWord(char* szText, size_t len, const char* szSrc, const char* szRepl, const char quote)
{
    // 
    if(!strlen(szSrc)) return szText;
    char* szFound = 0; // 
    szFound = strstr(szText, szSrc);
    
    if(!szFound || szFound>=(szText+len)) return szText;
    size_t slen = strlen(szSrc);
    size_t rlen = strlen(szRepl);
    char* szE = szFound+slen;
    if(szFound==firstAfterExpression(szText, len, szFound, quote))    
    {
        if(!isCharInsideWord(*szE)) // only full word: no valid characters after...
        {
            if(szFound>szText && !isCharInsideWord(*(szFound-1))) // nor before!
            {
                // printf("'%c'>%.*s<'%c'\n", *szFound, szE-szFound, szFound, *szE); // it was for debug
                if(slen!=rlen) // make room or shrink, if needed...
                {
                    memmove(szFound+rlen, szFound+slen, strchr(szFound, '\0')-(szFound+slen-1)); 
                }
                memcpy(szFound, szRepl, rlen); // then replace
                szFound+=rlen;
            }
        }
    }
    else
    {
        szFound = firstAfterExpression(szText, len, szFound, quote);
    }
    szFound++; // advance one then recurse to find everything!
    len = strlen(szFound);
    replaceWholeWord(szFound, len, szSrc, szRepl, quote);
    return szText;
}


char* firstAfterExpression(char* szText, size_t len, char* szSrc, const char quote)
{
    if(szSrc>szText && szSrc<szText+len) // if no szSrc (assuming szText is not null)
    {
        char* szQuote = 0;
        szQuote = strchr(szText, quote); // found first quote, if not...
        if(!szQuote) return szSrc; // return! compared with the previous version ####
        if(szSrc<szQuote) return szSrc; // if quote found after searching, return
        char* szNextQuote = 0; 
        szNextQuote = strchr(szQuote+1, quote); // else check the next one:
        if(szNextQuote) // if another quote...
        {
            if(szSrc<szNextQuote) /// ... and if inside of the pair of quotes, then
            {
                return ++szNextQuote; // simply return the first character after the expression.
                // // // mission complete!
            }
            // when not, recurse and search after the end of the expression for new occurrences:
            ++szNextQuote;
            return firstAfterExpression(szNextQuote, len-(szNextQuote-szText), szSrc, quote);
        }
        // else: the pair of quotes doesn't match and is after: return the end of the string ####
        return szText+len; 
    }
    return szSrc;
}
