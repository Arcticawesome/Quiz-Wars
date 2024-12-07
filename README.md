# Quiz-Wars

This repository contains the programming for a Quiz-Bowl style buzzer system for Arduino UNO. 

The system is set up for two teams of four people. Questions are displayed on the python terminal word by word at a specified, mutable rate. Use keyboards to type in the answer. Remember, interruptions are ALLOWED...

Have fun, nerds!

Instructions: 

1. Run the arduino file first. Do NOT open the Serial Monitor (or else the serial port would be in use and it is needed to communicate with the Python code)
2. Run the python file.
3. Enjoy!

Unique functionality: This system does not require separate digital pins being used for the LEDs! The digital pin, once it reads voltage coming in from the button in the buzzer, will switch from input to output and start to provide the voltage to keep the LED on.
