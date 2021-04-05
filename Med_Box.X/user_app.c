/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief The application for the Smart Medical Box Project

 **********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
 ***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags; /*!< @brief Global state flags */


/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms; /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s; /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags; /*!< @brief From main.c */

/***********************************************************************************************************************
 * Type Definitions
 ***********************************************************************************************************************/

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


/***********************************************************************************************************************
 * Global variables limited to this file.
 ***********************************************************************************************************************/


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




/**********************************************************************************************************************
Function Definitions
 **********************************************************************************************************************/

/*
@brief
 * Initializes the application's variables.
 * Should only be called once in main init section.
 */
void UserAppInitialize(void) {

    /*-------  Timer  -------*/
    T0CON0 = 0x90;


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

/*
 * Sets Timer0 to count u16Time in Microseconds

 @Requires:
    - u16Time is the value in microseconds to time from 1 to 65535

 @Promises:
    - Configures T0CON1
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

/*
 * Sets Timer0 to count u16Time in Milliseconds 
 * * Note: This function can only count in multiples of 1.024 ms

 @Requires:
    - u16Time is the value in milliseconds to time from 1 to 65535

 @Promises:
    - Configures T0CON1
    - Pre-loads TMR0H:L to clock out desired period
    - TMR0IF cleared
    - Timer0 enabled
 */
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

/*
 * Updates the Clock by 1 second while checking for overflow

 @Promises:
    - Updates the values of day, hour, minute, seconds stored in 'clock' struct
 * 
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

/*
 * Waits for a button press for the specified duration

 @Requires:
    - u8 btnChoice: Which button to wait for:  1, 2, 3, 4
    - int duration: How long before it registers as a button press in Milliseconds (Note this requires a 1ms delay in main())
    - callback: Pointer to the callback function to be executed after the button is pressed

 @Promises:
    - Will execute the callback function: callback()
 
 @Example:
 
    void ToggleLED() {
        LATA += 0x01;
        if (LATA > 0x90) {
            LATA = 0x80;
        }
    }

    void UserAppRun(void) {
        ButtonPress(1, 500, ToggleLED);      // Short Press
        ButtonPress(3, 3000, ToggleLED);     // Long Press
    }
 
 */

void ButtonPress(u8 btnChoice, int pressDuration, void (*callback)()) {
    AddButtonListener(&Buttons[btnChoice], pressDuration, callback, btnChoice);
}

/*
 * Will start an Alarm by turning on both the LEDs and Speaker

 @Requires:
    - u8 boxChoice: Which Box to start the Alarm for
 *  - u8 mode: 1 = Dispense Pill Alarm, 2 = Refill Box Alarm
 * 
 */

void StartAlarm(u8 boxChoice, u8 mode) {

    if (mode == 1) { // Dispense Pill Alarm

        alarms[boxChoice] = 1;
        ToggleLED(boxChoice, false);

    } else if (mode == 2) { // Refill Box Alarm

        alarms[boxChoice] = 2;
        ToggleLED(boxChoice, true);

    }

    // TODO: Add code for Alarming speaker
}

/*
 * Will end an Alarm that's already running. It will automatically start a new Refill alarm if the box is empty

 @Requires:
    - u8 boxChoice: Which Box to stop the Alarm for
 * 
 */
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

/*!----------------------------------------------------------------------------------------------------------------------
 * 
 @brief Application code that runs once per system loop
 * 
 *!----------------------------------------------------------------------------------------------------------------------
 */

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




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
