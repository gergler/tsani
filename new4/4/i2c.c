#include "tsani.h"
#include <utility.h>
#include "i2c.h"

//Везде используется порт подключения I2C-1

int check_value_int(int* value, int min, int max) //Проверяет в диапазоне ли значение и возвращает в диапазон, если нет  
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
	portMask(0, 0x7f); //Левый бит равен нулю, т. к. это port0/line7 (чтение SDAin), остальные  линии port0 на запись
	portMask(1, 0x00); //На port1 все линии используются только для чтения
	portOut(0, 0x00);
	portOut(1, 0x00);
} 

void i2c_write(int sda, int scl)
{
	unsigned char temp;
	temp = (sda << 3) | (scl << 4) | 0x07; //Это SDAout и SCLout для I2C-1
	temp = temp | (1 << 5) | (1 << 6); //На всякий случай, держим в высоком положении SDAout и SCLout для I2C-2
	portOut(0, temp);
}

void i2c_read(int* sda, int* scl)
{
	unsigned char temp1, temp2;
	portIn(0, &temp1);
	portIn(1, &temp2);
	*sda = (temp1 >> 7) & 1; // port0/line7 - SDAin для I2C-1
	*scl = temp2 & 1;		 // port1/line0 - SCLin для I2C-1
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
	return (temp >> 7) & 0x01; // Текущее значение SDA
}

void i2c_send_bit(int bit)
{
	i2c_write(i2c_sda(), 0); //Подхватываем текущее состояние SDA (данные не должны меняться, когда опускаем SCL) и опускаем SCL
	i2c_write(bit, 0); //Меняем значение SDA на содержимое нашего бита
	Delay(DELAY);
	i2c_write(bit, 1); //Поднимаем SCL при неизменном SDA, т. е. начинаем такт
	Delay(DELAY);
	i2c_write(bit, 0); //Опускаем SCL при неизменном SDA, т. е. завершаем такт. 
	Delay(DELAY);	   //Таким образом, в данном такте осуществлена передача бита через значение напряжения на линии SDA
}

void i2c_send_byte(int byte)
{
	int i;
	for(i = 0; i < 8; i++) i2c_send_bit((byte >> (7-i)) & 0x01); //Послать 8 бит подряд из нашего байта, передача ведется от старшего к младшему
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

int i2c_receive_ACK() //"После принятия каждого переданного байта (8 бит) получатель должен выставить подтвеждение."
{					  //Данная функция вызывается после отправки программой байта, т. к. в данной функции мы пытаемся получить бит подтверждения
	double time = 0;	  //"За генерацию тактовых сигналов как при приеме, так и при передаче отвечает ведущий."
	i2c_write(i2c_sda(), 0);
	Delay(DELAY);
	i2c_write(0, 0); //Опускаем SDA	//////////////////
	Delay(DELAY);
	i2c_write(0, 1); //Опускаем SDA ////////////////// Или надо (1, 1) ?
	while (i2c_sda() == 1)
	{
		Delay(DELAY);
		time += DELAY;
		if(time > 1) return 1; //Если больше секунды ждем ACK, то вернуть ошибку
	}
	Delay(DELAY);
	i2c_write(0, 0);

	return 0;
}

void i2c_send_ACK() //Данная функция вызывается после получения программой байта, т. к. в данной функции мы пытаемся отправить бит подтверждения
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
	i2c_write(1, 0); ////////////////// Какому размыканию ключа?
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
	address = address << 1; // Левые 7 бит - адрес, последний бит в значении 0 - значит будет запись в этот адрес
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

int i2c_read_word(int address, int subaddress, int* data, int n) //n - сколько байтов будем считывать за транзакцию
{
	int temp, i;
	check_value_int(&address, 0, 127);
	if(n < 1) n = 1; //Вдруг кто-то по ошибке попросит считать менее одного байта
	address = address << 1; // Левые 7 бит - адрес, последний бит в значении 0 - значит будет запись в этот адрес
	check_value_int(&subaddress, 0, 255);
	
	i2c_start();
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(subaddress);  //Передали в устройство субадрес, чтобы в дальнешем прочитать данные с этого субадреса
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_stop();
	
	i2c_start();
	address = address | 0x01; //Теперь последний бит в значении 1 - будет чтение с этого адреса
	
	i2c_send_byte(address); //Вновь обращаемся к устройству
	if(i2c_receive_ACK() != 0) return 1;
	
	for(i = 0; i < n; i++)
	{
		i2c_receive_byte(data + i); //И читаем данные по указателю (может быть как массивом для n>1, так и переменной для n=1)
		if(i != (n-1)) i2c_send_ACK(); //"Ведущее устройство выставляет ACK после каждого байта, а после последнего выставляет NACK."
	}
	i2c_send_NACK(); 
	
	i2c_stop();

	return 0;
}

int i2c_read_temperature(double* value)
{						  
	int temp, data = 0;
	int address = 0x28; //Адрес термодатчика - 0x28. Выдает два байта информации, используются биты с 15 по 6: 0000 0000 00XX XXXX  
	address = (address << 1) | 0x01; //Будем читать по этому адресу
	
	i2c_start();
	
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_receive_byte(&temp);
	i2c_send_NACK();

	data = temp << 2; //Термодатчик возвращает двумя байтами (16 бит) 10 бит информации. 8 из них мы уже получили - запишем их
	
	//i2c_receive_byte(&temp);
	//i2c_send_NACK();
	
	//data = data + (temp >> 6); //Теперь запишем оставшиеся 2 бита, итого 10

	i2c_stop();
	
	if(data >= 512)
		data = data - 1024;
	
	*value = (double)data/4; //Запишем конкретное значение температуры в градусах

	return 0;
}












