#include "mbed.h"
#include "rtos.h"

#include "C12832.h"

#define LM75B_ADDR 0x90

#define LM_75B_PTR_CONF 0x01
#define LM_75B_PTR_TEMP 0x00

// LCD display on App shied
C12832 LCD(PB_5, PA_5, PA_6, PF_13, PD_14);

I2C lm75b(I2C_SDA, I2C_SCL);

// main() runs in its own thread in the OS
int main()
{
    char Msg_Config[2] = {LM_75B_PTR_CONF, 0x00};
    char Msg_Ptr_Temp[1] = {LM_75B_PTR_TEMP};
    char Msg_Lu[2];
    volatile char erreur;
    volatile short temp_code;
    volatile float temp_deg;

    volatile short tmp1, tmp2;

    // configuration
    erreur = lm75b.write(LM75B_ADDR, Msg_Config, 2);

    while(1)
    {
       erreur = lm75b.write(LM75B_ADDR, Msg_Ptr_Temp, 1);
        erreur = lm75b.read(LM75B_ADDR, Msg_Lu, 2);

        tmp1 = ((short)Msg_Lu[0]) << 8;
        tmp2 = ((short)Msg_Lu[1]) & 0x00FF;
        temp_code = tmp1 + tmp2;
        temp_deg = (float)temp_code * 0.125 / 32.0;

        LCD.locate(0, 0);
        LCD.printf("Temperature lue %.1f Deg", temp_deg);

        ThisThread::sleep_for(1000ms);
    }
}
