#include "2(1-3).h"

//==============================================================================
//
// Title:       2(1-2)
// Purpose:     A short description of the application.
//
// Created on:  02.03.2021 at 9:24:19 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "2(1-2).h"
#include "toolbox.h"

static int panelHandle;
double in, hor, vert;

int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "2(1-2).uir", PANEL));
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */ 
    DiscardPanel (panelHandle);
    return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK Timer1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			analogIn(2, &in); 
			SetCtrlVal(panelHandle, PANEL_AI2, in);
			hor=-(in-2.5)/2.5;
			SetCtrlVal(panelHandle, PANEL_HOR, hor);
			analogIn(3, &in);  
			SetCtrlVal(panelHandle, PANEL_AI3, in);  
			vert=(in-2.3)/2.5;
			SetCtrlVal(panelHandle, PANEL_VERT, vert);
			PlotPoint(panelHandle, PANEL_GRAPH1, hor, vert, VAL_SOLID_SQUARE, VAL_WHITE);
			
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int  CVICALLBACK Vert(int panel, int control, int event, void *callbackData, int eventData1, int eventData2){ return 0; }
int  CVICALLBACK Hor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2){ return 0; }
int  CVICALLBACK AI2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2){ return 0; }
int  CVICALLBACK AI3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2){ return 0; }

int CVICALLBACK GRAPH1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:

			break;
		case EVENT_LEFT_CLICK:

			break;
		case EVENT_RIGHT_CLICK:

			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
