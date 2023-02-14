/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 This example code is in the public domain.

 */

 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
 
LiquidCrystal_I2C lcd(0x27,16,2); 
File myFile;

void setup()
{
  lcd.begin(); 
  lcd.setCursor(0,0);
  lcd.print("Started");
  delay(2000);
  //Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    //Wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(53))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  //Open the file. note that only one file can be open at a time,
  //So you have to close this one before opening another.
  myFile=SD.open("test3.txt", FILE_WRITE);
  //If the file opened okay, write to it:
  if (myFile)
  {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    //Close the file:
    myFile.close();
    Serial.println("done.");
  }
  else
  {
    //If the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  //Re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile)
  {
    Serial.println("test.txt:");
    //Read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    //Close the file:
    myFile.close();
  } else
  {
    //If the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  //Nothing happens after setup
  lcd.clear();
  lcd.print("Loop");\
  delay(2000);
}

