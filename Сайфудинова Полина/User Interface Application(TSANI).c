//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  16.02.2021 at 11:05:15 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "User Interface Application(TSANI).h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double val;
//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	ni6251Slot(2);
	
	ni6251Close();
	
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK Output (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			double volt1, volt0, volt;
			GetCtrlVal(panelHandle, PANEL_Output, &volt);
			analogOut(0, volt);
			analogIn(0, &volt0);
			analogOut(1, volt);
			analogIn(1, &volt1);
			SetCtrlVal(panelHandle, PANEL_Input0, volt0);
			SetCtrlVal(panelHandle, PANEL_Input1, volt1);

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

int CVICALLBACK Input0 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			
			break;
			
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK Input1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			 
			break;
			
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK Timer1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			
			double volt1, volt0, volt;
			GetCtrlVal(panelHandle, PANEL_Output, &volt);
			analogOut(1, volt);
			analogIn(1, &volt1);
			analogOut(0, volt);
			analogIn(0, &volt0);
			SetCtrlVal(panelHandle, PANEL_Input1, volt1);
			SetCtrlVal(panelHandle, PANEL_Input0, volt0);
			
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
