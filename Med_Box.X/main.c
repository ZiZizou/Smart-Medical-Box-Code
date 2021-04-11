/*!**********************************************************************************************************************
@file main.c                                                                
@brief Main system file for the ENCM 369 firmware.  
***********************************************************************************************************************/ 

#include "mcc_generated_files/mcc.h"

#include "configuration.h"


/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32SystemTime1ms = 0;     /*!< @brief Global system time incremented every ms, max 2^32 (~49 days) */
volatile u32 G_u32SystemTime1s  = 0;     /*!< @brief Global system time incremented every second, max 2^32 (~136 years) */
volatile u32 G_u32SystemFlags   = 0;     /*!< @brief Global system flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */

//void TimeXus(u16 u16MicroSeconds);
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/


/*!**********************************************************************************************************************
@fn void main(void)
@brief Main program where all tasks are initialized and executed.


***********************************************************************************************************************/


void main(void)
{
    
    G_u32SystemFlags |= _SYSTEM_INITIALIZING;
    
    /***************** MCC Initialization *****************/
    SYSTEM_Initialize();
    
    
    /* Low level initialization */
    ClockSetup();
    SysTickSetup();
    GpioSetup();
    
    


    /* Application initialization */
    UserAppInitialize();
    
//    Boxes[3].numOfPills = 50;
//    Boxes[3].numOfPillsToTake = 50;
//    Boxes[3].minute = 5;
//    Boxes[3].second = 35;
//    Boxes[3].hour = 20;
//    
//    u8 Box4Days[] = {1, 0, 1, 0, 1, 0, 0}; // Monday, Wed, Fri
//    memcpy(Boxes[3].days, Box4Days, 7);


    /* Super loop */
    u16 counter = 0;

    while(1)
    {

      /* Applications */
      UserAppRun();


      /* System sleep */
      SystemSleep();

      TimeXus(1000);
      while((PIR3 & 0x80) == 0x00); // *** 1 ms Delay ***

      counter++;
      if(counter == 1000){
          LATA ^= 0x80; // Toggle Heartbeat LED
          counter = 0;
      }

    }
  
}




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
