### Overview
The Software_PWM_Generator project is designed to generate PWM signals with varying duty cycles using a software approach on the PIC18F452 microcontroller. 
The program utilizes Timer0 for precise timing and an external interrupt to cycle through different duty cycles.
This is particularly useful for applications where hardware PWM modules are either unavailable or insufficient.

### Features
Generates PWM signals with the following duty cycles: 5%, 25%, 50%, 75%, and 90%.
Allows dynamic switching between duty cycles using an external button.
Uses Timer0 for precise control over the PWM signal.
Employs an external interrupt (INT0) to trigger duty cycle changes.


Tested and verified using proteus simulation and gives same result as pwm hardware generator.
