#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "external_eeprom.h"
unsigned int key,func_flag=0,key1=0xFF;
void init_config(void);

unsigned char key_press=0;


//matrix
void init_matrix_keypad();
unsigned char scan_key();
unsigned char read_switches(unsigned char detection);


//adc
void init_adc();
unsigned short read_adc(unsigned char channel);


void init_uart();
void put_char(unsigned char byte);
int put_string(const char *s);