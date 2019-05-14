#include <Arduino.h>
#include <LiquidCrystal.h>
#include <MIDI_Controller.h>
#include <Keypad.h>
#include <SimpleTimer.h>

// Groups
int lastGroup = 3;
int currentGroup = 0;
const int GROUP_0 = 0;
const int GROUP_1 = 1;
const int GROUP_2 = 2;
const int GROUP_3 = 3;

const byte ROWS = 2; //four rows
const byte COLS = 3; //four columns

char keys[ROWS][COLS] = {
  {'d','e','f'},
  {'a','b','c'}
};

byte rowPins[ROWS] = {8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10}; //connect to the column pinouts of the keypad

// MIDI Variables
const byte chan = 1;

//initialize an instance of class NewKeypad
Keypad buttons = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Init LCD
const int rs = 6, en = 7;
LiquidCrystal lcd(rs, en, 2, 3, 4, 5);

// the timer object
SimpleTimer timer;

void printGroup() {
  lcd.setCursor(0, 0);
  lcd.print("Group: " + String(currentGroup + 1));
}

void resetButtonText() {
  lcd.setCursor(0, 2);
  lcd.print("                    ");
}

void printButton(KeypadEvent key) {
  lcd.setCursor(0, 2);
  lcd.print("Button: " + String(key));

  timer.setTimeout(2000, resetButtonText);
}

void nextGroup() {
  currentGroup++;

  if (currentGroup > lastGroup) {
    currentGroup = 0;
  }

  printGroup();
}

void sendCC(int note, int value) {
  usbMIDI.sendControlChange(note, value, chan);
}

// Keypad Event
void keypadEvent(KeypadEvent key){
  // Serial.println(key);
  // Serial.println("type: " + String(buttons.getState()));
  //Serial.println(PRESSED);
  switch (buttons.getState()){
    case PRESSED:
    Serial.println(key);
    printButton(key);

      if (key == 'a') {
        switch (currentGroup) {
          case GROUP_0:
            sendCC(1, 1);
            break;
          case GROUP_1:
            sendCC(2, 1);
            break;
          case GROUP_2:
            sendCC(3, 1);
            break;
          case GROUP_3:
            sendCC(4, 1);
            break;
        }
      }

      if (key == 'b') {
        switch (currentGroup) {
          case GROUP_0:
            sendCC(1, 2);
            break;
          case GROUP_1:
            sendCC(2, 2);
            break;
          case GROUP_2:
            sendCC(3, 2);
            break;
          case GROUP_3:
            sendCC(4, 2);
            break;
        }
      }

      if (key == 'c') {
        switch (currentGroup) {
          case GROUP_0:
            sendCC(1, 3);
            break;
          case GROUP_1:
            sendCC(2, 3);
            break;
          case GROUP_2:
            sendCC(3, 3);
            break;
          case GROUP_3:
            sendCC(4, 3);
            break;
        }
      }

      if (key == 'd') {
        switch (currentGroup) {
          case GROUP_0:
            sendCC(1, 4);
            break;
          case GROUP_1:
            sendCC(2, 4);
            break;
          case GROUP_2:
            sendCC(3, 4);
            break;
          case GROUP_3:
            sendCC(4, 4);
            break;
        }
      }

      if (key == 'e') {
        switch (currentGroup) {
          case GROUP_0:
            sendCC(1, 5);
            break;
          case GROUP_1:
            sendCC(2, 5);
            break;
          case GROUP_2:
            sendCC(3, 5);
            break;
          case GROUP_3:
            sendCC(4, 5);
            break;
        }
      }

      if (key == 'f') {
        Serial.println('f');
        nextGroup();
      }

      break;

    case HOLD:
      break;
    case RELEASED:
      break;
    case IDLE:
      break;
  }
}

void setup() {
  // LCD init
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  printGroup();
  // Setup buttons
  buttons.setHoldTime(500);
  buttons.addEventListener(keypadEvent);
}

void loop() {
  usbMIDI.read();
  MIDI_Controller.refresh();

  buttons.getKey();
  timer.run();

}