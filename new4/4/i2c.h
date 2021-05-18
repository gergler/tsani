#ifndef __i2c_H__
#define __i2c_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

#define DELAY 0.001
		
int check_value_int(int* value, int min, int max);
void i2c_init(void);
void i2c_write(int sda, int scl);
void i2c_read(int* sda, int* scl);
void i2c_start();
void i2c_stop();
int i2c_sda();
void i2c_send_bit(int bit);
void i2c_send_byte(int byte);
int i2c_receive_ACK();
void i2c_send_ACK();
void i2c_send_NACK();
int i2c_receive_bit();
void i2c_receive_byte(int* byte);
int i2c_write_word(int address, int subaddress, int word);
int i2c_read_word(int address, int subaddress, int* data, int n);
int i2c_read_temperature(double* value);


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __i2c_H__ */
