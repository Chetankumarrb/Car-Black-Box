
#include "main.h"
#include "black_box.h"

// matrix keypad
void init_matrix_keypad()
{
    TRISB = 0x1E;
    RBPU = 0; // make portb as pull up
}

unsigned char scan_key()
{
    // 1st row
    RB5 = 0;
    RB6 = RB7 = 1;
    if (RB1 == 0)
        return 1;
    else if (RB2 == 0)
        return 4;
    else if (RB3 == 0)
        return 7;
    else if (RB4 == 0)
        return 10;

    // 2nd row
    RB6 = 0;
    RB5 = RB7 = 1;
    if (RB1 == 0)
        return 2;
    else if (RB2 == 0)
        return 5;
    else if (RB3 == 0)
        return 8;
    else if (RB4 == 0)
        return 11;

    // 3rd row
    RB7 = 0;
    RB6 = RB5 = 1;
    if (RB1 == 0)
        return 3;
    else if (RB2 == 0)
        return 6;
    else if (RB3 == 0)
        return 9;
    else if (RB4 == 0)
        return 12;

    return 0xFF;
}
unsigned char read_switches(unsigned char detection)
{
    static unsigned char once = 0, key;
    static int longpress = 0;


    if (detection == 1)
    {
        // Edge
        key = scan_key();
        if (key != 0xFF && once != 2)
        {
            once = 1;
            if (longpress++ > 500)
            {
                longpress = 0;
                key_press = 2;
                once = 2;
                return key;
            }
            key1 = key;
        }
        else if (key == 0xFF && once == 1)
        {
            once  = 0;
            longpress = 0; 
            return key1;
        }
        else if(key == 0xFF)
        {
            longpress = 0;
            key1 = 0xFF;
            once = 0;
        }
    }
    return 0xFF;
}

// adc
void init_adc()
{
    ADON = 1;
    // REFRENCE VOLTAGE
    VCFG1 = 0;
    VCFG0 = 0;
    // ANALOG PIN FOR CHANNEL 4 for temp
    PCFG3 = 1;
    PCFG2 = 0;
    PCFG1 = 1;
    PCFG0 = 0;

    // RIGHT ALIGN
    ADFM = 1;
    // CLOCK SELECT
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;
    // ACQUSTION TIME
    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;
}

unsigned short read_adc(unsigned char channel)
{
    ADCON0 = ((ADCON0 & 0xC3) | (channel << 2));
    GO = 1;
    while (GO)
        ;

    return (ADRESL | ((ADRESH & 0x03) << 8));
}

void init_timer0()
{
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    TMR0IF = 0;

    TMR0 = 6;
    TMR0ON = 1;
    T08BIT = 1;
    T0CS = 0;
    PSA = 1;
}

unsigned int count = 0;
void __interrupt() isr()
{
    // TMR0 = TMR0 + 6+ 3;
    TMR0 = TMR0 + 8;
    if (TMR0IF)
    {
        if (count++ == 20000)
        {
            secC++;
            sec--;
            count = 0;
        }
        TMR0IF = 0;
    }
}



//uart

void init_uart(void)
{
	/* Serial initialization */
	TRISC7 = 1;
    TRISC6 = 0;

	
	BRGH = 1;
	SPEN = 1;
	
	/* TX/RC7 and RX/RC6 act as serial port */
	/* Continous reception enable or disable */ 
	CREN = 1;
	
	

	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;
/* Receive Interruot Enable Bit (Enabling the serial port Interrupt) */
    RCIE = 1;


}

void put_char(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXREG = byte;
}

int put_string(const char *s)
{
	while(*s)		
	{
		put_char(*s++);	
	}

	return 0;
}




