/*
 * File:   newmainXC16.c
 * Author: melvinferreira
 *
 * Created on October 10, 2023, 09:37 PM
 */


// PIC24FJ64GA002 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC Oscillator (FRC))
#pragma config SOSCSEL = LPSOSC         // Sec Oscillator Select (Low Power Secondary Oscillator (LPSOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Windowed Watchdog Timer enabled; FWDTEN must be 1)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#ifndef FCY
 // Unless already defined assume 4MHz instruction clock frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define FCY 4000000UL
#endif 

//------------------------------------------------------------------------------
//  HEADER FILES
//------------------------------------------------------------------------------
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pin_functions.h"
#include "lcd.h"


//------------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//------------------------------------------------------------------------------
// Detects a push button with polling. 
// Active low, requires external pullup to Vcc with resistor. I used 3.3k
bool button_push(void) ; 

//------------------------------------------------------------------------------
// ENUMERATIONS
//------------------------------------------------------------------------------
enum state_var {startup, Mainson, Loadshedding,Startup};


int main(void) {
    // set Initial state for FSM
    enum state_var state =  startup ;
    
    while(1) {
        switch(state) {
             case startup: {
             // Do start-up
            // LCD setup
            // Set-up & configure mapping for pins as output
            setupPins();
            // Initialise display
            Lcd8_Init();
            Lcd8_Clear();  
            
            // Digital I/O setup
            // Outputs
            // Red LED
            LATBbits.LATB15 = 0 ;
            TRISBbits.TRISB15 = 0 ;

            // Yellow LED
            LATAbits.LATA1 = 0 ;
            TRISAbits.TRISA1 = 0 ;
            LATAbits.LATA1 = 0 ;

            //Green LED
            LATAbits.LATA2 = 0 ;
            TRISAbits.TRISA2 = 0 ;
            LATAbits.LATA2 = 0 ;

            // Set as input
            TRISBbits.TRISB8 = 1 ; 
            
            // Waste a bit of time to show that we are in startup state
            Lcd8_Set_Cursor(1,0);
            Lcd8_Write_String("Startup state") ;
            __delay_ms(2000) ;
            // Move to next state e.g State 1
            state = Mainson ; }
             break ;
            case Mainson: 
                LATAbits.LATA1 = 0 ;
                LATAbits.LATA2 = 0 ;
                LATBbits.LATB15 = 1 ;
                Lcd8_Clear();
                Lcd8_Set_Cursor(1,0);
                Lcd8_Write_String("Mains on") ;
                while (!button_push()) ;
                state = Loadshedding ; 
                break;
                
            case Loadshedding: 
                LATBbits.LATB15 = 0 ;
                LATAbits.LATA1 = 0 ;
                LATAbits.LATA2 = 1 ;
                Lcd8_Clear();
                Lcd8_Set_Cursor(1,0);
                Lcd8_Write_String("Loadshedding") ;
                while (!button_push()) ;
                state = Startup ; 
                break;
           case Startup: 
                LATBbits.LATB15 = 0 ;
                LATAbits.LATA2 = 0 ;
                LATAbits.LATA1 = 1 ;
                Lcd8_Clear();
                Lcd8_Set_Cursor(1,0);
                Lcd8_Write_String("Start up") ;
                while (!button_push()) ;
                state = startup ; 
                break;
              
            default: 
                Lcd8_Clear();
                Lcd8_Set_Cursor(1,0);
                Lcd8_Write_String("ERROR undefined state (default)") ;
                __delay_ms(5000) ;
                // Go back to startup
                state = startup ; 
                break;
        }
        
    }
    return 0;
}


bool button_push(void){
     if (!PORTBbits.RB8) {
         // Experiment with this value I do not get acceptable results below 120ms using only pull-up resistor.
         // Basically a waiting period to debounce in software "wait" for transient to go away.
         __delay_ms(200) ; 
         return 1 ;
     }
     else
         return 0 ;
}
