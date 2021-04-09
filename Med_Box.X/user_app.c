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
u8 G_au8UserAppsinTable[] = 
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
0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c   
};
u8 G_au8Notes[] = {E4,NN,G4,NN,A4,NN,A4,NN,
                             E4,NN,G4,NN,B4,NN,A4,NN,
                             E4,NN,G4,NN,A4,NN,A4,NN,
                             A4,NN,E4,NN,E4,NN,B4,NN,
                             A4,NN,G4,NN,A4,NN,G4,NN,
                             B4,NN,C4,NN,C4,NN,G4,NN,
                             B4,NN,C4,NN,C4,NN,G4,NN,
                             B4,NN ,C4,NN,C4,NN,D4,NN,
                             D4,NN,D4,NN,D4,NN,E4,NN,
                             E4,NN ,F,NN,F,NN,G4,NN,A4,NN,
                             G4,NN,A4,NN ,G4,NN ,A4,NN ,G4,NN}; //fefeca a#a
    u8 G_au8NOTEDurations[] = {N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N2,RT,RT,N4,RT,N4,
                                     N4,RT,N4,RT,N4,RT,N4,RT,N4,
                                     RT,N4,RT,N4,RT,RT,N4,RT,N4};
/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 * 24-Hour Military Format Clock
 * 
 * Days:
 *  - Monday:       0
 *  - Tuesday:      1
 *  - Wednesday:    2
 *  - Thursday:     3
 *  - Friday:       4
 *  - Saturday:     5
 *  - Sunday:       6
 * 
 */
typedef struct {
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} Clock;

/*
 * bool days = [Mon, Tue, Wed, Thu, Fri, Sat, Sun]
 * 
 */
typedef struct {
    int numOfPills;
    int numOfPillsToTake;
    u8 days[7];
    u8 hour;
    u8 minute;
    u8 second;
} Box;

typedef struct {
    u8 bitMask;
    u8 state;
    bool flashing;
} LED;

typedef struct {
    u8 bitMask;
    u8 state;
    u32 pressedTimer;
    u32 releasedTimer;
    bool pressed;
} Button;

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
/* Internal Clock to keep track of Day and Time */
static Clock clock;


/* Boxes */
static Box Box1;
static Box Box2;
static Box Box3;
static Box Box4;


/* LEDs */
static LED Led1;
static LED Led2;
static LED Led3;
static LED Led4;


/* Buttons */
static Button Btn1;
static Button Btn2;
static Button Btn3;
static Button Btn4;


static Box Boxes[4];
static LED Leds[4];
static Button Buttons[4];


/* Variable for which alarms are currently active
 * 0: Off
 * 1: On (Solid - Dispense Pill)
 * 2: On (Flashing - Refill Box) 
 */
static u8 alarms[] = {0, 0, 0, 0};



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
void UserAppInitialize(void) {

    /*-------  Timer  -------*/
    DAC1DATL= 0;
    T0CON0 = 0x90;
    T0CON1 = 0x54;


    // TODO: Add code for importing data from SD Card CSV file


    /*-------  Real Time Clock -------*/

    // * Manually set by user at the time of Programming using the CSV

    // Monday 8:05:30 PM
    clock = (Clock){
        .day = 0,
        .hour = 20,
        .minute = 5,
        .second = 30
    };


    /*-------  Boxes  -------*/

    Box1 = (Box){
        .numOfPills = 5,
        .numOfPillsToTake = 1,
        .hour = 20,
        .minute = 5,
        .second = 40
    };

    Box2 = (Box){
        .numOfPills = 10,
        .numOfPillsToTake = 1,
        .hour = 9,
        .minute = 0,
        .second = 30
    };

    Box3 = (Box){
        .numOfPills = 20,
        .numOfPillsToTake = 1,
        .hour = 12,
        .minute = 30,
        .second = 0
    };

    Box4 = (Box){
        .numOfPills = 50,
        .numOfPillsToTake = 50,
        .hour = 20,
        .minute = 5,
        .second = 35
    };

    u8 Box1Days[] = {1, 0, 0, 1, 0, 0, 0}; // Mon, Thu
    u8 Box2Days[] = {0, 0, 0, 0, 1, 0, 0}; // Tue, Fri
    u8 Box3Days[] = {0, 0, 0, 0, 0, 1, 0}; // Sat
    u8 Box4Days[] = {1, 0, 1, 0, 1, 0, 0}; // Monday, Wed, Fri

    memcpy(Box1.days, Box1Days, 7);
    memcpy(Box2.days, Box2Days, 7);
    memcpy(Box3.days, Box3Days, 7);
    memcpy(Box4.days, Box4Days, 7);

    /*-------  LEDs  -------*/

    Led1 = (LED){
        .bitMask = 0b00000001,
        .state = 0,
        .flashing = false
    };

    Led2 = (LED){
        .bitMask = 0b00000010,
        .state = 0,
        .flashing = false
    };

    Led3 = (LED){
        .bitMask = 0b00000100,
        .state = 0,
        .flashing = false
    };

    Led4 = (LED){
        .bitMask = 0b00001000,
        .state = 0,
        .flashing = false
    };


    /*-------  Buttons  -------*/

    Btn1 = (Button){
        .bitMask = 0b00000001,
        .state = 0,
        .pressedTimer = 0,
        .releasedTimer = 0,
        .pressed = false
    };

    Btn2 = (Button){
        .bitMask = 0b00000010,
        .state = 0,
        .pressedTimer = 0,
        .releasedTimer = 0,
        .pressed = false
    };

    Btn3 = (Button){
        .bitMask = 0b00000100,
        .state = 0,
        .pressedTimer = 0,
        .releasedTimer = 0,
        .pressed = false
    };

    Btn4 = (Button){
        .bitMask = 0b00001000,
        .state = 0,
        .pressedTimer = 0,
        .releasedTimer = 0,
        .pressed = false
    };


    Boxes[0] = Box1;
    Boxes[1] = Box2;
    Boxes[2] = Box3;
    Boxes[3] = Box4;

    Leds[0] = Led1;
    Leds[1] = Led2;
    Leds[2] = Led3;
    Leds[3] = Led4;

    Buttons[0] = Btn1;
    Buttons[1] = Btn2;
    Buttons[2] = Btn3;
    Buttons[3] = Btn4;

}
 /* end UserAppInitialize() */

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
void TimeXus(u16 u16Time) {
    // Fosc/4(16Mhz), Async, Prescaler = 1:16
    T0CON1 = 0b01010100;

    // Disable the timer during config
    T0CON0 &= 0x7F;

    // Preload TMR0L and TMR0H based on u16TimeXus
    u16 u16Complement = 0xFFFF - u16Time;

    TMR0L = (u8) (u16Complement & 0x00FF);
    TMR0H = (u8) (u16Complement >> 8);

    // Clear TMR0IF
    PIR3 &= 0x7F;

    // Enable the Timer
    T0CON0 |= 0x80;
}

void TimeXms(u16 u16Time) {
    // Fosc/4(16Mhz), Async, Prescaler = 1:16384
    T0CON1 = 0b01011110;

    // Disable the timer during config
    T0CON0 &= 0x7F;

    // Preload TMR0L and TMR0H based on u16TimeXus
    u16 u16Complement = 0xFFFF - u16Time + 0x0001;

    TMR0H = (u8) (u16Complement >> 8);
    TMR0L = (u8) (u16Complement & 0x00FF);

    // Clear TMR0IF
    PIR3 &= 0x7F;

    // Enable the Timer
    T0CON0 |= 0x80;
}

/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UpdateClock() {
    static u16 millisecondCounter = 0;

    if (millisecondCounter >= 1000) { /* 1 second has passed */
        clock.second += 1;
        millisecondCounter = 0;
        if (clock.second == 60) {
            clock.second = 0;
            clock.minute += 1;
            if (clock.minute == 60) {
                clock.minute = 0;
                clock.hour += 1;
                if (clock.hour == 24) {
                    clock.hour = 0;
                    //need to add code for updating days
                    clock.day = clock.day>>1;
                    if(clock.day==0){//check if we right shifted Sunday into zero or not
                        clock.day = 0b01000000;
                    }//next day after Sunday is Monday so set to that
                }
            }
        }
    }
    millisecondCounter++;
}

/*
 * Makes an LED either Solid or Flashing
 @Requires:
    - BoxChoice: Which LED to turn on
    - flashing: Whether the LED should be flashing or solid
     
 */
void ToggleLED(u8 ledChoice, bool flashing) {
    if (!flashing) {
        Leds[ledChoice].state = 1;
    } else {
        Leds[ledChoice].flashing = true;
    }
}

/*
 * This method will render all 4 of the LEDs according to the 'Leds' array values
 * Either Off, Solid, or Blinking
 */
void RenderLEDs() {
    u8 leds = PORTA & 0b11110000;
    static u16 flashingCounter = 0;
    flashingCounter++;

    for (int i = 0; i < 4; i++) {
        if (Leds[i].flashing) {
            if (flashingCounter >= 250) {
                Leds[i].state ^= 0x01;
            }
        }
        if (Leds[i].state) {
            leds |= Leds[i].bitMask;
        }
    }

    if (flashingCounter >= 250) {
        flashingCounter = 0;
    }

    LATA = leds;
}

/*
 * Don't use this function directly. This is a helper method.
 */
void AddButtonListener(Button* btn, int pressDuration, void (*callback)(), u8 boxChoice) {

    if (btn->pressed) {

        if ((PORTB & btn->bitMask) == 0x00) { // Button either released or bounced
            btn->releasedTimer++;
            if (btn->releasedTimer > 10) { // Button released for 10ms
                btn->pressed = false;
            }
        }

        btn->pressedTimer += 1;

        if (btn->pressedTimer >= pressDuration) { // How long before registers as a button press
            btn->pressed = false;
            callback(boxChoice);
        }

    } else if ((PORTB & btn->bitMask) != btn->state && (PORTB & btn->bitMask) == btn->bitMask) {
        btn->pressed = true;
        btn->pressedTimer = 0;
        btn->releasedTimer = 0;
    }

    btn->state = PORTB & btn->bitMask;
}


void ButtonPress(u8 btnChoice, int pressDuration, void (*callback)()) {
    AddButtonListener(&Buttons[btnChoice], pressDuration, callback, btnChoice);
}

void StartAlarm(u8 boxChoice, u8 mode) {
    
    u8 index=0;
    u32 i=0;
    u32 Count=0;
    u8  u8Index=0;
    u8 Length=92;
    if (mode == 1) { 
        
        // Dispense Pill Alarm
      while(i<4000000) // ~ approx 10 seconds alarm 
        {
            DAC1DATL=G_au8UserAppsinTable[index];
            index+=4;
            TimeXus(11);
            i++;  
        }
        i=0;
        index=0;
  
        alarms[boxChoice] = 1;
        ToggleLED(boxChoice, false);

    } else if (mode == 2) { // Refill Box Alarm

        alarms[boxChoice] = 2;
        ToggleLED(boxChoice, true);
        while(i<4000000)
        {
            if(u32Count==G_au8NOTEDurations[u8Index])
            {
                u8Index++;
                u32Count^=u32Count;
            }
            if(u8Index==Length)
            {
                u8Index=0;
            }
            u32Count++;
            TimerXus(G_au8Notes[u8Index]); 
        }
        i=0;
        

    }

    // TODO: Add code for Alarming speaker
}

void EndAlarm(u8 boxChoice) {

    if (alarms[boxChoice] == 1) {

        Boxes[boxChoice].numOfPills -= Boxes[boxChoice].numOfPillsToTake;

        if (Boxes[boxChoice].numOfPills <= 0) { // Box is empty
            StartAlarm(boxChoice, 2); // Create new alarm for Box Refill and return
            return;
        }

    } else if (alarms[boxChoice] == 2) {
        // TODO: Reset numOfPills from CSV
        Boxes[boxChoice].numOfPills = 20;
    }

    alarms[boxChoice] = 0;
    Leds[boxChoice].state = 0;
    Leds[boxChoice].flashing = false;
    // TODO: Turn off Speaker

}


void UserAppRun(void)
{

    UpdateClock();


    // Check if its time for taking a pill in any of the boxes:
    for (int i = 0; i < 4; i++) {

        if (Boxes[i].days[clock.day] == 1 &&
                Boxes[i].hour == clock.hour &&
                Boxes[i].minute == clock.minute &&
                Boxes[i].second == clock.second)
        {
            StartAlarm(i, 1); // Start the Alarm for Dispensing Pill

        }
    }

    for (u8 i = 0; i < 4; i++) {
        if (alarms[i] == 1) {
            ButtonPress(i, 50, EndAlarm); // Short Press - 50ms
        } else if (alarms[i] == 2) {
            ButtonPress(i, 2000, EndAlarm); // Long Press - 2000ms
        }
    }

    RenderLEDs();

}

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
