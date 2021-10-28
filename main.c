/*
 * File:   main.c
 * Author: regoe
 *
 * Created on October 27, 2021, 5:21 PM
 */


#include "CONFIG.h"
#define _XTAL_FREQ 40000000
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
    unsigned int Temp, Battery,X;
    InitADC(AN0);
    //InitADC(AN1);
    TRISIO=0;
    GP4=0;
    GP5=0;
    X=0;
    int i;
    
    while(1)
    {
        Temp = GetADCValue(AN0);
        //Battery = GetADCValue(AN1);
        if (Temp>512)
        {
          if(X==1)
          {
           GP5=1;   
          }  
          else
          {
              for(i=0; i<7;i++)
              {
                 __delay_ms(100); 
                 Temp=GetADCValue(AN0);
                 if (Temp<512)
                 {
                     break;
                 }
              }
           X=1;
          }
            
        }
        else 
        {
            X=0;
            GP5=0;
        }  
    }
    return;
}
