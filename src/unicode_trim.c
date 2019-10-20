#include "unicode_trim.h"


uint8_t isWhiteSpace(uint32_t character){
  switch (character){
  case 32: //space
  case 9: //tab
  case 10: //new line
  case 13: //carriage return
    return 1;
  default:
    return 0;
  }
  
}

char *utf8_trim(char *string){
  char *result = string;
  //Skip left white spaces
  while (isWhiteSpace(((uint8_t)*string))){ string++; } 

  //Find end of line
  char *lineEnd=result;
  while(*lineEnd != 0) { lineEnd++; }
  
  //Find the last non white space
  lineEnd--;
  while (lineEnd >=result && isWhiteSpace(((uint8_t)*lineEnd))){
    lineEnd--;
  }
  lineEnd[1] = 0;
  
  return result;
}


uint32_t *unicode_trim(uint32_t *string){
  uint32_t *result = string;
  //Skip left white spaces
  while (isWhiteSpace(*string)){ string++; } 

  //Find end of line
  uint32_t *lineEnd=result;
  while(*lineEnd != 0) { lineEnd++; }
  
  //Find the last non white space
  lineEnd--;
  while (lineEnd >=result && isWhiteSpace(*lineEnd)){
    lineEnd--;
  }
  lineEnd[1] = 0;
  
  return result;
}
