#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <RTClib.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;

SoftwareSerial gpsSerial(19, 18); // RX: pin 19, TX: pin 18
TinyGPSPlus gps;

const int geigerPin = A1;
int geigerCount = 0;

const int rainPin = A0;
int rainValue = 0;

const int lightningIRQ = 2;
const int lightningAddress = 0x03;

const int chipSelect = 4;
SdFat sd;
File dataFile;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23);

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  dht.begin();
  bmp.begin();
  pinMode(geigerPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lightningIRQ), handleLightningIRQ, RISING);
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Error al iniciar el reloj en tiempo real");
    while (1);
  }
  if (!sd.begin(chipSelect, Wire)) {
    Serial.println("Error al inicializar la tarjeta SD");
    return;
  }
  dataFile = sd.open("datos.csv", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error al abrir el archivo de datos");
    return;
  }
  dataFile.println("Fecha,Hora,Contador Geiger,Sensor de lluvia,Sensor de relámpagos,BMP180,DHT11,Módulo GPS");
  lcd.print("Datos guardados");
  delay(2000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();
  geigerCount = 0;
  rainValue = analogRead(rainPin);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fecha: ");
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());
  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
  lcd.setCursor(0, 2);
  lcd.print("Geiger: ");
  lcd.print(geigerCount);
  lcd.setCursor(0, 3);
  lcd.print("Lluvia: ");
  lcd.print(rainValue);
  int lightningDist = readLightning();
  if (lightningDist >= 0) {
    lcd.setCursor(12, 2);
    lcd.print("Rayo: ");
    lcd.print(lightningDist);
    lcd.print(" km");
  }
  float bmpTemp = bmp.readTemperature();
  float bmpPressure = bmp.readPressure() / 100.0F;
  float dhtTemp = dht.readTemperature();
  float dhtHumidity = dht.readHumidity();
  float gpsLat, gpsLon;
  uint32_t gpsAge;
  gpsSerial.listen();
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      gps.f_get_position(&gpsLat, &gpsLon, &gpsAge);
    }
  }
  dataFile.print(now.day());
  dataFile.print("/");
  dataFile.print(now.month());
  dataFile.print("/");
  dataFile.print(now.year());
  dataFile.print(",");
  dataFile.print(now.hour());
  dataFile.print(":");
  dataFile.print(now.minute());
  dataFile.print(":");
  dataFile.print(now.second());
  dataFile.print(",");
  dataFile.print(geigerCount);
  dataFile.print(",");
  dataFile.print(rainValue);
  dataFile.print(",");
  dataFile.print(lightningDist);
  dataFile.print(",");
  dataFile.print(bmpTemp);
  dataFile.print(",");
  dataFile.print(dhtTemp);
  dataFile.print(",");
  dataFile.print(dhtHumidity);
  dataFile.print(",");
  dataFile.print(gpsLat, 6);
  dataFile.print(",");
  dataFile.println(gpsLon, 6);
}

void handleLightningIRQ() {
  int distance = readLightning();
  if (distance >= 0) {
    lcd.setCursor(12, 2);
    lcd.print("Rayo: ");
    lcd.print(distance);
    lcd.print(" km");
  }
}

int readLightning() {
  Wire.beginTransmission(lightningAddress);
  Wire.write(0x00); // Register 0x00: Noise floor level
  Wire.write(0x1C); // Set the noise floor level (suggested default)
  Wire.endTransmission();

  Wire.beginTransmission(lightningAddress);
  Wire.write(0x08); // Register 0x08: Interrupt status
  Wire.endTransmission();

  delay(2);

  Wire.requestFrom(lightningAddress, 1); // Read one byte
  byte status = Wire.read(); // Read the status byte

  if (bitRead(status, 3) == 1) {
    Wire.beginTransmission(lightningAddress);
    Wire.write(0x03); // Register 0x03: Distance to the detected event
    Wire.endTransmission();

    delay(2);

    Wire.requestFrom(lightningAddress, 2); // Read two bytes
    byte msb = Wire.read(); // Read the most significant byte
    byte lsb = Wire.read(); // Read the least significant byte

    int distance = (msb << 8) | lsb; // Combine the bytes to get the distance
    return distance;
  } else {
    return -1; // No lightning detected
  }
}

