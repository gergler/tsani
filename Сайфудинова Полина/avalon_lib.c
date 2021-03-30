#include "tsani.h"
#include <utility.h>
#include "avalon_lib.h"

void Init(int adr, int subadr) {
	portMask(0, 0xFF); 
	portMask(1, 0xFF);
	portMask(2, 0x07); 
	portOut(2, 0x06); // ALE = 0, READ = 1, WRITE = 1
	
	int a = (subadr<<3)|adr;
	
	unsigned char adr0;
	unsigned char adr1;

	adr0 = (a&0xFF);
	adr1 = (a>>8)&0xFF;
	
	portOut(0, adr0);
	portOut(1, adr1);
	
	portOut(2, 0x07); // ALE =1, READ = 1, WRITE = 1 
	portOut(2, 0x06); // ALE = 0, READ = 1, WRITE = 1 
}

void Write(int adr, int subadr, int data) {	
	Init(adr, subadr);
	
	unsigned char w0, w1;
	
	w0 = (data&0xFF);
	w1 = ((data>>8)&0xFF);
	
	portOut(0, w0);
	portOut(1, w1);
	
	portOut(2, 0x02); // ALE = 0, READ = 1, WRITE = 0
	portOut(2, 0x06); // ALE = 0, READ = 1, WRITE = 1
}

int Read(int adr, int subadr) {
	Init(adr, subadr);
	
	portMask(0, 0x00);
	portMask(1, 0x00);
	portOut(2, 0x04);
	
	Delay(0.000006); 

	unsigned char r0, r1;
	
	portIn(0, &r0);
	portIn(1, &r1);
	
	portOut(2, 0x06);
	portMask(0, 0xFF);
	portMask(1, 0xFF);
	
	return (r0 | (r1<<8));
}

void DACcode(double U) {
    double Code=255/3.3*U;
    if (Code<0) {
        U=0;
        Code = 0;
    }
    if (Code>255) {
        U=3.3;
        Code = 255;
    }
    Write(2, 2, Code);
	Write(2, 3, Code);
}

void DAC(double Code) {
    double U=3.3*Code/255;
    if (U<0) {
       U=0;
       Code = 0;
    }
    if (U>3.3) {
       U=3.3;
       Code = 255;
    }  
    Write(2, 2, Code);
	Write(2, 3, Code);
}


void ACP(unsigned int* data1, unsigned int* data2) {
    double codeACP;

	Write(2, 0x11, 3); // start and iack
	
	int check = 0x03;
	while(!((check)&(0x01)==0x01)) {
		check = Read(2, 0x11);
		Delay(0.1);
	}
	
	if (NULL != data1)
    	*data1=Read(2, 0x16); //регистр АЦП Vin1, используется для чтения оцифрованных данных по окончании измерения
    if (NULL != data2)
		*data2=Read(2, 0x17);
}

void SETI() {
    int reg;
    reg = Read(2, 0x11); // чтение РЕГИСТРА КОМАНД АЦП
    reg = reg|0x4; // побитовое ИЛИ, для установки 1 в SETI
    Write(2, 0x11, reg); // запись в РЕГИСТР, что SETI = 1, При записи 1 в этот бит модуль установит прерывание; запись 0 игнорируется; чтение всегда возвращает 0.
}

void IACK(){
    int reg;
    reg = Read(2, 0x11); // чтение РЕГИСТРА КОМАНД АЦП
    reg = reg|0x2; // побитовое ИЛИ, для установки 1 в IACK
    Write(2, 0x11, reg); // запись в РЕГИСТР, что IACK = 1, чтобы снять предыдущее прерывание и начать запуск нового измерения
}



























