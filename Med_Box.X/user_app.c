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

typedef struct {
    int numOfPills;
    int numOfPillsToTake;
    u8 days;
    u8 hour;
    u8 minute;
    u8 second;
} Box;

typedef struct {
    u8 pinMask;
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


/* The 4 buttons used for short and long press */
static Button Btn1;
static Button Btn2;
static Button Btn3;
static Button Btn4;





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


    /*-------  Real Time Clock -------*/
    
    // * Manually set by user at the time of Programming using the CSV
    
    // Monday 8:05:30 PM
    clock.day = 0;
    clock.hour = 20;
    clock.minute = 5;
    clock.second = 30;


    /*-------  Buttons  -------*/
    Btn1.pinMask = 0x01;
    Btn2.pinMask = 0x02;
    Btn3.pinMask = 0x04;
    Btn4.pinMask = 0x08;

    Btn1.state = 0x00;
    Btn2.state = 0x00;
    Btn3.state = 0x00;
    Btn4.state = 0x00;

    Btn1.pressedTimer = 0x00;
    Btn2.pressedTimer = 0x00;
    Btn3.pressedTimer = 0x00;
    Btn4.pressedTimer = 0x00;

    Btn1.releasedTimer = 0x00;
    Btn2.releasedTimer = 0x00;
    Btn3.releasedTimer = 0x00;
    Btn4.releasedTimer = 0x00;

    Btn1.pressed = false;
    Btn2.pressed = false;
    Btn3.pressed = false;
    Btn4.pressed = false;

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
 * Makes LED blink to a certain pattern based on mode (2 modes - Pill Alarm and Pill Refill)

 @Requires:
    - Two parameters: BoxChoice and Mode
    - BoxChoice is a 4 bit value (eg - 1000 means LED 1 is activated
    - Should be called in the main user section after every 1 second is passed to ensure no illegal values exist
 
 @Promises:
    
 */
void LED(u8 BoxChoice, u8 Mode) {
    if (BoxChoice == 1) {
        PORTA = 0x01;
    } else if (BoxChoice == 2) {
        PORTA = 0x02;
    } else if (BoxChoice == 3) {
        PORTA = 0x04;
    } else {
        PORTA = 0x08;
    }
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
 * Don't use this function directly. This is a helper method.
 */
void AddButtonListener(Button* btn, int pressDuration, void (*callback)()) {

    if (btn->pressed) {

        if ((PORTB & btn->pinMask) == 0x00) { // Button either released or bounced
            btn->releasedTimer++;
            if (btn->releasedTimer > 10) { // Button released for 10ms
                btn->pressed = false;
            }
        }

        btn->pressedTimer += 1;

        if (btn->pressedTimer >= pressDuration) { // How long before registers as a button press
            btn->pressed = false;
            callback();
        }

    } else if ((PORTB & btn->pinMask) != btn->state && (PORTB & btn->pinMask) == btn->pinMask) {
        btn->pressed = true;
        btn->pressedTimer = 0;
        btn->releasedTimer = 0;
    }

    btn->state = PORTB & btn->pinMask;
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

    if (btnChoice == 1) {
        AddButtonListener(&Btn1, pressDuration, callback);
    } else if (btnChoice == 2) {
        AddButtonListener(&Btn2, pressDuration, callback);
    } else if (btnChoice == 3) {
        AddButtonListener(&Btn3, pressDuration, callback);
    } else if (btnChoice == 4) {
        AddButtonListener(&Btn4, pressDuration, callback);
    }
}

/*!----------------------------------------------------------------------------------------------------------------------
 * 
 @brief Application code that runs once per system loop
 * 
 *!----------------------------------------------------------------------------------------------------------------------
 */

void UserAppRun(void) {

    UpdateClock();

    // initalize medicine time data here
//    static u8 u8Hour = 17; // enter start time details
//    static u8 u8Minutes = 34; // enter start time details
//    static u8 u8Seconds = 0; // enter start time details
//    static u8 u8days = 0b01111111; // bits showing which day the current day is. Bit 8 is unused and bit 7 is Sunday
//    static u8 u8PillIndex = 0; // get current time by accessing all the time variables
//    static u8 u8A_user_dat[2][7] = {// box number, set initial total pills, pills left, pills to be taken, hour, minute, day
//        {3, 50, 48, 2, 17, 35, 0b01001000}
//    }; // sample user data
//    if (((u8A_user_dat[u8PillIndex][6] & u8days) != 0)&&(u8A_user_dat[u8PillIndex][4] == u8Hour)&&(u8A_user_dat[u8PillIndex][5] == u8Minutes)) {// check current time against the current pill day and time
//        LED(u8A_user_dat[u8PillIndex][0]); // Test command//call corresponding LED in the right mode
//        // call alarm in the right mode
//        // call  to ensure time increments are not missed
//        // keep doing the above till snooze button is pressed
//    }
//    // call refill function and receive return value - return function is being worked on by Atharva
//    // send return value of refill to LED and speaker function


}




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
