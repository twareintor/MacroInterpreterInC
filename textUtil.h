
/*************************************************************************************** */
#ifndef TEXTUTIL_H_912836192891729034812346518902
#define TEXTUTIL_H_912836192891729034812346518902

#include <string.h>

/******** TEXTUTIL_H ********/

/** Function taking a text szText and its lenght len, searching for a sequence of characters szSrc
 * and replaces with szRepl only and only if szSrc is a "word" after the following rules:
 *  - the character before and after the match of szSrc in szText returns 0 to isCharInsideWord(), 
 *  - one of these ends match the aforementioned condition and the other end is begin(szText) or beforeend(szText)
 * It doesn't matter what sequence of characters is the match of szSrc in szText, the aforementioned conditions
 * must be fulfiled: eg: for szText like "the $4%CK? brown fox" and szSrc being "$4%CK?" is a match because
 * the character before and after the sequence "$4%CK?" are just blanks
 * This function is useful only for replacing macro-like constants in a text-code. If you don't understand the
 * utility of this function you don't need it.
 * //Permanent link: https://rextester.com/WBOY57907 (see it)
 */
char* replaceWholeWord(char* szText, size_t len, const char* szSrc, const char* szRepl, const char quote);

/** Function returning 1 (true) if c is alphanumeric or _
 *  return c>='a'&& c<='z' || c>='A'&& c<='Z' || c>='0'&&c<='9' || '_'==c; - alternative to C isalnum() function
 */
const int isCharInsideWord(char c);

/** First Text After Expression - Function in C
 * C function taking as arguments:
 * a string of characters (szText) where a searching string (szSrc) _is_inside_ szText
 * the lenght of the szText to be taken into consideration (len)
 * the szSrc explained above
 * a character used as quote (e.g., '"/|@í whatever)
 * Return value: the szSrc, if the szSrc happens to be outside of a pair of (quote)s
 * the first character after the closing (quote) of a pair, if szSrc happens to be betwenn them
 * the last character of szText, if szSrc is after an opening quote but the closing one is absent
 * every other situation leads to undefined result
 *
 * NOTE: if you don't understand what this function is good for, you don't need it
 * DISCLAIMER: not fully tested nor implemented; use on your own responsibility
 * See testing and usage: https://rextester.com/JLUP24994
 */
char* firstAfterExpression(char* szText, size_t len, char* szSrc, const char quote);


#endif//TEXTUTIL_H_912836192891729034812346518902
/*************************************************************************************** */


