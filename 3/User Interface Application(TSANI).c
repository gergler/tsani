#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "User Interface Application(TSANI).h"
#include "toolbox.h"
#include "avalon.h"

static int panelHandle;

#include <stdio.h>
#include <stdlib.h>
int point=26501;
double realcodeACP[26501];
double perfectACPvolt[26501];
double LSB=2.56/1024;
double ideal_to_1023=2.5575-2.56/1024/2;
double zero=0.0;
double scale=0.0;
double real_to_1023=0.0;
double integral[26501];
double max=0.0;
double quant_error=0.5;
int up_polka=0.0;
int down_polka=0.0;

void ACPread()
{
    FILE* pFcode;
    FILE* pFvolt;

    pFvolt=fopen("volt.txt","r"); // real code
    pFcode=fopen("adc.txt","r"); // real code

    for (int i=0;i<26501;i++){
        float temp_code;
        float t;
        fscanf(pFcode,"%f",&temp_code);
        realcodeACP[i]=temp_code;
        fscanf(pFvolt,"%f",&t);
        perfectACPvolt[i]=t;
    }

    fclose(pFcode);
    fclose(pFvolt);
    PlotXY (panelHandle, PANEL_GRAPH_3, perfectACPvolt,realcodeACP, 26501, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_DOT, 1, VAL_YELLOW);
}


void Zerroerror(){
    int j=0;
    do
       j++;
    while (realcodeACP[j]!=1);
    down_polka=j;
    zero= perfectACPvolt[j] -0.5*LSB;
    SetCtrlVal(PANEL, PANEL_NUMERIC_9, zero/LSB);
}

void without_zero(){
    for (int i=0; i<26501;i++){
        perfectACPvolt[i]=perfectACPvolt[i]- zero;
    }
}

void Scale_error(){
    int j=0;
    do
        j++;
    while (realcodeACP[j]!=1023);
    up_polka=j;
    scale=(ideal_to_1023 - perfectACPvolt[j]);
    real_to_1023 =perfectACPvolt[j];
    SetCtrlVal(PANEL, PANEL_NUMERIC_14,  scale/LSB);
}

void without_scale(){
    for (int i=574;i<26501;i++){
        perfectACPvolt[i]=perfectACPvolt[i]*(ideal_to_1023/real_to_1023);
    }
    PlotXY(panelHandle, PANEL_GRAPH_7, perfectACPvolt, realcodeACP, 26501, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_DOT, 1, VAL_GREEN);
}

void integral_error(){
    for (int i=down_polka;i<up_polka+1;i++){
        integral[i]=(perfectACPvolt[i]-LSB*realcodeACP[i])/LSB +quant_error;
        //if(integral[i]<0)
        //    integral[i]=(-1)*integral[i];
        if(integral[i]>max)
            max =integral[i];
    }
    PlotXY(panelHandle, PANEL_GRAPH_8, perfectACPvolt, integral, 26501, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_DOT, 1, VAL_WHITE); 
    SetCtrlVal(PANEL, PANEL_NUMERIC_15,  max);
}

//////////////////////////////////////////////////////////////////////////////////////


int main (int argc, char *argv[])
{
    int error = 0;
    ni6251Slot(2);
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
    
    Write(2,0,3); 
    ACPread();
    
    Zerroerror();
    without_zero();
    Scale_error();
    without_scale();
    integral_error();
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
	ni6251Close();
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
