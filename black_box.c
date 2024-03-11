

#include "main.h"
#include "black_box.h"

unsigned char add = 0x00;
unsigned char w_add = 0x00;

void Dashboard_diplay() {

    clcd_print("TIME", LINE1(2));
    clcd_print("E", LINE1(11));
    clcd_print("SP", LINE1(14));

    clcd_print(arr[index], LINE2(11));
    display_time();

    if (index == 8 && key == SW1)
        index = 1;
    else if (index < 7 && key == SW1)
        index++;
    else if (key == SW2 && index > 1 && index < 8)
        index--;
    else if (key == SW3)
        index = 8;

    adc_val = read_adc(4);
    speed = adc_val / 10.25;
    clcd_putch(((speed % 10) + 48), LINE2(15));
    clcd_putch(((speed / 10) + 48), LINE2(14));

    if (key == SW11) {
        secC = 0;
        key = 0xFF;
        CLEAR_DISP_SCREEN;
        func_flag = 1;
    }
}

void passwd_display() {

    if (ff)
        clcd_print(" ENTER PASSWORD ", LINE1(1));

    if (secC >= 3 && key == 0xFF) {

        func_flag = 0;
        i = 0;
        CLEAR_DISP_SCREEN;
        secC = 0;
        return;
    } else if (key != 0xFF && i < 4) {
        secC = 0;
        if (key == SW11)
            temp[i] = '0';
        else if (key == SW12)
            temp[i] = '1';

        clcd_putch('*', LINE2(i));
        i++;
    } else if (i >= 4) {
        secC = 0;
        for (int j = 0; j < 4; j++) {
            a = 0;
            if (orgArr[j] != temp[j] && c != 1) {
                ff = 1;
                a = 1, sec = 120;

                i = 0;
                clcd_print("ATTEMPT FAILURE", LINE1(0));

                clcd_print("ATTEMPT LEFT -  ", LINE2(0));
                clcd_putch((--c) + 48, LINE2(15));
                for (int l = 5000; l--;)
                    for (int k = 200; k--;)
                        ;
                CLEAR_DISP_SCREEN;
                secC = 0;
                break;
            } else if (c == 1) {
                ff = 0;
                clcd_print("YOU ARE BLOCKED ", LINE1(0));
                clcd_print("WAIT FOR ", LINE2(0));
                clcd_putch(sec % 10 + '0', LINE2(11));
                clcd_putch((sec / 10) % 10 + '0', LINE2(10));
                clcd_putch(sec / 100 + '0', LINE2(9));
                if (sec == 0) {
                    CLEAR_DISP_SCREEN;
                    func_flag = 0;
                    sec = 120;
                }
                return;
            }
        }
        if (a == 0) {
            CLEAR_DISP_SCREEN;
            clcd_print("  SUCCESS  ", LINE1(0));
            for (int i = 0; i < 200; i++)
                ;
            CLEAR_DISP_SCREEN;
            func_flag = 2;
        }
    }
}

void main_menu() {

    if(key == SW12 && key_press == 2)
    {
        CLEAR_DISP_SCREEN;
        func_flag = 0;
        key_press = 0;
    } 
    else  if (key == SW11 && key_press == 2)
    {
        func_flag = 3 + m_index;
        CLEAR_DISP_SCREEN;
        key_press = 0;
    } 
    else 
    {
        if (m_index < 4 && key == SW12)
        {
            CLEAR_DISP_SCREEN;
            m_index++;
        } 
        else if (m_index > 0 && key == SW11)
        {
            CLEAR_DISP_SCREEN;
            m_index--;
        }
        if (m_index % 2 == 0) 
        {
            clcd_print("*", LINE1(0));
            clcd_print(menu[m_index], LINE1(2));
            if (m_index != 4)
            {
                clcd_print(menu[m_index + 1], LINE2(2));
            }
        }
        if (m_index % 2 == 1) 
        {
            clcd_print("*", LINE2(0));
            clcd_print(menu[m_index - 1], LINE1(2));
            clcd_print(menu[m_index], LINE2(2));
        }
    }
}

void eeprom_storing() {

    if (w_add > 0x78)
        w_add = 0x00;

    for (int i = 0; i < 8; i++) {
        write_external_eeprom(w_add++, time[i]);
    }

    write_external_eeprom(w_add++, *arr[index]);

    write_external_eeprom(w_add++, *(arr[index] + 1));

    write_external_eeprom(w_add++, speed / 10 + '0');

    write_external_eeprom(w_add++, speed % 10 + '0');


}

void View_Log() {
    if (Clear_log_flag == 0) {
        static char view_flag = 0;
        clcd_print("View logg       ", LINE1(0));

        if (add > 0x78)
            add = 0x00;

        if (view_flag == 0) {
            for (int i = 0; i < 12; i++) {
                clcd_putch(read_external_eeprom(add++), LINE2(i));
            }
            view_flag = 1;
        }
        if (view_flag) {
            if (key == SW12) {
                for (int i = 0; i < 12; i++) {
                    clcd_putch(read_external_eeprom(add++), LINE2(i));
                }
            } else if (key == SW11 && add > 24) {
                add -= 24;
                for (int i = 0; i < 12; i++) {
                    clcd_putch(read_external_eeprom(add++), LINE2(i));
                }
            }

        }
        clcd_print("     ", LINE2(12));
    } else {
        clcd_print(" NO LOGS        ", LINE1(1));
    }
    if (key == SW12 && key_press == 2) {
        CLEAR_DISP_SCREEN;
        func_flag = 2;
        key_press = 0;
    }

}

void Download_Log() {
    if (Clear_log_flag == 0) {
        static char j = 0;
        static char down_add = 0x00;
        if (j < 10) {
            for (int i = 0; i < 12; i++) {
                put_char(read_external_eeprom(down_add++));
            }
            put_char('\n');
            put_char('\r');
            j++;

        }
    } else {
        clcd_print(" NO LOGS        ", LINE1(1));
    }
    if (key == SW12 && key_press == 2) {
        CLEAR_DISP_SCREEN;
        func_flag = 2;
        key_press = 0;
    }
}

void Clear_log() {
    Clear_log_flag = 1;
    clcd_print("CLEARED LOG   ", LINE1(1));
    if (key == SW12 && key_press == 2) {
        CLEAR_DISP_SCREEN;
        func_flag = 2;
        key_press = 0;
    }


}

void Set_time() {
    int seconds = 0, minutes = 0, hours = 0, field_flag = 1;
    unsigned char s_key = 0xFF;
    unsigned static char set_dummy = 0;
    CLEAR_DISP_SCREEN;
    get_time();
    set_dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, set_dummy | 0x80);
    clcd_print("HH:MM:SS", LINE1(2));
    clcd_print(time, LINE2(2));
    //current time
    hours = (((time[0] - 48)*10) + time[1] - 48);
    minutes = (((time[3] - 48)*10) + time[4] - 48);
    seconds = (((time[6] - 48)*10) + time[7] - 48);
    int delay = 0;
    unsigned long int wait = 0;
    while (1) {
        if (wait++ <= 1000) {
            clcd_putch((seconds / 10) + '0', LINE2(8));
            clcd_putch((seconds % 10) + '0', LINE2(9));
            clcd_putch((minutes / 10) + '0', LINE2(5));
            clcd_putch((minutes % 10) + '0', LINE2(6));
            clcd_putch((hours / 10) + '0', LINE2(2));
            clcd_putch((hours % 10) + '0', LINE2(3));
        }
        else if (wait > 1000 && wait < 2000) {
            if (field_flag == 1) {
                clcd_print("  ", LINE2(8));
            }
            if (field_flag == 2) {
                clcd_print("  ", LINE2(5));
            }
            if (field_flag == 3) {
                clcd_print(" ", LINE2(2));
            }
        } else {
            wait = 0;
        }
        for(int i=100;i--;);
        s_key = read_switches(1);
       
        if (s_key == SW11 && key_press == 2) 
        {
            key_press = 0;
            //save time to eeprom
            write_ds1307(SEC_ADDR, (seconds % 10) | ((seconds / 10) << 4));
            write_ds1307(MIN_ADDR, (minutes % 10) | ((minutes / 10) << 4));
            write_ds1307(HOUR_ADDR, (hours % 10) | ((hours / 10) << 4));
            set_dummy = read_ds1307(SEC_ADDR);

            write_ds1307(SEC_ADDR, set_dummy & 0x7F);
            CLEAR_DISP_SCREEN;
            clcd_print("TIME SET SUCC", LINE1(0));
            __delay_ms(2100);
            func_flag = 2;
            CLEAR_DISP_SCREEN;
            return;
        }
        else if(s_key == SW12 && key_press == 2)
        {
            key_press = 0;
            func_flag =2;
            return;
            
        }
        else if (s_key == SW11) 
        {
            if (field_flag == 1) 
            {
                if (seconds++ == 59) 
                {
                    seconds = 0;
                }
                clcd_putch((seconds / 10) + '0', LINE2(8));
                clcd_putch((seconds % 10) + '0', LINE2(9));
            }
            else if (field_flag == 2)
            {
                if (minutes++ == 59) 
                {
                    minutes = 0;
                }
                clcd_putch((minutes / 10) + '0', LINE2(5));
                clcd_putch((minutes % 10) + '0', LINE2(6));
            }
            else if (field_flag == 3) 
            {
               if (hours++ == 23) {
                    hours = 0;
                }
                clcd_putch((hours / 10) + '0', LINE2(2));
                clcd_putch((hours % 10) + '0', LINE2(3));
            }
        }
        else if (s_key == SW12)
        {
            if (field_flag < 3)
                field_flag++;
            else
                field_flag = 1;
        }
    }
}

void Change_password()
{

    unsigned static char change_flag = 0;
    unsigned static new_pass_buff[4];
    unsigned static char chn_pass_cnt = 0;
    if (change_flag == 0)
    {
        clcd_print("  NEW PASSWORD  ", LINE1(0));
        if (key == SW11)
        {
            clcd_print("*", LINE2(chn_pass_cnt + 2));
            new_pass_buff[chn_pass_cnt++] = 1;
        }
        else if (key == SW12 )
        {
            clcd_print("*", LINE2(chn_pass_cnt + 2));
            new_pass_buff[chn_pass_cnt++] = 0;
        }
        if (chn_pass_cnt == 4)
        {
            CLEAR_DISP_SCREEN;
             change_flag = 1;
            chn_pass_cnt = 0;
        }
    }
    else if (change_flag == 1)
    {
        clcd_print("  CNF PASSWORD  ", LINE1(0));
        if (key == SW11)
        {
            if (new_pass_buff[chn_pass_cnt++] != 1)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASS NO MATCH", LINE1(1));
                __delay_ms(2100);
                func_flag = 2;
                return;
            }
            else
            {
                clcd_print("*", LINE2(chn_pass_cnt + 2));
            }
        }
        else if (key == SW12 )
        {
            
            if (new_pass_buff[chn_pass_cnt++] != 0)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASS NO MATCH", LINE1(1));
                __delay_ms(2100);
                func_flag = 2;
                return;
            }
            else
            {
                clcd_print("*", LINE2(chn_pass_cnt + 2));
            }
        }
        if (chn_pass_cnt == 4)
        {
            CLEAR_DISP_SCREEN;
            change_flag = 2;
            chn_pass_cnt = 0;
        }
    }
    else if (change_flag == 2)
    {
        for (char chn = 0; chn < 4; chn++)
        {
            write_external_eeprom(0x82 + chn, new_pass_buff[chn]);
        }
        for (char chn = 0; chn < 4; chn++)
        {
            orgArr[chn] = read_external_eeprom(0x82 + chn);
        }

        CLEAR_DISP_SCREEN;
        clcd_print("PASS CHN SUCSS", LINE1(1));
        __delay_ms(2100);
        CLEAR_DISP_SCREEN;
        func_flag = 0;
        change_flag = 0;
    }




}