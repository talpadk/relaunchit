#ifndef UNICODE_TRIM_H
#define UNICODE_TRIM_H

#include <stdint.h>

/** 
 * Trims (removes white spaces left and right) the given string by returning it as an offset and inserting a null into it.
 * 
 * @param string the string to trim/modify
 * 
 * @return the trim
 */
uint32_t *unicode_trim(uint32_t *string);

char *utf8_trim(char *string);


#endif //UNICODE_TRIM_H 
