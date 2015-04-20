//Main Mashavu Work Sketch
/*
**
Requires following work libraries in Arduino sketchbook libraries folder - http://www.arduino.cc/en/guide/libraries
-Adafruit RGB LCD Shield Library - https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library/archive/master.zip 
--Guide https://learn.adafruit.com/rgb-lcd-shield/overview
-Adafruit Ultimate GPS Logger Shield
--Guide https://learn.adafruit.com/adafruit-ultimate-gps-logger-shield/overview
-12 Button Keypad - http://playground.arduino.cc/uploads/Code/keypad.zip
--Guide http://cdn.sparkfun.com/datasheets/Components/General/SparkfunCOM-08653_Datasheet.pdf


LCD Shield Colors
*/
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Keypad.h>

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
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.print("Hello, Patient!");
  lcd.setBacklight(WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
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
  }
}
