/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2008. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_NUMERIC_TEMP               2
#define  PANEL_BUTTON_SINGLE              3       /* callback function: BUTTON_SINGLE_function */
#define  PANEL_BUTTON_WORD                4       /* callback function: BUTTON_WORD_function */
#define  PANEL_STRING                     5
#define  PANEL_TEXTMSG_3                  6
#define  PANEL_TEXTMSG_2                  7
#define  PANEL_TEXTMSG                    8
#define  PANEL_TBUTTON_LED                9       /* callback function: TBUTTON_LED_function */
#define  PANEL_TBUTTON_TEMPERATURE        10      /* callback function: TBUTTON_TEMPERATURE_function */
#define  PANEL_LED7                       11      /* callback function: LEDButton */
#define  PANEL_LED6                       12      /* callback function: LEDButton */
#define  PANEL_LED5                       13      /* callback function: LEDButton */
#define  PANEL_LED4                       14      /* callback function: LEDButton */
#define  PANEL_LED3                       15      /* callback function: LEDButton */
#define  PANEL_LED2                       16      /* callback function: LEDButton */
#define  PANEL_LED1                       17      /* callback function: LEDButton */
#define  PANEL_LED0                       18      /* callback function: LEDButton */
#define  PANEL_TIMER_TEMPERATURE          19      /* callback function: TIMER_TEMPERATURE_function */
#define  PANEL_NUMERIC_DEBUG2             20
#define  PANEL_NUMERIC_DEBUG              21
#define  PANEL_TIMER_LED                  22      /* callback function: TIMER_LED_function */
#define  PANEL_STRIPCHART                 23


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BUTTON_SINGLE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BUTTON_WORD_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LEDButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TBUTTON_LED_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TBUTTON_TEMPERATURE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_LED_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_TEMPERATURE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
