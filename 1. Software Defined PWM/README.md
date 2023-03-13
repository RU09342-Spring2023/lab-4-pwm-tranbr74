***Author: Brandon Tran
Lab: Software Defined PWM: ***

***What's happenning?***
This code uses a software PWM to control the brightness a red LED and a green LED. There are two buttons (P4.1 and P2.3) that determines the brightness of the LEDs with the push of the respected buttons. There 10 different levels of brightness and each level of brightness is changed one at a time per button press. When the LEDs reach their maximum brightness, they will reset to its lowest brightness.

***What was used?***
Pin 1.0, Red LED
Pin 1.0 is set to an output, toggled by Timer B0interrupt, brightness is changed by Pin 2.3.

Pin 6.6, Green LED
Pin 6.6 is set to an output, toggled by Timer B1interrupt, brightness is changed by Pin 4.1.

Pin 2.3, Blinking Speed Button
Pin 2.3 is set to an input, pullup resistor enabled, interrupt enabled, falling edge.

Pin 4.1, Reset Button
Pin 4.1 is set to an input, pullup resistor enabled, interrupt enabled, falling edge.

Timer B0 Peripheral
Set to TB0CCR1 interrupt, SMCLK, up mode.

Timer B1 Peripheral
Set to TB1CCR1 interrupt, SMCLK, up mode.

***Design Constraints***
There needs to be a limit when the LEDs' duty cycles are incremented to 100%. When the duty cycle is at a 100%, it needs to become 0% duty cycle. This can be done by using an if statement to change the duty cycle from 100% to 0%.
``#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                            // Clear P2.3 interrupt flag

    if (TB0CCR1 >= 1000) { //If brightness is at maximum value, change it back to it's lowest brightness
        TB0CCR1 = 1;
    }
    else {
        TB0CCR1 += 100;  //Increase brightness by 10%
    }
}


#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 interrupt flag
    if (TB1CCR1 >= 1000) { //If brightness is at maximum value, change it back to it's lowest brightness
            TB1CCR1 = 1;
        }
        else {
            TB1CCR1 += 100;  //Increase brightness by 10%
        }
}
``
**How to use the code**
First, load the code into code composer or another compiler of your choosing. Next, upload the code to the MSP430FR2355. The LEDs should begin at 50 percent brightness. Pin 2.3 is used to increment the red LED duty cycle by 10% and Pin 4.1 for the green LED. The duty cycle will increase for every button press until it hits 100% duty cycle. The next button press after 100% will reset the duty cycle to 0. From there on, the cycle of reaching to 100% duty cycle will repeat.
