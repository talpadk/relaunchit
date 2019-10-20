#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>


#define MAX_CONF_STR_LEN (100)

struct LauncherItem {
  char title[MAX_CONF_STR_LEN];
  char command[MAX_CONF_STR_LEN];
  struct LauncherItem *nextLauncherItem;
};

typedef struct {
  char fontPath[MAX_CONF_STR_LEN];
  uint32_t fontSize;
  uint16_t itemHeight;
  uint16_t baselineOffset;
  struct LauncherItem *firstLauncherItem;
} Settings;

extern Settings settings_;

uint8_t settings_init();

#endif //SETTINGS_H 
