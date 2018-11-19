#include <Arduino.h>
#include <LiquidCrystal.h>
#define USE_ROTARY_ENCODER
// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <MIDI_Controller.h>
#include <Keypad.h>

const byte ROWS = 2; //four rows
const byte COLS = 3; //four columns
const int chan = 1, vel = 127, rs = 6, en = 7;
const int bpmCC = 1, swingCC = 2;
char keys[ROWS][COLS] = {
  {'d','e','f'},
  {'a','b','c'}
};
byte rowPins[ROWS] = {9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad buttons = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal lcd(rs, en, 2, 3, 4, 5);
RotaryEncoder bpmEncoder(23, 22, bpmCC, chan, 1, NORMAL_ENCODER, TWOS_COMPLEMENT);
RotaryEncoder swingEncoder(20, 19, swingCC, chan, 1, NORMAL_ENCODER, TWOS_COMPLEMENT);

Digital bpmButton(21, 7, chan, vel);
Digital swingButton(18, 8, chan, vel);

void printButton(char key) {
  lcd.setCursor(0, 0);
  lcd.print("Button: " + String(key));
}

void setup() {

  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    Serial.println(thisChar);
    lcd.print(thisChar);
  }
}

void loop() {
  MIDI_Controller.refresh();

  char customKey = buttons.getKey();
  
  if (customKey){
    printButton(customKey);
    Serial.println(customKey);
  }
}