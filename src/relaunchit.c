///#exe
///#link fb
///#link refresh
///#link serde


#include "lib.h"
#include "utf8_decode.h"
#include <string.h>
#include <stdlib.h>

#include "remarkable_lut.h"
#include "remarkable_blit.h"
#include "remarkable_text_render.h"
#include "remarkable_multitouch.h"

#include "settings.h"

FT_Library  libFT_;
FT_Face     font_;


int main(void){
  remarkable_framebuffer *screen = remarkable_framebuffer_init("/dev/fb0");
  remarkable_lut16Init();
  remarkable_multitouch_init();
  
  remarkable_framebuffer_fill(screen, REMARKABLE_BRIGHTEST);
  remarkable_framebuffer_refresh(screen, UPDATE_MODE_FULL, WAVEFORM_MODE_GC16_FAST, 4096, EPDC_FLAG_USE_REMARKABLE_DITHER, 0, DRAWING_QUANT_BIT, 0, 0,0, 1872,1404);

  if (FT_Init_FreeType(&libFT_)){
    printf("Freetype lib init failed!\n");
    return 1;
  }
  
  if (settings_init()){
    if (FT_New_Face(libFT_, "/usr/share/fonts/ttf/noto/NotoSans-Bold.ttf", 0, &font_)){
      printf("Error while loading font");
      return 1;
    }

    if (FT_Set_Char_Size(font_, settings_.fontSize, 0, 226, 226)){
      printf("Failed to set font size");
      return 1;
    }

    uint8_t keepRunning = 1;
    while (keepRunning){ 
    
      uint8_t even = 1;
      int16_t y = 0;
      uint32_t itemCount = 0;
      struct LauncherItem *item = settings_.firstLauncherItem;
      remarkable_framebuffer_fill(screen, REMARKABLE_BRIGHTEST);
      while (item != 0){
	uint8_t colour = 15;
	if (even) { colour = 14; }
	remarkable_blitRect(screen, colour, 0,y, screen->vinfo.xres-1,y+settings_.itemHeight);
	
	remarkable_text_render_simple(screen, font_, item->title, 20, y+settings_.baselineOffset, 1, 0);
	
	y += settings_.itemHeight;
	item = item->nextLauncherItem;
	even ^= 1;
	itemCount++;
      }
      
      remarkable_framebuffer_refresh(screen, UPDATE_MODE_FULL, WAVEFORM_MODE_GC16_FAST, 4096, EPDC_FLAG_USE_REMARKABLE_DITHER, 0, DRAWING_QUANT_BIT, 0, 0,0, screen->vinfo.yres,screen->vinfo.xres);

      uint8_t waitingForClick = 1;
      uint32_t itemIndex = 0;
      MultitouchEvent touchEvent;
      while (waitingForClick){
	remarkable_multitouch_animate();
	while (remarkable_multitouch_getNextEvent(&touchEvent)){
	  //Look for a release above an item
	  if (touchEvent.eventType == MULTITOUCH_EVENT_RELEASE){
	    itemIndex = touchEvent.y / settings_.itemHeight;
	    if (itemIndex < itemCount) { waitingForClick = 0; }
	    printf("Item %d of %d clicked\n", itemIndex, itemCount);
	  }
	}
      }
      item = settings_.firstLauncherItem;
      while (itemIndex > 0 && item != 0){
	itemIndex--;
	item = item->nextLauncherItem;
      }
      if (item != 0){
	if (item->command[0] != 0){
	  system(item->command);
	}
      }
      printf("Command done\n");
      remarkable_multitouch_deinit(); //Clear events
      remarkable_multitouch_init();
    }
  }

  return 0;
}
