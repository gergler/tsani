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
#define  PANEL_Vin2_code                  2
#define  PANEL_Vin1_code                  3
#define  PANEL_Vin2_voltage               4
#define  PANEL_Vin1_voltage               5
#define  PANEL_DATA2                      6
#define  PANEL_DATA1                      7
#define  PANEL_SUBADDRESS2                8
#define  PANEL_SUBADDRESS1                9
#define  PANEL_DAC_code                   10      /* callback function: DAC_code */
#define  PANEL_DAC_voltage                11      /* callback function: DAC_voltage */
#define  PANEL_ADDRESS2                   12
#define  PANEL_ADDRESS1                   13
#define  PANEL_READ                       14      /* callback function: READ */
#define  PANEL_WRITE                      15      /* callback function: WRITE */
#define  PANEL_ADC                        16      /* callback function: ADC */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ADC(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DAC_code(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DAC_voltage(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WRITE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
