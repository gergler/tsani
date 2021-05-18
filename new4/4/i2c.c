#include "tsani.h"
#include <utility.h>
#include "i2c.h"

//����� ������������ ���� ����������� I2C-1

int check_value_int(int* value, int min, int max) //��������� � ��������� �� �������� � ���������� � ��������, ���� ���  
{
	if (*value > max)
	{
		*value = max;
	}
	else if (*value < min)
	{
		*value = min;
	}
	
	return 0;
}

void i2c_init(void)
{
	portMask(0, 0x7f); //����� ��� ����� ����, �. �. ��� port0/line7 (������ SDAin), ���������  ����� port0 �� ������
	portMask(1, 0x00); //�� port1 ��� ����� ������������ ������ ��� ������
	portOut(0, 0x00);
	portOut(1, 0x00);
} 

void i2c_write(int sda, int scl)
{
	unsigned char temp;
	temp = (sda << 3) | (scl << 4) | 0x07; //��� SDAout � SCLout ��� I2C-1
	temp = temp | (1 << 5) | (1 << 6); //�� ������ ������, ������ � ������� ��������� SDAout � SCLout ��� I2C-2
	portOut(0, temp);
}

void i2c_read(int* sda, int* scl)
{
	unsigned char temp1, temp2;
	portIn(0, &temp1);
	portIn(1, &temp2);
	*sda = (temp1 >> 7) & 1; // port0/line7 - SDAin ��� I2C-1
	*scl = temp2 & 1;		 // port1/line0 - SCLin ��� I2C-1
}

void i2c_start()
{
	i2c_write(1, 1);
	Delay(DELAY);
	i2c_write(0, 1);
	Delay(DELAY);
	i2c_write(0, 0);
	Delay(DELAY);
}

void i2c_stop()
{
	i2c_write(0, 0);
	Delay(DELAY);
	i2c_write(0, 1);
	Delay(DELAY);
	i2c_write(1, 1);
	Delay(DELAY);
}

int i2c_sda()
{
	unsigned char temp;
	portIn(0, &temp);
	return (temp >> 7) & 0x01; // ������� �������� SDA
}

void i2c_send_bit(int bit)
{
	i2c_write(i2c_sda(), 0); //������������ ������� ��������� SDA (������ �� ������ ��������, ����� �������� SCL) � �������� SCL
	i2c_write(bit, 0); //������ �������� SDA �� ���������� ������ ����
	Delay(DELAY);
	i2c_write(bit, 1); //��������� SCL ��� ���������� SDA, �. �. �������� ����
	Delay(DELAY);
	i2c_write(bit, 0); //�������� SCL ��� ���������� SDA, �. �. ��������� ����. 
	Delay(DELAY);	   //����� �������, � ������ ����� ������������ �������� ���� ����� �������� ���������� �� ����� SDA
}

void i2c_send_byte(int byte)
{
	int i;
	for(i = 0; i < 8; i++) i2c_send_bit((byte >> (7-i)) & 0x01); //������� 8 ��� ������ �� ������ �����, �������� ������� �� �������� � ��������
	/*for (i = 0; i < 8; i++)
	{
		i2c_write(i2c_sda(), 0);
		Delay(DELAY);
		i2c_write((byte >> (7 - i)) &1, 0);
		Delay(DELAY);
		i2c_write((byte >> (7 - i)) &1, 1);
		Delay(DELAY);
		i2c_write((byte >> (7 - i)) &1, 0);
		Delay(DELAY);
	} */
}

int i2c_receive_ACK() //"����� �������� ������� ����������� ����� (8 ���) ���������� ������ ��������� ������������."
{					  //������ ������� ���������� ����� �������� ���������� �����, �. �. � ������ ������� �� �������� �������� ��� �������������
	double time = 0;	  //"�� ��������� �������� �������� ��� ��� ������, ��� � ��� �������� �������� �������."
	i2c_write(i2c_sda(), 0);
	Delay(DELAY);
	i2c_write(0, 0); //�������� SDA	//////////////////
	Delay(DELAY);
	i2c_write(0, 1); //�������� SDA ////////////////// ��� ���� (1, 1) ?
	while (i2c_sda() == 1)
	{
		Delay(DELAY);
		time += DELAY;
		if(time > 1) return 1; //���� ������ ������� ���� ACK, �� ������� ������
	}
	Delay(DELAY);
	i2c_write(0, 0);

	return 0;
}

void i2c_send_ACK() //������ ������� ���������� ����� ��������� ���������� �����, �. �. � ������ ������� �� �������� ��������� ��� �������������
{
	i2c_send_bit(0);
}

void i2c_send_NACK()
{
	i2c_send_bit(1);
}

int i2c_receive_bit()
{
	int bit;
	i2c_write(1, 0); ////////////////// ������ ���������� �����?
	Delay(DELAY);
	i2c_write(1, 1);
	Delay(DELAY);
	bit = i2c_sda();
	Delay(DELAY);
	i2c_write(1, 0);
	return bit;
}

void i2c_receive_byte(int* byte)
{
	int i, temp = 0;
	for(i = 0; i < 8; i++)
	{
		temp = temp | (i2c_receive_bit() << (7 - i));
	}
	*byte = temp;
}

int i2c_write_word(int address, int subaddress, int word)
{
	check_value_int(&address, 0, 127);
	address = address << 1; // ����� 7 ��� - �����, ��������� ��� � �������� 0 - ������ ����� ������ � ���� �����
	check_value_int(&subaddress, 0, 255);
	check_value_int(&word, 0, 255);
	
	i2c_start();
	
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(subaddress);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(word);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_stop();

	return 0;
}

int i2c_read_word(int address, int subaddress, int* data, int n) //n - ������� ������ ����� ��������� �� ����������
{
	int temp, i;
	check_value_int(&address, 0, 127);
	if(n < 1) n = 1; //����� ���-�� �� ������ �������� ������� ����� ������ �����
	address = address << 1; // ����� 7 ��� - �����, ��������� ��� � �������� 0 - ������ ����� ������ � ���� �����
	check_value_int(&subaddress, 0, 255);
	
	i2c_start();
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(subaddress);  //�������� � ���������� ��������, ����� � ��������� ��������� ������ � ����� ���������
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_stop();
	
	i2c_start();
	address = address | 0x01; //������ ��������� ��� � �������� 1 - ����� ������ � ����� ������
	
	i2c_send_byte(address); //����� ���������� � ����������
	if(i2c_receive_ACK() != 0) return 1;
	
	for(i = 0; i < n; i++)
	{
		i2c_receive_byte(data + i); //� ������ ������ �� ��������� (����� ���� ��� �������� ��� n>1, ��� � ���������� ��� n=1)
		if(i != (n-1)) i2c_send_ACK(); //"������� ���������� ���������� ACK ����� ������� �����, � ����� ���������� ���������� NACK."
	}
	i2c_send_NACK(); 
	
	i2c_stop();

	return 0;
}

int i2c_read_temperature(double* value)
{						  
	int temp, data = 0;
	int address = 0x28; //����� ������������ - 0x28. ������ ��� ����� ����������, ������������ ���� � 15 �� 6: 0000 0000 00XX XXXX  
	address = (address << 1) | 0x01; //����� ������ �� ����� ������
	
	i2c_start();
	
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_receive_byte(&temp);
	i2c_send_NACK();

	data = temp << 2; //����������� ���������� ����� ������� (16 ���) 10 ��� ����������. 8 �� ��� �� ��� �������� - ������� ��
	
	//i2c_receive_byte(&temp);
	//i2c_send_NACK();
	
	//data = data + (temp >> 6); //������ ������� ���������� 2 ����, ����� 10

	i2c_stop();
	
	if(data >= 512)
		data = data - 1024;
	
	*value = (double)data/4; //������� ���������� �������� ����������� � ��������

	return 0;
}












