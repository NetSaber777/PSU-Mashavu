//Main Mashavu Work Sketch
/*
Our Git Repo - https://github.com/NetSaber777/PSU-Mashavu

Requires following work libraries in Arduino sketchbook libraries folder - http://www.arduino.cc/en/guide/libraries
-Adafruit RGB LCD Shield Library - https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library/archive/master.zip 
--Guide https://learn.adafruit.com/rgb-lcd-shield/overview
-Adafruit Ultimate GPS Logger Shield
--Guide https://learn.adafruit.com/adafruit-ultimate-gps-logger-shield/overview
-12 Button Keypad - http://playground.arduino.cc/uploads/Code/keypad.zip
--Guide http://cdn.sparkfun.com/datasheets/Components/General/SparkfunCOM-08653_Datasheet.pdf

*/

//LCD Shield Colors
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Keypad.h>
#include <TinyGPS.h>

//Begin LCD Setup
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//Begin Keypad Setup
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};
byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  delay(2000);
  // set up the LCD's number of columns and rows: 
  //Serial1 is debug channel on USB.
  Serial1.begin(9600);
  Serial1.println("Begin Debug");
  //Begin LCD Screen
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.print("Hello, Patient!");
}

void loop() {
  if (Serial1.available() > 0) {
  for (int i=0; i<8; i++) {
    char buff[i];
    Serial1.print(buff[i] = Serial2.read());
    }
  }
  /*
  char key = keypad.getKey();

  if (key) {
    lcd.clear();
    lcd.print(key);
  }
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      lcd.print("UP ");
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.print("DOWN ");
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("LEFT ");
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.print("RIGHT ");
      lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
    }
  }*/
}

String getgps(TinyGPS &gps)
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
  temporaryGPSData += String("."); temporaryGPSData += String(hundredths, DEC);
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
