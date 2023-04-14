/*
 * Lab 4 Part 3
 *
 * Author: Brandon tran
 * Version 4/13/2023
 *
 *
 *
 *
 */
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT
        // Configure P6.0 as Output
        P6DIR |= BIT0;                          // Set P6.0 to output direction
        P6OUT &= ~BIT0;                         // Clear P6.0 output latch for a defined power-on state

        // Configure Button on P2.3 as input with pullup resistor
        P2DIR &= ~BIT3;                                                 //Set P2.3 as input
        P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
        P2REN |= BIT3;                          // P2.3 pull-up register enable
        P2IE |= BIT3;                           // P2.3 interrupt enabled

        // Configure Button on P4.1 as input with pullup resistor
        P4OUT &= ~BIT1;                                                 // Set P4.1 as input
        P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
        P4REN |= BIT1;                          // P4.1 pull-up register enable
        P4IE |= BIT1;                           // P4.1 interrupt enabled


    PM5CTL0 &= ~LOCKLPM5;                                               // Disable the GPIO power-on default high-impedance mode to activate


    // Configure Timer
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBIE;              // SMCLK, UP mode
    TB0CCTL1 |= CCIE;                                    // Enable TB0 CCR1 Interrupt
    TB0CCTL2 |= CCIE;                                    // Enable TB0 CCR1 Interrupt

    TB0CCR0 = 20000;                                     // Set CCR0 to the value to set the period
    TB0CCR1 = 18500;                                     // Set CCR1 to the Duty Cycle



    __bis_SR_register(LPM3_bits | GIE);           // Enter LPM3, enable interrupts


    __no_operation();                             // For debugger
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                            // Clear P2.3 interrupt flag


    if(TB0CCR1 < 19000){                        //if statement to increase duty cycle until maximum when pressing P2.3

        TB0CCR1 += 100;                         //increase angle by apx. 1 degree (increase duty cycle)
    }

    else{
        TB0CCR1 = 18999;                        //Stays at maximum angle when servo cannot rotate anymore
    }

}

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 interrupt flag

        if(TB0CCR1 > 18000){            //if statement to decrease duty cycle until minimum when pressing P4.1

            TB0CCR1 -= 100;             //decrease angle by apx. 1 degree (decrease duty cycle)
        }

        else{

            TB0CCR1 = 0;
        }

}

// Timer0_B3 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            if(TB0CCR1){
            P6OUT |= BIT0;                       // Set P6.0 to an output
            }
            break;
        case TB0IV_TBCCR2:

            break;
        case TB0IV_TBIFG:
            P6OUT &= ~BIT0;                      // Clear P6.0 to an input

            break;
        default:
            break;
    }

    }

