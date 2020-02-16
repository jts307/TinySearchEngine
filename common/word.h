/* 
 * word.h - header file for 'word' module
 * 
 * The word module has functions that handle processes that involves char* types,
 * specifically the modification of them.
 * 
 * Jacob Werzinsky, Winter 2020
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>

/**************** global types ****************/
/* none */

/**************** functions ****************/

/* Changes all alphabetic characters in a string to lower case.
 * parameters:
 *   word - the string to be edited. Must be mutable.
 * returns:
 *   0 - on sucess
 *   1 - on failure caused by null word
 */
int normalizeWord(char *word);

#endif // __WORD_H

