#ifndef MAKISE_CONFIG_H
#define MAKISE_CONFIG_H
#include <stdio.h>
#include <stdint.h>

#define MAKISE_DEBUG_OUTPUT printf

//comment unused displays
//#define MAKISE_SDL2_USE 1
//#define MAKISE_ILI9340_USE 1

//Use if you want your own color type
#define MAKISE_COLOR_CUSTOM_TYPE
typedef struct {
    float r;
    float g;
    float b;
    uint8_t a : 1;
} MColor;
#define MC_Transparent (MColor){0,0,0,1}
#define MC_IS_Transparent(c) ((c.a) == 1)

#define MC_Black (MColor){0,0,0,0}
#define MC_White (MColor){1,1,1,0}
#define MC_Gray  (MColor){0.5,0.5,0.5,0}
#define MC_Green (MColor){0,1,0,0}

//#define MAKISE_PRIMITIVES_DRAWER_DEFAULT 
//#define MAKISEGUI_DRIVER_DEPTH 16
//#define MAKISEGUI_BUFFER_DEPTH 16
//#define MAKISEGUI_BUFFER_DEPTHMASK 0b11111111111111111

//#define MAKISE_BUF_H 64
//#define MAKISE_BUF_W 128

//Control section
//len of buffer with input events
#define MAKISE_GUI_INPUT_BUFFER_LEN 10
//use x-y pointing control devices. like mouse or touchscreen
#define MAKISE_GUI_INPUT_POINTER_ENABLE 1


//Select used elements
#define MAKISE_E_SLIST             1
#define MAKISE_E_BUTTONS           1
#define MAKISE_E_CANVAS            1 
#define MAKISE_E_LABLE             1
#define MAKISE_E_PROGRESS_BAR      0
#define MAKISE_E_SLIDER            0
#define MAKISE_E_TABS              0
#define MAKISE_E_TEXT_FIELD        1
#define MAKISE_E_TOGGLE            1
#define MAKISE_E_FSVIEWER          0//MAKISE_E_FSVIEWER_STDIO

#define MAKISE_UNICODE             1
#define MAKISE_DISPLAY_INVERTED    0

#define MAKISE_MUTEX               0

#if MAKISE_MUTEX
/* #include "FreeRTOS.h" */
/* #include "semphr.h" */
/* #define MAKISE_MUTEX_t       xSemaphoreHandle //change for your OS */
/* #define MAKISE_MUTEX_TIMEOUT 1000 */

/* //implement that functions for your OS */

/* //create mutex object */
/* uint8_t m_mutex_create (MAKISE_MUTEX_t *sobj); */
/* //delete mutex */
/* uint8_t m_mutex_delete (MAKISE_MUTEX_t *sobj); */
/* //Request Grant to Access some object */
/* uint8_t m_mutex_request_grant (MAKISE_MUTEX_t *sobj); */
/* //Release Grant to Access the Volume */
/* uint8_t m_mutex_release_grant (MAKISE_MUTEX_t *sobj); */
#endif

// FONTS. Uncomment to use
/* #define MAKISE_FONTS_DEFAULT10X20 */
/* #define MAKISE_FONTS_DEFAULT8X13 */
/* #define MAKISE_FONTS_DEFAULT6X10 */
/* #define MAKISE_FONTS_DEFAULT5X7 */

/* Declare own fonts. Defined in the cairo-fonts.c.
It can be done anywhere. */
typedef struct _MakiseFont MakiseFont;
extern const MakiseFont F_Default5x7;
extern const MakiseFont F_Default6x10;
extern const MakiseFont F_Default8x13;
extern const MakiseFont F_Default10x20;    


#ifdef MAKISE_ILI9340_USE
#include "ili9340.h"
#endif

#endif
