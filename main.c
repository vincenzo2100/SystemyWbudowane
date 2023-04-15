
/*******************************************************************************
Explorer 16 Demo Main File

Company:
Microchip Technology Inc.

File Name:
main.c

Summary:
Explorer 16 Demo Main File.

Description:
This is the main file for the Explorer 16 Demo. It contains the main demo
flow and calls processor specific files for processor specific
implementations.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc. All rights reserved.


Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).


You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.


SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF

MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF

SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "app.h"

#define ONE_VOLT 310
#define ONE_TENTH_VOLT 31
#define ONE_HUNDREDTH_VOLT 3

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************


void Update_LCD ( void ) ;
void SYS_Initialize ( void ) ;
extern void ConvertADCVoltage ( unsigned int ) ;
extern void Hex2Dec ( unsigned char ) ;


APP_DATA appData =
{
    .messageLine1 = "Explorer 16 Dexd",
    .messageLine2 = "Press S3 to cont",
    .messageTime = "Time 00: 00: 00 ",
    .messageADC = " Pot = 0.00 Vdc "
} ;


void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;

    for (i = 0; i < ms; i++)
    {

        for (j = 0; j < 200; j++)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
        }
    }
}


void prog1()
{
    for (int i = 0;i<=256;i++)
{
LATA=i;
}
}


void prog2()
{
    for (int i=256; i>0; i--)
{
LATA = i;
} 
}


void prog3()
{
   for (int i=0; i<256; i++)
{
LATA = i ^ (i >> 1);

} 
}

void prog4()
{
    for (int i=256; i>0; i--)
{
LATA = i ^ (i >> 1);

} 
}
void prog5()
{
    unsigned char ones = 0;
    unsigned char tens = 0;

    while (1) {
       
        LATA = ones | (tens << 4);

        ones++;
        if (ones > 9) {
            ones = 0;
            tens++;
        }
        if (tens > 9) {
            ones = 0;
            tens = 0;
        }
    }
}
void prog6()
{
     unsigned char ones = 9;
    unsigned char tens = 9;

    while (1) {
        
        LATA = ones | (tens << 4);

        
        if (ones == 0 && tens == 0) {
            ones = 9;
            tens = 9;
        } else if (ones == 0) {
            ones = 9;
            tens--;
        } else {
            ones--;
        }
    }
}
void prog7()
{
    for (int i = 2; i < 8; i++)
{
LED_On (i+1);
LED_On (i);
LED_On (i-1);
delay(500);
LED_Off(i-1);
}
for (int i = 7; i>=2; i--)
{
LED_On (i+1);
LED_On (i);
LED_On (i-1);
delay(500);
LED_Off(i+1);
}
}

void prog8()
{
    for (int j = 9; j >= 0; j--)
{
for (int i = 0; i < j; i++)
{
LED_On (i);
delay(100);
if (i != j - 1)
{
LED_Off (i);
}
}
}
}

void generate_random(unsigned char current_value) {
   
    unsigned char bit1 = current_value & 0x01;
    unsigned char bit2 = current_value & 0x02;
    unsigned char bit3 = current_value & 0x04;
    unsigned char bit4 = current_value & 0x10;
    unsigned char bit5 = current_value & 0x20;
    unsigned char bit6 = current_value & 0x40;

    
    unsigned char new_value = (bit1 << 5) | (bit2 << 3) | (bit3 << 1) |
                              (bit4 >> 3) | (bit5 >> 5) | (bit6 >> 6);

   
    LATA = new_value;
}
void prog9()
{
    
    unsigned char current_value = 0x3F;

   
    for (int i = 0; i < 10; i++) {
        generate_random(current_value);
        current_value = LATA;
        __delay_ms(1000);
    }
}


void r_program(int running_prog)
{
    switch(running_prog)
    {
    case 1:
        prog1();
        break;
    case 2:
        prog2();
        break;
    case 3:
        prog3();
        break;
    case 4:
        prog4();
        break;
    case 5:
        prog5();
        break;
    case 6:
        prog6();
        break;
    case 7:
        prog7();
        break;
    case 8:
        prog8();
        break;
    case 9:
        prog9();
        break;
    default:
        prog1();
        break;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    SYS_Initialize ( ) ;

    ADC_SetConfiguration ( ADC_CONFIGURATION_AUTO_SAMPLE_CONVERT ) ;

    int running_prog = 1;


    /* Infinite Loop */
    while ( 1 )
    {
        delay(500);
        if(BUTTON_IsPressed(BUTTON_S3))
        {
            running_prog++;
        }
        else if (BUTTON_IsPressed(BUTTON_S4))
        {
            running_prog--;
        }
        if(running_prog > 9)
        {
            running_prog = 1;
        }
        else if (running_prog < 1)
        {
            running_prog = 9;
        }

        LCD_ClearScreen();
        char str[20];
        sprintf(str, "program %d", running_prog);
        LCD_PutString(str, sizeof(str));

        r_program(running_prog);
        LATA = 0;
//LED_On(LED_D10);
        delay(200);
    } ;
}
/*******************************************************************************

Function:
void Update_LCD( void )

Summary:
Function to update LCD

Description:
This function will update the time on the LCD

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/
void Update_LCD ( void )
{
    LCD_PutChar ( 'T' ) ;
    LCD_PutChar ( 'i' ) ;
    LCD_PutChar ( 'm' ) ;
    LCD_PutChar ( 'e' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

    Hex2Dec ( appData.minutes ) ;
    LCD_PutChar ( ':' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

    Hex2Dec ( appData.seconds ) ;
    LCD_PutChar ( ':' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

}

/*******************************************************************************

Function:
void ConvertADCVoltage ( unsigned int )

Summary:
Function to convert ADC data into volts

Description:
This is the file for the Explorer 16 Dem that converts raw 10 bit ADC data
to volts suitable for the LCD display

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/

void ConvertADCVoltage ( unsigned int adc_conv_data )
{
    /* reset values */
    appData.adones = 0 ;
    appData.adtens = 0 ;
    appData.adhunds = 0 ;
    appData.adthous = 0 ;

    while ( adc_conv_data > 0 )
    {
        /* test for 1 volt or greater */
        if ( adc_conv_data > ( ONE_VOLT - 1 ) )
        {
            /* increment 1 volt counter */
            appData.adones++ ;

            /* subtract 1 volt */
            adc_conv_data -= ONE_VOLT ;
        }


        /* test for 0.1 volt */
        else if ( adc_conv_data > ( ONE_TENTH_VOLT - 1 ) )
        {
            /* increment tenths */
            if ( appData.adtens < 9 )
            {
                appData.adtens++ ;
            }
            else
            {
                /* tenths has rolled over */
                appData.adones++ ;

                /* so increment ones and reset tenths */
                appData.adtens = 0 ;
            }

            adc_conv_data -= ONE_TENTH_VOLT ;
        }

        /* test for 0.01 volt */
        else if ( adc_conv_data > ( ONE_HUNDREDTH_VOLT - 1 ) )
        {
            /* increment hundredths */
            if ( appData.adhunds < 9 )
            {
                appData.adhunds++ ;
            }
            else
            {
                /* hundredths has rolled over */
                appData.adtens++ ;

                /* so increment tenths and reset hundredths */
                appData.adhunds = 0 ;
            }

            adc_conv_data -= ONE_HUNDREDTH_VOLT ;
        }

        else if ( adc_conv_data <= ( ONE_HUNDREDTH_VOLT - 1 ) )
        {
            appData.adthous++ ;
            adc_conv_data -- ;
        }
    }

    appData.adones += 0x30 ;
    appData.adtens += 0x30 ;
    appData.adhunds += 0x30 ;
    appData.adthous += 0x30 ;
}

/*******************************************************************************

Function:
void Hex2Dec ( unsigned char )

Summary:
Explorer 16 Demo Hex to Decimal Conversion File

Description:
This is the file for the Explorer 16 Dem that converts the hexadecimal data
into decimal format.

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/

void Hex2Dec ( unsigned char count )
{
    /* reset values */
    appData.hunds = 0 ;
    appData.tens = 0 ;
    appData.ones = 0 ;

    while ( count >= 10 )
    {

        if ( count >= 200 )
        {
            count -= 200 ;
            appData.hunds = 0x02 ;
        }

        if (count >= 100)
        {
            count -= 100 ;
            appData.hunds++ ;
        }

        if (count >= 10 )
        {
            count -= 10 ;
            appData.tens++ ;
        }
    }

    appData.ones = count ;
}













/*******************************************************************************
Explorer 16 Demo Main File

Company:
Microchip Technology Inc.

File Name:
main.c

Summary:
Explorer 16 Demo Main File.

Description:
This is the main file for the Explorer 16 Demo. It contains the main demo
flow and calls processor specific files for processor specific
implementations.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc. All rights reserved.


Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for

additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END


// ***************************************************************************
// ***************************************************************************
// Section: Included Files
// ***************************************************************************
// ***************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "app.h"

#define ONE_VOLT 310
#define ONE_TENTH_VOLT 31
#define ONE_HUNDREDTH_VOLT 3

// ***************************************************************************
// ***************************************************************************
// Section: File Scope Variables and Functions
// ***************************************************************************
// ***************************************************************************


void Update_LCD ( void ) ;
void SYS_Initialize ( void ) ;
extern void ConvertADCVoltage ( unsigned int ) ;
extern void Hex2Dec ( unsigned char ) ;


APP_DATA appData =
{
    .messageLine1 = "Explorer 16 Demo",
    .messageLine2 = "Press S3 to cont",
    .messageTime = "Time 00: 00: 00 ",
    .messageADC = " Pot = 0.00 Vdc "
} ;

void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;

    for (i = 0; i < ms; i++)
    {

        for (j = 0; j < 200; j++)
        {
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
        }
    }
}

void turnOnLedByNymber(int x)
{
    switch(x)
    {
    case 0:
        LED_On(LED_D3);
        LATA += 0b00000001;
        break;
    case 1:
        LED_On(LED_D4);
        LATA += 0b00000010;
        break;
    case 2:
        LED_On(LED_D5);
        LATA += 0b00000100;
        break;
    case 3:
        LED_On(LED_D6);
        LATA += 0b00001000;
        break;
    case 4:
        LED_On(LED_D7);
        LATA += 0b00010000;
        break;
    case 5:
        LED_On(LED_D8);
        break;
    case 6:
        LED_On(LED_D9);
        LATA += 0b00100000;
        break;
    case 7:
        LED_On(LED_D10);
        LATA += 0b01000000;
        break;
    }
}

void turnOffLedByNymber(int x)
{
    switch(x)
    {
    case 0:
        LED_Off(LED_D3);
        break;
    case 1:
        LED_Off(LED_D4);
        break;
    case 2:
        LED_Off(LED_D5);
        break;
    case 3:
        LED_Off(LED_D6);
        break;
    case 4:
        LED_Off(LED_D7);
        break;
    case 5:
        LED_Off(LED_D8);
        break;
    case 6:
        LED_Off(LED_D9);
        break;
    case 7:
        LED_Off(LED_D10);
        break;
    }
}

void printBinary(int x)
{
    LED_On(LED_D10);
    for(int i = 0; i < 8; i++)
    {
//turnOffLedByNymber(i);
    }

    int current = 0;
    for (int i = 0; i < 256 ; i >>=1)
    {
        if(x & i)
        {
            turnOnLedByNymber(current);
            printf("1");
        }
        else
        {
            printf("0");
        }
        current++;
    }
}

void program1()
{
    for(unsigned int i = 0; i < 256; i++)
    {
        LATA = i;
        delay(200);
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            break;
        }
    }

}

void program2()
{
    for (unsigned int i = 256; i >= 0; i--)
    {
        LATA = i;
        delay(200);
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            break;
        }
    }
}

void program3()
{
    unsigned int count = 0;
    unsigned int gray_count = 1;

    while (count < 256)
    {
        LATA = count;
        count++;
        if (count == 0)
        {
            gray_count = 0;
        }
        else
        {
            int n = count;
            gray_count = n ^ (n >> 1);
        }
        delay(200);
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            break;
        }
    }
}

void program4()
{
    unsigned int count = 255;
    unsigned int gray_count = 1;

    while (count > 0)
    {
        LATA = count;
        count--;
        if (count == 0)
        {
            gray_count = 0;
        }
        else
        {
            int n = count;
            gray_count = n ^ (n >> 1);
        }
        delay(200);
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            break;
        }
    }
}

void program5()
{
    unsigned char tens = 0;
    unsigned char units = 0;
    while (units < 99)
    {
        units++;
        if (units >= 10)
        {
            units = 0;
            tens++;
            if (tens >= 10)
            {
                tens = 0;
                units = 0;
            }
        }
        delay(200);
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            break;
        }
    }
}

void program6()
{
    delay(200);
    if(BUTTON_IsPressed(BUTTON_S5))
    {
        break;
    }
}

void program7()
{
    LATA = 111; // pocz?tkowa pozycja w??a
    bool direction_right = true; // kierunek poruszania si? w??a
    unsigned int led_position = 0b00000111; // pocz?tkowa pozycja diod LED

    while(1)
    {
//LATA = (LATA << (7-3)) & led_position; // wy?wietlenie aktualnej pozycji w??a
        delay(200);

//if(direction_right) { // w?? porusza si? w prawo
// LATA <<= 1; // przesuni?cie pozycji w??a o jedn? pozycj? w lewo
//if(LATA == 111000) { // je?li w?? osi?gn?? kraw?d?, zmiana kierunku
// direction_right = false;
// }
//} else { // w?? porusza si? w lewo
// LATA >>= 1; // przesuni?cie pozycji w??a o jedn? pozycj? w prawo
// if(LATA == 111) { // je?li w?? osi?gn?? kraw?d?, zmiana kierunku
// direction_right = true;
// }
// }

        if(BUTTON_IsPressed(BUTTON_S5))   // przerwanie programu, gdy wci?ni?to przycisk S5
        {
            break;
        }
    }
}

void program8()
{




}


void runProgram(int program)
{
    switch (program)
    {
    case 0:
        program1();
        break;
    case 1:
        program2();
        break;
    case 2:
        program3();
        break;
    case 3:
        program4();
        break;
    case 4:
        program5();
        break;
    case 5:
        program6();
        break;
    case 6:
        program7();
        break;
    case 7:
        program8();
        break;
    default:
        program1();
        break;
    }
}

// ***************************************************************************
// ***************************************************************************
// Section: Main Entry Point
// ***************************************************************************
// ***************************************************************************
int main ( void )
{


    /* Call the System Intialize routine*/
    SYS_Initialize ( ) ;

    /* Display welcome message */
// LCD_PutString ( (char*) &appData.messageLine1[0] , sizeof (appData.messageLine1 ) - 1 ) ;
// LCD_PutString ( (char*) &appData.messageLine2[0] , sizeof (appData.messageLine2 ) - 1 ) ;

    /* wait here until switch S3 is pressed */
//while (!BUTTON_IsPressed ( BUTTON_S3 )) ;

    /*Initialize Timer*/
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_RTCC ) ;

    /*Initialize ADC configure it as 10 bit interrupt driven mode*/
    ADC_SetConfiguration ( ADC_CONFIGURATION_AUTO_SAMPLE_CONVERT ) ;

    /* Display initial ADC and Timer messages */
// LCD_PutString ( (char*) &appData.messageTime[0] , sizeof (appData.messageTime ) - 1 ) ;
// LCD_PutString ( (char*) &appData.messageADC[0] , sizeof (appData.messageADC ) - 1 ) ;


    int currentProgram = 0;
    LED_On(LED_D10);
    /* Infinite Loop */
    while ( 1 )
    {
// LCD_PutString ( (char*) ("program " + currentProgram) , sizeof (appData.messageADC ) ) ;
        if(BUTTON_IsPressed(BUTTON_S3))
        {
            currentProgram++;
        }
        else if (BUTTON_IsPressed(BUTTON_S4))
        {
            currentProgram--;
        }
        if(currentProgram > 9)
        {
            currentProgram = 0;
        }
        else if (currentProgram < 0)
        {
            currentProgram = 9;
        }

        LCD_ClearScreen();
        char str[20];
        sprintf(str, "program %d", currentProgram);
        LCD_PutString(str, sizeof(str));

        runProgram(currentProgram);
        LATA = 0;
//LED_On(LED_D10);
        delay(200);
    }
} ;
/*******************************************************************************

Function:
void Update_LCD( void )

Summary:
Function to update LCD

Description:
This function will update the time on the LCD

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/
void Update_LCD ( void )
{
    LCD_PutChar ( 'T' ) ;
    LCD_PutChar ( 'i' ) ;
    LCD_PutChar ( 'm' ) ;
    LCD_PutChar ( 'e' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

    Hex2Dec ( appData.minutes ) ;
    LCD_PutChar ( ':' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

    Hex2Dec ( appData.seconds ) ;
    LCD_PutChar ( ':' ) ;
    LCD_PutChar ( ' ' ) ;
    LCD_PutChar ( appData.tens + 0x30 ) ;
    LCD_PutChar ( appData.ones + 0x30 ) ;

}

/*******************************************************************************

Function:
void ConvertADCVoltage ( unsigned int )

Summary:
Function to convert ADC data into volts

Description:
This is the file for the Explorer 16 Dem that converts raw 10 bit ADC data
to volts suitable for the LCD display

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/

void ConvertADCVoltage ( unsigned int adc_conv_data )
{
    /* reset values */
    appData.adones = 0 ;
    appData.adtens = 0 ;
    appData.adhunds = 0 ;
    appData.adthous = 0 ;

    while ( adc_conv_data > 0 )
    {
        /* test for 1 volt or greater */
        if ( adc_conv_data > ( ONE_VOLT - 1 ) )
        {
            /* increment 1 volt counter */
            appData.adones++ ;

            /* subtract 1 volt */
            adc_conv_data -= ONE_VOLT ;
        }


        /* test for 0.1 volt */
        else if ( adc_conv_data > ( ONE_TENTH_VOLT - 1 ) )
        {
            /* increment tenths */
            if ( appData.adtens < 9 )
            {
                appData.adtens++ ;
            }
            else
            {
                /* tenths has rolled over */
                appData.adones++ ;

                /* so increment ones and reset tenths */
                appData.adtens = 0 ;
            }

            adc_conv_data -= ONE_TENTH_VOLT ;
        }

        /* test for 0.01 volt */
        else if ( adc_conv_data > ( ONE_HUNDREDTH_VOLT - 1 ) )
        {
            /* increment hundredths */
            if ( appData.adhunds < 9 )
            {
                appData.adhunds++ ;
            }
            else
            {
                /* hundredths has rolled over */
                appData.adtens++ ;

                /* so increment tenths and reset hundredths */
                appData.adhunds = 0 ;
            }

            adc_conv_data -= ONE_HUNDREDTH_VOLT ;
        }

        else if ( adc_conv_data <= ( ONE_HUNDREDTH_VOLT - 1 ) )
        {
            appData.adthous++ ;
            adc_conv_data -- ;
        }
    }

    appData.adones += 0x30 ;
    appData.adtens += 0x30 ;
    appData.adhunds += 0x30 ;
    appData.adthous += 0x30 ;
}

/*******************************************************************************

Function:
void Hex2Dec ( unsigned char )

Summary:
Explorer 16 Demo Hex to Decimal Conversion File

Description:
This is the file for the Explorer 16 Dem that converts the hexadecimal data
into decimal format.

Precondition:
None.

Parameters:
None.

Returns:
None.

Remarks:

*/
/******************************************************************************/

void Hex2Dec ( unsigned char count )
{
    /* reset values */
    appData.hunds = 0 ;
    appData.tens = 0 ;
    appData.ones = 0 ;

    while ( count >= 10 )
    {

        if ( count >= 200 )
        {
            count -= 200 ;
            appData.hunds = 0x02 ;
        }

        if (count >= 100)
        {
            count -= 100 ;
            appData.hunds++ ;
        }

        if (count >= 10 )
        {
            count -= 10 ;
            appData.tens++ ;
        }
    }

    appData.ones = count ;
}
