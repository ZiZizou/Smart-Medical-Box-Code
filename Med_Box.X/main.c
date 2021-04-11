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


FATFS drive;
FIL file;

const char* readCard(){
    UINT actualLength;
    char dataNewLine[] = "\r\n";
    char contents[] = "The number is: ";
    char buffer[8];
    UINT br;
    
    
    if (f_mount(&drive,"0:",1) == FR_OK){  // mount
       
        if (f_open(&file, "data.csv", FA_READ ) == FR_OK) { //Open/Append or Create DATA.TXT file
        
//            f_write(&file, contents , sizeof(contents)-1, &actualLength );     //Write Temperature is:
//            f_write(&file, num , sizeof(num)-1, &actualLength ); //write temperature value            
//            f_write(&file, dataNewLine, sizeof(dataNewLine)-1, &actualLength );  //go to new line            
//                 
            
            if(f_read(&file, buffer, sizeof(buffer), &br) == FR_OK){
                return buffer;
            }
            
                
            f_close(&file);

        }
      
        
      f_mount(0,"0:",0);  //unmount disk
    }
    return buffer;
}


void main(void)
{
    
    G_u32SystemFlags |= _SYSTEM_INITIALIZING;
    
    /***************** MCC Initialization *****************/
    SYSTEM_Initialize();
  
    char* contents = readCard();
    
    /* Low level initialization */
    ClockSetup();
    SysTickSetup();
    GpioSetup();
    
    


    /* Application initialization */
    UserAppInitialize();


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
