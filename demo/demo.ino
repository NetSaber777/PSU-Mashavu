#include <SD.h>

int ledOnOff = 6;
const int chipSelect = 4;

#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define RXPIN 2
#define TXPIN 3

#define GPSBAUD 4800


TinyGPS gps;

SoftwareSerial uart_gps(RXPIN, TXPIN);

String getgps(TinyGPS &gps);

void setup()
{

  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(6,OUTPUT);
  
  uart_gps.begin(GPSBAUD);
  
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
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ","; 
    }
  }
  //add GPS
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    //get GPS Data
    while(uart_gps.available())     // While there is data on the RX pin...
    {
      int c = uart_gps.read();    // load the data into a variable...
      if(gps.encode(c))      // if there is a new valid sentence...
      {
        datatString += getgps(gps);         // then grab the data.
      }
    }
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    digitalWrite(ledOnOff,HIGH);
    delay(1000);
    digitalWrite(ledOnOff,LOW);
    
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
  
}


void getgps(TinyGPS &gps)
{
  // To get all of the data into varialbes that you can use in your code, 
  // all you need to do is define variables and query the object for the 
  // data. To see the complete list of functions see keywords.txt file in 
  // the TinyGPS and NewSoftSerial libs.
  String temporaryGPSData = "";
  // Define the variables that will be used
  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  temporaryGPSData += String("Lat/Long:"); 
  temporaryGPSData += String(latitude,5); 
  temporaryGPSData += String(","); 
  temporaryGPSData += String(longitude,5);
  temporaryGPSData += String(":"); 
  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
  temporaryGPSData += String("Date:"); 
  temporaryGPSData += String(month, DEC); 
  temporaryGPSData += String("/"); 
  temporaryGPSData += String(day, DEC); temporaryGPSData += String("/"); temporaryGPSData += String(year);
  temporaryGPSData += String("  Time: "); temporaryGPSData += String(hour, DEC); temporaryGPSData += String(":"); 
  temporaryGPSData += String(minute, DEC); temporaryGPSData += String(":"); temporaryGPSData += String(second, DEC); 
  temporaryGPSData += String("."); temporaryGPSData += stringln(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr
  
  /*
  // Here you can print the altitude and course values directly since 
  // there is only one value for the function
  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());  
  // Same goes for course
  Serial.print("Course (degrees): "); Serial.println(gps.f_course()); 
  // And same goes for speed
  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
  Serial.println();
  */
  
  // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  //Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  //Serial.println(); Serial.println();
  return temporaryGPSData;
}
