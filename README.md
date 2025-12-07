# Cisc210-Final
Qibla Compass project 

Student - Mohammed Armaan Raza, armaanra@udel.edu, 702693891, No Partner

## Project Description
Muslims pray 5 times a day and when praying they pray in the direction of Mecca, Saudi Arabia. Knowing which way to face can be tedious to work out so i created this project to help me out.
Utalizing the magnetometer within the SenseHAT on the Pi I have created a compass which points to the direction of the Qibla from my home (Newark). The direction is set to 32 degrees when within the range of 30-40 degrees a arrow will light up on the SenseHAT to show you are in the right range. Otherwise the display is off. (Pi must be on flat surface of compass does not work).

The Joystick is used to control display of the compass.

Push in - toggle on/off

Up - increase brightness(1-5)

Down - dim brightness(1-5)

Left - last color

Right - next color

The color of the arrow by default is green but can be changed to white, blue, or red (in that order).

### Compile
make
./qibla_compass
