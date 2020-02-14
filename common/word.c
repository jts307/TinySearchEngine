/* 
 * word.c - 'word' module, contains operations that are performed on words, or more generally char* types.
 *
 * see word.h for more information.
 *
 * Jacob Werzinsky Winter 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see pageDir.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/* none */

/**************** functions **********************/

/**************** normalizeWord() ****************/
int normalizeWord(char *word)
{
  if (word != NULL) {
    // looping through word char by char
    for (int i=0; i < strlen(word); i++) {	  
     // changing each char to lower case if applicable
     word[i] = tolower(word[i]);
    }
    // success
    return 0;
  }
  // failure
  return 1;
}
