# EyeBall

I made this project in a week before halloween event in my gym. Wanted to make something original and funny. So after some long thoughts I decided to create monks third eye with joystick control. 

# Parts used

- 1x Arduino Nano
- 2x SN74HC0595N
- Joystick
- 52x Led diodes
- 1x 10k resistor
- 8x 150R resistor

# Modes and operating principle
There are two modes in which the eye can work:
- Joystick control
- Auto

You can switch between auto and joystick control by joystick click. During Joystick control you can move eyeball pupil according to you joystick position and in auto mode, default animation gets played. The eye blinks once or twice every 2 to 4 seconds.
Leds get multiplexed to save power and give ability to connect all of them to microcontroller.   

# Code
I didn't find a good library to control both SN chips so I made my own functions to get them to work. 

- ```void driveSwitch()``` loops all array values and calls ```latch()``` and creates one full cycle through all leds.
- ```void latch(uint8_t x, uint8_t y)``` bitwrites data to SN chips and blinks one led according to X and Y data. The function uses 2d array data generated before.
- ```void blink``` blinks the eye 
- ```void setPupilInArr()``` sets pupil in main eye array depending on the position of joystick 
- ```void animationDrive()``` plays animation and sets joystick values to ```animationArr``` array values
- ```void mapXY()``` reads joystick button and its potentiometer values

# Bugs
If you push joystick on one of the corners too far, some part of pupil will be showed on opposite side of eye

# Schematic
![Eye Ball Schematic](https://i.ibb.co/tXrQGSY/Eye-Ball-Capture.png)

# Result
![Eye Ball Schematic](https://i.ibb.co/CndLwnX/IMG-3117.jpg)
