/*
  AnalogReadSerial for rain sensor

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

//The setup routine runs once when you press reset:
void setup() {
  //Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

//The loop routine runs over and over again forever:
void loop() {
  //Read the input on analog pin 0:
  int sensorValue=analogRead(A0);
  //Print out the value you read:
  Serial.println(sensorValue);
  delay(1);//Delay in between reads for stability
}
