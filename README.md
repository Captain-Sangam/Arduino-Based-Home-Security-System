# Arduino-Based-Home-Security-System

A completer Home Security system based on Arduino with multiple sensors to monitor status, pin based entry and locking, GSM shield to give instrusion alerts via SMS.

## Features

- It has multiple sensors to monitor status.
- Pin Based entry and locking.
- The number and type of sensors can be modified as per convinience.
- GSM Module used to provide intrusion alerts via SMS.

## Required Libraries

This contains the extra libraries required for successfull compliation. Extract them and paste them into your arduino's Library folder.

## Components Required

> | Component                           | Quantity |
> | ----------------------------------- | -------- |
> | Pyroelectric infrared motion sensor | 1        |
> | 4x4 universal 16 key switch keypad  | 1        |
> | Arduino Mega                        | 1        |
> | 20x4 LCD Display                    | 1        |
> | Breadboard                          | 1        |
> | M2F Jumper                          | 40       |
> | M2M Jumper X40                      | 40       |
> | Alarm Buzzer                        | 1        |
> | Reed Switch                         | 1        |
> | GSM Module                          | 1        |
> | LEDs                                | Optional |
> | 10k Rheostat                        | 1        |

## How to Connect

Follow the below given directions to connect the sensors as per the defined Main_Code

Reed Sensors x 3
   
	Reed Sensor 1  ( No Polarity )  --- 22,28
	Reed Sensor 2  ( No Polarity )  --- 23,29
	Reed Sensor 3  ( No Polarity )  --- 24,30	

PIR Sensor
 
	PIR + --- 5V
	PIR - --- GND
	PIR OUTPUT --- 31

BUZZER 

	BUZZER + -- 34
	BUZZER + -- GND

Keypad

	Row 0 46
	Row 1 47
	Row 2 48
	Row 3 49
	Col 0 50 
	Col 1 51
	Col 2 52
	Col 3 53

20x4 LCD Display

	LCD RS pin to digital pin 12
	LCD Enable pin to digital pin 11
	LCD D4 pin to digital pin 5
	LCD D5 pin to digital pin 4
	LCD D6 pin to digital pin 3
	LCD D7 pin to digital pin 2
	LCD R/W pin to ground
	LCD VSS pin to ground
	LCD VCC pin to 5V
	10K resistor:
	ends to +5V and ground
	wiper to LCD VO pin (pin 3)

GSM SHIELD 

 	RX  --- 9
	TX  --- 10
	GND --- GND


## Images

Images folder contains the overall flowchat and the pin out diagrams for the LCD Screen

## Additional Files for Set up and Testing

They contain additional codes for the following purposes.
Contents

- code.ino
  - Contains the main code
- Read_Phone_Number_from_EEPROM
  - Contains the code the check the phone number written in EEPROM
- Read_Pin_from_EEPROM
  - Contains the code the check the pin written in EEPROM
- Test_Sensor_Connections
  - Contains the code to check if all sensors are connected properly
- Write_Phone_Number_To_EEPROM
  - Contains the code the write the phone number into the EEPROM
- Write_Pin_To_EEPROM
  - Contains the code the write pin into the EEPROM
