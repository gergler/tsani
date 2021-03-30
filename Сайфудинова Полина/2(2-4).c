//==============================================================================
//
// Title:       2(2-4)
// Purpose:     A short description of the application.
//
// Created on:  16.03.2021 at 10:32:00 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "2(2-4).h"
#include "toolbox.h"
#include "avalon_lib.h"

static int panelHandle;
int adr, subadr, data, value;

int main (int argc, char *argv[])
{
    int error = 0;
    ni6251Slot(2);
	
	Write(2, 0, 3); 
	
	//ADC
    Write(2, 0x10, 3);
	//обнуляем
    Write(2, 0x12, 0); //нач., используется для указания адреса (0...255), с которого начнётся запись оцифрованных значений в ЗУ
    Write(2, 0x13, 0); //конеч., используется для указания конечного адреса ЗУ. Всего будет оцифровано (конечный адрес − начальный адрес + 1) значений
    Write(2, 0x14, 0);
	
	
	/* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "2(2-4).uir", PANEL));
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
	ni6251Close();
    return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK WRITE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_ADDRESS1, &adr);
			GetCtrlVal(panelHandle, PANEL_SUBADDRESS1, &subadr);
			GetCtrlVal(panelHandle, PANEL_DATA1, &data);
			Write(adr, subadr, data);
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

int CVICALLBACK READ (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_ADDRESS2, &adr);
			GetCtrlVal(panelHandle, PANEL_SUBADDRESS2, &subadr);
			value = Read(adr, subadr);
			SetCtrlVal(panelHandle, PANEL_DATA2, value);
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

int CVICALLBACK ADC (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	double u;
	unsigned int  codeACP1, codeACP2;
	switch (event) {
		case EVENT_COMMIT:
			
			ACP(&codeACP1, &codeACP2);
			SetCtrlVal(panelHandle, PANEL_Vin1_code, codeACP1);
			u = codeACP1*2.56/1024;
			SetCtrlVal(panelHandle, PANEL_Vin1_voltage, u);
		
			SetCtrlVal(panelHandle, PANEL_Vin2_code, codeACP2);
			u = codeACP2*2.56/1024;
			SetCtrlVal(panelHandle, PANEL_Vin2_voltage, u); 
			
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

int CVICALLBACK DAC_voltage (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	double U, code;
	switch (event) 
	{
		case EVENT_COMMIT:	    
			
			GetCtrlVal(panelHandle, PANEL_DAC_voltage, &U);
			DACcode(U);
			code=Read(2, 2);
			SetCtrlVal(panelHandle, PANEL_DAC_code, code);
			
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

int CVICALLBACK DAC_code (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	double U, code;
	switch (event) 
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panelHandle, PANEL_DAC_code, &code);
			DAC(code);
			U=Read(2, 2)*3.3/255;
			SetCtrlVal(panelHandle, PANEL_DAC_voltage, U);
			
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

int CVICALLBACK iack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			IACK();
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

int CVICALLBACK seti (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			SETI();
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
