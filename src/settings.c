#include "settings.h"

#include "utf8_decode.h"
#include "unicode_trim.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Settings settings_;

typedef enum {
  SETTINGS_SECTION_NONE,
  SETTINGS_SECTION_GLOBAL,
  SETTINGS_SECTION_ITEM,
} SectionType;

static FILE *settingsFile_;
static uint32_t lineNumber_;
static SectionType currentSection_;
static struct LauncherItem *currentItem_;

#define EXTRA_LENGTH (20)

uint8_t beginsWith(const char *string, const char *sevenBitMatch){
  while (*sevenBitMatch != 0){
    if (*string != *sevenBitMatch){
      return 0; }
    sevenBitMatch++;
    string++;
  }
  return 1;
}


static SectionType getSectionType(const char *line){
  if      (beginsWith(line, "[global]")) { return SETTINGS_SECTION_GLOBAL; }
  else if (beginsWith(line, "[item]"))   { return SETTINGS_SECTION_ITEM;   }
  else                                   { return SETTINGS_SECTION_NONE;   }
}

static uint8_t creatNewItem(void){
  struct LauncherItem *item = (struct LauncherItem *)malloc(sizeof(struct LauncherItem));
  if (item==0) { return 0; }

  strcpy(item->title, "??? Title ???");
  item->command[0] = 0;
  item->nextLauncherItem = 0;

  if (currentItem_ == 0){ settings_.firstLauncherItem = item; }
  else               { currentItem_->nextLauncherItem = item; }

  currentItem_ = item;
  
  return 1;
}

void storeStringValue(char *destination, char *sourceLine){
  //Find =
  while (*sourceLine != 0 && *sourceLine != (uint32_t)'='){
    sourceLine++;
  }
  if (*sourceLine == 0) { return; } //not found
  sourceLine++;

  sourceLine = utf8_trim(sourceLine);
  uint32_t bytesLeft = MAX_CONF_STR_LEN-1;
  while (*sourceLine != 0 && bytesLeft>0){
    *destination++ = *sourceLine++;
  }
  *destination = 0;
}


static uint8_t readSettingsLine(void){
  char buffer[MAX_CONF_STR_LEN+EXTRA_LENGTH];
  
  char *result = fgets(buffer, MAX_CONF_STR_LEN+EXTRA_LENGTH, settingsFile_);
  if (result == 0) { return 0; } //we are done

  lineNumber_++;
  // @todo strip
  SectionType newSectionType = getSectionType(buffer);
  if (newSectionType == SETTINGS_SECTION_ITEM){
    printf("New Item\n");
    creatNewItem();
  }
  if (newSectionType != SETTINGS_SECTION_NONE) {
    currentSection_ = newSectionType;
  }
  switch (currentSection_){
  default:
    break;
  case SETTINGS_SECTION_ITEM:
    if (beginsWith(buffer, "title")) { storeStringValue(currentItem_->title, buffer); }
  }
  return 1;  
}

uint8_t settings_init(){
  //Configure the default values
  strncpy(settings_.fontPath, "/usr/share/fonts/ttf/noto/NotoSans-Bold.ttf", MAX_CONF_STR_LEN);
  settings_.fontPath[MAX_CONF_STR_LEN-1] = 0;
  settings_.fontSize = 3000;
  settings_.itemHeight = 200;
  settings_.baselineOffset = (settings_.itemHeight*3)/4;
  settings_.firstLauncherItem = 0;

 
  lineNumber_ = 0;
  currentItem_ = 0;
  currentSection_ = SETTINGS_SECTION_NONE;
  const char *filename = "/home/root/.relaunchit.ini";
  settingsFile_ = 0;
  settingsFile_ = fopen(filename, "r");
  if (settingsFile_ == 0){
    printf("Unable to '%s' for reading\n", filename);
    return 0;
  }

  while (readSettingsLine()){ }
  
  return 1;
}
