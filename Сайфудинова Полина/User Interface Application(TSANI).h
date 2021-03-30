/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2021. All Rights Reserved.          */
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
#define  PANEL_Input1                     2       /* callback function: Input1 */
#define  PANEL_Input0                     3       /* callback function: Input0 */
#define  PANEL_Output                     4       /* callback function: Output */
#define  PANEL_Timer1                     5       /* callback function: Timer1 */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Input0(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Input1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Output(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Timer1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
