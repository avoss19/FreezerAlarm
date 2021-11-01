/*
 * File:   main.c
 * Author: Andrew Voss and Rylee Goellner
 *
 * Created on October 27, 2021, 5:21 PM
 */


#include "CONFIG.h"
#define _XTAL_FREQ 4000000  // PIC12F675 internal freq of 4MHz
#define AN0 1<<0
#define AN1 1<<1
#define AN2 1<<2
#define AN3 1<<3


void InitADC(unsigned char Channel)
{
    ANSEL   = 0X10;
    ANSEL  |= Channel;
    TRISIO  = 0x10;
    TRISIO |= Channel;
    ADCON0  = 0X81;
    CMCON   = 0X07;
    VRCON   = 0X00;
}

unsigned int GetADCValue(unsigned char Channel)
{
    ADCON0 &= 0x03;
            
            switch(Channel)
            {
                case AN0: ADCON0|=0x00; break;
                case AN1: ADCON0|=0x04; break;
                case AN2: ADCON0|=0x08; break;
                case AN3: ADCON0|=0x0c; break;
                
                default: return 0;
            }
            __delay_ms(10);
            GO_nDONE=1;
            while(GO_nDONE);
            return ((ADRESH<<8)+ADRESL);
            
}

void main(void) {
    unsigned int Temp, Battery;
    int buzz = 0;
    
    InitADC(AN0);
    InitADC(AN1);
    
    TRISIO=0;
    
    GP4=0;
    GP5=0;
            
    unsigned int tempCal = 512;
    
    while(1)
    {

        Temp = GetADCValue(AN0);
        Battery = GetADCValue(AN1);
        
        if(Temp > tempCal)
            GP2 = 1;
        else
            GP2 = 0;
        
        if(!buzz && Temp > tempCal){
            for(int i = 0; i < 600; i++){
                __delay_ms(100);
                Temp = GetADCValue(AN0);
                if(Temp < tempCal)
                    break;
            }
            if(Temp > tempCal)
                buzz = 1;
        } else if(!(Temp > tempCal)) {
            buzz = 0;
        }
        
        // 3V => Battery = 64; (2.8/3)*64 = 59.733
        if(Battery < 59)
            GP4 = 1;
        else
            GP4 = 0;
                
        if(buzz)
            GP5 = !GP5;
       
        __delay_ms(10);
    }
    return;
}
