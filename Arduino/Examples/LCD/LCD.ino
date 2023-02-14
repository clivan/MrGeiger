/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

//Include the library code:
#include <LiquidCrystal.h>

//Initialize the library by associating any needed LCD interface pin
//With the arduino pin number it is connected to
const int rs=9, en=10, d4=6, d5=8, d6=5, d7=7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  //Set the cursor to column 0, line 1
  //(Note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  //Print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
