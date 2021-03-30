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
        LED(u8A_user_dat[u8PillIndex][0]);  //Test command//call corresponding LED in the right mode
                                            //call alarm in the right mode
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
