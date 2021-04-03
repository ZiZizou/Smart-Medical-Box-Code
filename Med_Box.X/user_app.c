/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"
#define _XTAL_FREQ 16000000                  // Fosc  frequency for _delay()  library

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
static u8 UserApp_au8sinTable[]=
{
0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,
0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4,0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,
0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,
0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc,0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd,0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,
0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9,0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,
0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,
0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28,
0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x14,0x12,0x11,0x0f,0x0e,0x0c,0x0b,
0x0a,0x09,0x07,0x06,0x05,0x05,0x04,0x03,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x03,0x04,0x05,0x05,0x06,0x07,0x09,
0x0a,0x0b,0x0c,0x0e,0x0f,0x11,0x12,0x14,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36,0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,
0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c,
};
/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
 * @fn void TimeCheck(void)

@brief
 * Takes the time variables as input (u8Hours, u8minutes and u8seconds). 
 * Checks if they have reached invalid values and changes them accordingly

Should be called in the main user section after every 1 second is passed to ensure
 * no illegal values exist

Requires:
- NONE

Promises:
- NONE

*/

void TimeCheck(u8* u8H, u8* u8M, u8* u8S, u8* u8D){
    *u8S+=1;
    if(*u8S>=60){
        *u8S-=60;
        *u8M+=1;
    }
    if(*u8M>=60){
        *u8M-=60;
        *u8H+=1;
    }
    if(*u8H>=24){
        *u8H=0;
        *u8D*=2;
        if(*u8D==0b10000000){
            *u8D=0b00000001;
        }
    }
}

/*!--------------------------------------------------------------------------------------------------------------------
 * @fn void LED(void)

@brief
 * Takes two parameters - BoxChoice and Mode
 * BoxChoice is a 4 bit value (eg - 1000 means LED 1 is activated
 * Makes LED blink to a certain pattern based on mode (2 modes - Pill Alarm and Pill Refill)

Should be called in the main user section after every 1 second is passed to ensure
 * no illegal values exist

Requires:
- NONE

Promises:
- NONE

*/

void LED(u8 BoxChoice, u8 Mode){
    if(BoxChoice==1){
        PORTA = 0x01;
    }
    else if(BoxChoice==2){
        PORTA = 0x02;
    }
    else if(BoxChoice==3){
        PORTA = 0x04;
    }
    else{
        PORTA = 0x08;
    }
}

void ALARM(u8 BoxChoice,u8 Mode)
{
    //if pill is over
    static u8 i=0;
    for(i=0;i<255;i++)
    {
        DAC1DATL=UserApp_au8sinTable[i]; // figuring out pic7 , so usong this so far.
        __delay_ms(1000);// found the timer module a bit complicated
        if(Mode==0)
        {
           break; 
        }
    }
}

/*!--------------------------------------------------------------------------------------------------------------------
 *  * @fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserAppInitialize(void)
{

    T0CON0 = 0x90;
    T0CON1 = 0x54;
    DAC1DATL= 0;

} /* end UserAppInitialize() */

/*--------------------------------------------------------------------
void TimeXus(INPUT_PARAMETER_)
Sets Timer0 to count u16Microseconds_
Requires:
- Timer0 configured such that each timer tick is 1 microsecond
- INPUT_PARAMETER_ is the value in microseconds to time from 1 to 65535
Promises:
- Pre-loads TMR0H:L to clock out desired period
- TMR0IF cleared
- Timer0 enabled
*/
void TimeXus(u16 u16Time)
{
/* OPTIONAL: range check and handle edge cases */
    T0CON0 &= 0x7F;/* Disable the timer during config */
    u16 u16MicroSeconds = 0xFFFF - u16Time;
    TMR0H = (u8)(u16MicroSeconds>>8); /* Preload TMR0H and TMR0L based on u16TimeXus */
    TMR0L = (u8)(u16MicroSeconds&0x00FF);
    PIR3 &= 0x7F;/* Clear TMR0IF and enable Timer 0 */       
    T0CON0 |= 0x80;
} /* end TimeXus () */

/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UserAppRun(void)
{
    
    //initalize medicine time data here
    static u8 u8Hour = 17;//enter start time details
    static u8 u8Minutes =  34;//enter start time details
    static u8 u8Seconds = 0;//enter start time details
    static u8 u8days = 0b01111111;//bits showing which day the current day is. Bit 8 is unused and bit 7 is Sunday
    static u8 u8PillIndex = 0;//get current time by accessing all the time variables
    static u8 u8A_user_dat[2][7] = {//box number, set initial total pills, pills left, pills to be taken, hour, minute, day
        {3,50,48,2,17,35,0b01001000}
    };//sample user data
    if(((u8A_user_dat[u8PillIndex][6]&u8days)!=0)&&(u8A_user_dat[u8PillIndex][4]==u8Hour)&&(u8A_user_dat[u8PillIndex][5]==u8Minutes)){//check current time against the current pill day and time
        LED(u8A_user_dat[u8PillIndex][0]);//Test command//call corresponding LED in the right mode
       // ALARM(  ,)                          //call alarm in the right mode
                                            //call  to ensure time increments are not missed
                                            //keep doing the above till snooze button is pressed
    }
    //call refill function and receive return value - return function is being worked on by Atharva
    //send return value of refill to LED and speaker function
    TimeCheck(&u8Hour, &u8Minutes, &u8Seconds, &u8days);
    //end 
} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
