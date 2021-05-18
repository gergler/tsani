#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "main.h"
#include "toolbox.h"

#include "i2c.h"

static int panelHandle;

/*
Обратите внимание, что в задании с чтением слова таблица ASCII в методичке дана с числами в десятичной системе,
а программа testerI2C принимает значения в 16-ричном виде. Надо будет не только составить имя, но и перевести числа в 16-ричные для ввода в программу.
Я предпочел скачать программу по переводу чисел на телефон для решения этой великой проблемы.
Например, на скриншоте tester.PNG плохо видно, но там введено имя JULIA

И не забудьте выставить адрес тестера, когда будете пытаться с ним работать, в данной программе все обращения к тестеру ведутся по адресу 0x01
*/

int main (int argc, char *argv[])
{
    int error = 0;
	ni6251Slot(2);
	i2c_init();
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "main.uir", PANEL));
    
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

int CVICALLBACK BUTTON_SINGLE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	if (event == EVENT_COMMIT)
	{
		double value;
		i2c_read_temperature(&value);
		SetCtrlVal(panelHandle, PANEL_NUMERIC_TEMP, value);
	}
	return 0;
}

int CVICALLBACK TIMER_TEMPERATURE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_TIMER_TICK)
	{
		double value;
		i2c_read_temperature(&value);
		SetCtrlVal(panelHandle, PANEL_NUMERIC_TEMP, value);
		PlotStripChartPoint(panelHandle, PANEL_STRIPCHART, value);
	}
	return 0;
}

int CVICALLBACK TBUTTON_TEMPERATURE_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	if(event == EVENT_COMMIT)
	{
		int state;
		GetCtrlVal(PANEL, PANEL_TBUTTON_TEMPERATURE, &state);
		if(state == 0x01) ClearStripChart(PANEL, PANEL_STRIPCHART);
		SetCtrlAttribute(PANEL, PANEL_TIMER_TEMPERATURE, ATTR_ENABLED, state);
	};
	return 0;
}

int CVICALLBACK LEDButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	if(event == EVENT_LEFT_CLICK)
	{
		int temp, data = 0;
		GetCtrlVal(PANEL, PANEL_LED0, &temp); data |= (temp << 0);
		GetCtrlVal(PANEL, PANEL_LED1, &temp); data |= (temp << 1);
		GetCtrlVal(PANEL, PANEL_LED2, &temp); data |= (temp << 2);
		GetCtrlVal(PANEL, PANEL_LED3, &temp); data |= (temp << 3);
		GetCtrlVal(PANEL, PANEL_LED4, &temp); data |= (temp << 4);
		GetCtrlVal(PANEL, PANEL_LED5, &temp); data |= (temp << 5);
		GetCtrlVal(PANEL, PANEL_LED6, &temp); data |= (temp << 6);
		GetCtrlVal(PANEL, PANEL_LED7, &temp); data |= (temp << 7); //Прочитали состояние индикаторов граф. интерфейса в надежде, что они сопадают с состояниями светодиодов тестера
	
		switch (control) //В зависимости от того, какой светодиод был нажат мышкой
		{
			case PANEL_LED0:
				data ^= (0x01 << 0);
				SetCtrlVal(PANEL, PANEL_LED0, ((data >> 0) & 0x01));
				break;
			case PANEL_LED1:
				data ^= (0x01 << 1);
				SetCtrlVal(PANEL, PANEL_LED1, ((data >> 1) & 0x01));
				break;
			case PANEL_LED2:
				data ^= (0x01 << 2);
				SetCtrlVal(PANEL, PANEL_LED2, ((data >> 2) & 0x01));
				break;
			case PANEL_LED3:
				data ^= (0x01 << 3);
				SetCtrlVal(PANEL, PANEL_LED3, ((data >> 3) & 0x01));
				break;
			case PANEL_LED4:
				data ^= (0x01 << 4);
				SetCtrlVal(PANEL, PANEL_LED4, ((data >> 4) & 0x01));
				break;
			case PANEL_LED5:
				data ^= (0x01 << 5);
				SetCtrlVal(PANEL, PANEL_LED5, ((data >> 5) & 0x01));
				break;
			case PANEL_LED6:
				data ^= (0x01 << 6);
				SetCtrlVal(PANEL, PANEL_LED6, ((data >> 6) & 0x01));
				break;
			case PANEL_LED7:
				data ^= (0x01 << 7);
				SetCtrlVal(PANEL, PANEL_LED7, ((data >> 7) & 0x01));
				break; 
		}
		i2c_write_word(0x01, 0x00, data); //0x01 - адрес тестера, 0x00 - регистр записи для управления светодиодами, data - статусы светодиодов
		//SetCtrlVal(PANEL, PANEL_NUMERIC_DEBUG, data);
	};
	return 0;
}

int CVICALLBACK TIMER_LED_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_TIMER_TICK)
	{
		int temp, data = 0;
		GetCtrlVal(PANEL, PANEL_LED0, &temp); data |= (temp << 0);
		GetCtrlVal(PANEL, PANEL_LED1, &temp); data |= (temp << 1);
		GetCtrlVal(PANEL, PANEL_LED2, &temp); data |= (temp << 2);
		GetCtrlVal(PANEL, PANEL_LED3, &temp); data |= (temp << 3);
		GetCtrlVal(PANEL, PANEL_LED4, &temp); data |= (temp << 4);
		GetCtrlVal(PANEL, PANEL_LED5, &temp); data |= (temp << 5);
		GetCtrlVal(PANEL, PANEL_LED6, &temp); data |= (temp << 6);
		GetCtrlVal(PANEL, PANEL_LED7, &temp); data |= (temp << 7); //Прочитали

		data = ((data >> 7) | (data << 1)) & 0xff; //Сдвинули
		SetCtrlVal(PANEL, PANEL_NUMERIC_DEBUG, data);
		i2c_write_word(0x01, 0x00, data); //Записали в тестер
		
		SetCtrlVal(PANEL, PANEL_LED0, ((data >> 0) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED1, ((data >> 1) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED2, ((data >> 2) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED3, ((data >> 3) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED4, ((data >> 4) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED5, ((data >> 5) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED6, ((data >> 6) & 0x01));
		SetCtrlVal(PANEL, PANEL_LED7, ((data >> 7) & 0x01)); //Отразили состояния в граф. интерфейсе
	}
		
	return 0;
}
	
int CVICALLBACK TBUTTON_LED_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	if(event == EVENT_COMMIT)
	{
		int state;
		GetCtrlVal(PANEL, PANEL_TBUTTON_LED, &state);
		SetCtrlAttribute(PANEL, PANEL_TIMER_LED, ATTR_ENABLED, state);
	};
	return 0;
}

int CVICALLBACK BUTTON_WORD_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	if (event == EVENT_COMMIT)
	{
		int i;
		char data[9]; //8 регистров чтения + NULL в конце строки (символ конца строки)
		int temp = 0;
		for(i = 0; i < 8; i++)
		{
			i2c_read_word(0x01, (0x08 + i), &temp, 1); //Регистры чтения 0x08-0x0F
			//break;
			data[i] = (unsigned char)temp;
		}
		
		//SetCtrlVal(PANEL, PANEL_NUMERIC_DEBUG2, temp);

		data[8] = NULL;
		
		SetCtrlVal(panelHandle, PANEL_STRING, data);
		
	}
	return 0;
}


































