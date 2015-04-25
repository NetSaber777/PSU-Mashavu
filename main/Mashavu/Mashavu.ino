//Main Mashavu Work Sketch
/*Our Git Repo - https://github.com/NetSaber777/PSU-Mashavu

Requires following work libraries in Arduino sketchbook libraries folder - http://www.arduino.cc/en/guide/libraries
-Adafruit RGB LCD Shield Library - https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library/archive/master.zip 
--Guide https://learn.adafruit.com/rgb-lcd-shield/overview
-Adafruit Ultimate GPS Logger Shield - https://github.com/adafruit/Adafruit-GPS-Library/
--Guide https://learn.adafruit.com/adafruit-ultimate-gps-logger-shield/overview
-12 Button Keypad - http://playground.arduino.cc/uploads/Code/keypad.zip
--Guide http://cdn.sparkfun.com/datasheets/Components/General/SparkfunCOM-08653_Datasheet.pdf
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Keypad.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include <SD.h>

//Begin DEV Settings
bool DEBUG_STATE;

//Begin LCD Setup
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
//LCD Shield Colors
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
////Global Text for LCD
char *DATA_ENTRY[] = {"1:Male 2:Female", "Age (Years):", "Height (cm):", "Weight (kg):", "Systolic BP:", "Diastolic BP:"};
char *MAIN_MENU[] = {"Hello Mashavu!", "Collect", "Options", "Status"};

//Begin GPS Setup
Adafruit_GPS GPS(&Serial1);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

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

//Logical Interrupt Time Handler
uint32_t timer = millis();

//Program Data Structs
typedef struct {
  uint8_t gender, age, height, weight, systolic_bp, diastolic_bp;
  //GPS DATA
  uint8_t hour, minute, seconds, year, month, day;
} SD_ENTRY;

//Begin Program Logic

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO && DEBUG_STATE)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

void setup() {
  //delay(5000);
  //Serial Debug via USB. Higher Refresh to keep up with parsing rate of GPS to USB Buffer
  Serial.begin(115200);
  Serial.println("Begin Mashavu Debug");
  useInterrupt(true);
  DEBUG_STATE = true;
  
  //Begin Serial1 Interface on GPS Module
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  
  //Begin LCD Screen - I2C Connection
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.print("Hello, Scrub!");
  delay(3000);
}

void gpsDebug(){
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // print state every 2 seconds
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
    
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
}

//Shows current GPS time on LCD screen
void lcdShowTime(){
  lcd.clear();
  lcd.print("Time");
  lcd.setCursor(0,1);
  
  lcd.print(GPS.hour); lcd.print(":");
  lcd.print(GPS.minute); lcd.print(":");
  lcd.print(GPS.seconds);
}



void loop() {
  
  // Alternative to Interrupt Handling. - Leave in
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop' - not active now
    char c = GPS.read();
    // Non-interrupt debug here
    if (GPSECHO && DEBUG_STATE)
      if (c) Serial.print(c);
  }
  
  // if NMEA sentece receievevd, parse away
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // parse fail handling - and we don't care
  }

  if(DEBUG_STATE)
    gpsDebug();
  
  lcdShowTime();
  delay(1000);
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

