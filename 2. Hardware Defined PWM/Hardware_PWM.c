/*
 *
 *
 *  Created on: March 1st, 2023
 *      Author: Brandon Tran
 *
 *
 */



#include <msp430.h>

char LEDstate;                // LED goes RED -> orange -> GREEN -> cyan -> BLUE -> purple -> RED ...

volatile unsigned int red;
volatile unsigned int green;
volatile unsigned int blue;

void LED_Setup();
void TimerB3_Setup();
void TimerB0_Setup();

void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    red = 0;
    green = 999;
    blue = 999;

    LED_Setup();     // Initialize our LEDs
    TimerB3_Setup();  // Initialize Timer
    TimerB0_Setup(); // Initialize Timer

    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0 w/ interrupt
    __no_operation();                         // For debugger
}

void LED_Setup(){
    // Configure Output pins
    //For RED LED
    P6DIR |= BIT0;                          // Set P6.0 to RED output
    P6OUT &= ~BIT0;                         // P6.0 to power-off state
    P6SEL0 |= BIT0;                         // Initialize select pin 1 for P6.0
    P6SEL1 &= ~BIT0;                        // Initialize select pin 0 for P6.0
    P6IE |= BIT0;                           // P6.0 interrupt enabled

    //For GREEN LED
    P6DIR |= BIT1;                          // Set P6.1 to GREEN output
    P6OUT &= ~BIT1;                         // P6.1 to power-off state
    P6SEL0 |= BIT1;                         // Initialize select pin 0 for P6.1
    P6SEL1 &= ~BIT1;                        // Initialize select pin 1 for P6.0
    P6IE |= BIT1;                           // P4.1 interrupt enabled

    //FOR BLUE LED
    P6DIR |= BIT2;                          // Set P6.2 to BLUE output
    P6OUT &= ~BIT2;                         // P6.2 to power-off state
    P6IE |= BIT2;                           // P6.2 interrupt enabled
    P6SEL0 |= BIT2;                         // Initialize select pin 0 for P6.2
    P6SEL1 &= ~BIT2;                        // Initialize select pin 1 for P6.2
}

void TimerB3_Setup(){
    TB3CCR0 = 1000-1;                     // PWM Period
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TB3CCTL3 = OUTMOD_7;                      // CCR3 reset/set

    // initializing the CC registers for initial state red LED on
    TB3CCR1 = red;                             // RED LED off
    TB3CCR2 = green;                                // GREEN LED off
    TB3CCR3 = blue;                                // BLUE LED off
}
void TimerB0_Setup() {
    TB0CCTL0 = CCIE;                          // TB0CCR0 interrupt enabled
    TB0CCR0 = 1;
    TB0CTL = TBSSEL_1 | MC_2 | ID_3 | TBCLR | TBIE;          // ACLK, continuous mode, /8, clear TBR, enable interrupt
}

// TimerB0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    switch(LEDstate){
    case 0 :  //LED is red then orange
            red = 0;                    //Red duty cycle is at 0%
            blue = 999;                 //Blue duty cycle is at 100%
            green--;                    //Green duty cycle is decremented to 0%
            if(green == 0)
                LEDstate = 1; //Move to next case
            break;
        // LED is orange then green
        case 1 :
            blue = 999;                  //Blue duty cycle is at 100%
            green = 0;                   //Green duty cycle is at 0%
            red++;                       //Increase duty cycle for red to 100%
            if(red == 999)
                LEDstate = 2;            //Move to next case
            break;
        // LED is green then cyan
        case 2 :
            red = 999;                   //Red duty cycle is at 100%
            green = 0;                   //Green duty cycle is at 0%
            blue--;                      //Decrease blue duty cycle to 0%
            if(blue == 0)
                LEDstate = 3;            //Move to next case
            break;
        // LED is cyan then blue
        case 3 :
            red = 999;                  //Red duty cycle is at 100%
            blue = 0;                   //Blue duty cycle is at 0%
            green++;                    // Increase green duty cycle to 100%
            if(green == 999)
                LEDstate = 4;           //Move to next case
            break;
        // LED is blue then purple
        case 4 :
            blue = 0;                   //Blue duty cycle is at 0%
            green = 999;                //Green duty cycle is at 100%
            red--;                      //Decrease red duty cycle to 0%
            if(red == 0)
                LEDstate = 5;           //Move to next case
            break;
        // LED is purple then red
        case 5 :
            red = 0;                    //Red duty cycle is at 0%
            green = 999;                //Green duty cycle is at 100%
            blue++;                     //Increase blue duty cycle to 100%
            if(blue == 999)
                LEDstate = 0;           //Move to next case
            break;
        }

    if(TB0R >= 60000)       // Reset timer register value
        TB0R = 1;

      TB3CCR1 = red;                   // Add OFFset to RED LED
      TB3CCR2 = green;                  // Add OFFset to GREEN LED
      TB3CCR3 = blue;                 // Add OFFset to BLUE LED
      TB0CCR0 += 8;       // Increments by 8 to make fading slower
}

