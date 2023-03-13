# Author: Brandon Tran

# What is the code?
This code makes an RGB LED fade between 6 colors using a hardware pwm. The colors are:

- Red
- Orange (Red + Green)
- Green
- Cyan (Green + Blue)
- Blue
- Purple (Red + Blue)

## What was used?
- Pin 6.0 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt. This would control the Red LED of the RGB LED.
- Pin 6.1 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt. This would control the Blue LED of the RGB LED.
- Pin 6.2 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt. This would control the Green LED of the RGB LED.
- Timer B0 Peripheral: Set to TB0CCR0 interrupt, ACLK, continuous, and divided by 8.
- Timer B3 Peripheral: Set to TB3CCR0 interrupt, SMCLK, up mode, out mode 7.

### Design Considerations
The registers of the MSP430FR2355 can only go up to to only 16-bits, causing issues because timer B0 will reach that value eventually with/without a clock divider. This can be solved by resetting the register to a starting point ever time it reachest 65,535 (decimal of 16 bits).

Resistors are used to create the circuit as running the code can cause damage to the operating device such as my computer. Without resistors though, the changes in colors for the LEDs are very apparanet and clear.

Timer B0 has an offset of 8 to slow the speed of the fading LED. Larger numbers will make the fading slower while smaller numbers make the fading faster.

#### How to use
First, load the code into code composer and upload it to the MSP430FR2355. Attach the anode of the common anode RGB LED to ground and connect the LED pins and (resistors if needed) to their corresponding pins on the board. When the circuit is properly completed, the LEDs will fade from red to orange to green to cyan to blue to purple and back indefinitely as long as the anode of the RGB LED is grounded and the pins of the board are connected to red, blue, and green.
