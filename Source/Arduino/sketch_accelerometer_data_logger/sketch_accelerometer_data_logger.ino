//-------------------------------------------------------------------------------
//  TinyCircuits Accelerometer TinyShield Example Sketch
//  Using Bosch BMA250 in I2C mode
//
//  Created 6/30/2013
//  by Ken Burns, TinyCircuits http://Tiny-Circuits.com
//
//  This example code is in the public domain.
//
//-------------------------------------------------------------------------------

#include <Wire.h>
#include <SD.h>
#include <stdlib.h>

#define BMA250_I2CADDR      0x18
#define BMA250_RANGE        0x03   // 0x03 = 2g, 0x05 = 4g, 0x08 = 8g, 0x0C = 16g
#define BMA250_BW           0x08   // 7.81Hz (update time of 64ms)

int AccelX;
int AccelY;
int AccelZ;
float AccelTemperature;
const int chipSelect = 10;
char buffer[50];

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  BMA250Init();

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}


void loop()
{
  BMA250ReadAccel();

  // make a string for assembling the data to log:
  String dataString = "";

  // Print out the accelerometer data
  dataString += "x: ";
  dataString += String(AccelX);
  dataString += ", y: ";
  dataString += String(AccelY);
  dataString += ", z: ";
  dataString += String(AccelZ);
  dataString += ", t: ";
  dtostrf(AccelTemperature,10,5,buffer);
  dataString += buffer;
  dataString += "degC";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(100);
}


void BMA250Init()
{
  // Setup the range measurement setting
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x0F); 
  Wire.write(BMA250_RANGE);
  Wire.endTransmission();
  
  // Setup the bandwidth
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x10);
  Wire.write(BMA250_BW);
  Wire.endTransmission();
}


int BMA250ReadAccel()
{
  uint8_t ReadBuff[8];
  
  // Read the 7 data bytes from the BMA250
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(BMA250_I2CADDR,7);
  
  for(int i = 0; i < 7;i++)
  {
    ReadBuff[i] = Wire.read();
  }
  
  AccelX = ReadBuff[1] << 8;
  AccelX |= ReadBuff[0];
  AccelX >>= 6;
  
  AccelY = ReadBuff[3] << 8;
  AccelY |= ReadBuff[2];
  AccelY >>= 6;
  
  AccelZ = ReadBuff[5] << 8;
  AccelZ |= ReadBuff[4];
  AccelZ >>= 6;  

  AccelTemperature = (ReadBuff[6] * 0.5) + 24.0;
}


