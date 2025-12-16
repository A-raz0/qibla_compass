# Qibla Compass


## Project Description
Muslims pray 5 times a day and when praying they pray in the direction of Mecca, Saudi Arabia. Knowing which way to face can be tedious to work out so i created this project to help me out.
Utalizing the magnetometer within the SenseHAT on the Pi I have created a compass which points to the direction of the Qibla from my home (Newark). The direction is set to 32 degrees when within the range of 30-40 degrees a arrow will light up on the SenseHAT to show you are in the right range. Otherwise the display is off. (Pi must be on flat surface of compass does not work).


## What It Does

Continuously reads compass data from the Sense HAT magnetometer

Uses a fixed Qibla bearing for Newark, Delaware (40°)

Displays an arrow only when the device is within a valid Qibla range

Updates in real time as the device is rotated

Uses the Sense HAT LED matrix for all visual output

## Controls

The Sense HAT joystick controls the display:

Center – turn the display ON or OFF

Up – increase LED brightness

Down – decrease LED brightness

Left / Right – cycle through arrow color themes

Available color themes:

Green

White

Blue

Red

### Compile
make
./qibla_compass
