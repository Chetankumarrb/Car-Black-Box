

#include "main.h"
#include "black_box.h"
void init_config(void)
{
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_timer0();
    init_i2c();
    init_ds1307();
    init_uart();
    write_external_eeprom(0x82, 1);
    write_external_eeprom(0x83, 1);
    write_external_eeprom(0x84, 1);
    write_external_eeprom(0x85, 1);
    orgArr[0] = read_external_eeprom(0x82);
    orgArr[1] = read_external_eeprom(0x83);
    orgArr[2] = read_external_eeprom(0x84);
    orgArr[3] = read_external_eeprom(0x85);
}

void main(void) {
    init_config();

    while (1) {
        get_time();

        key = read_switches(1);
        if (func_flag == 0)
            Dashboard_diplay();
        if (func_flag == 1)
            passwd_display();
        if (func_flag == 2)
            main_menu();
        if(func_flag == 3)
        {
             View_Log();
        }
        else if(func_flag == 4)
        {
            Download_Log();
        }
        else if(func_flag == 5)
        {
            Clear_log();
        }
        else if(func_flag == 6)
        {
            Set_time();
        }
        else if(func_flag == 7)
        {
            Change_password();
        }
        if (key == SW1 || key == SW2 || key == SW3)
        {
            eeprom_storing();
            Clear_log_flag = 0;
        }
    }
}
