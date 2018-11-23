#include <Arduino.h>
#include <LiquidCrystal.h>
#define USE_ROTARY_ENCODER
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <MIDI_Controller.h>
#include <Keypad.h>

const byte ROWS = 2; //four rows
const byte COLS = 3; //four columns
const int chan = 1, vel = 127, rs = 6, en = 7;
const int bpmCC = 1, 
          swingCC = 2,
          nextPrevCC = 3,
          repeatCC = 4;
const int noteNext = 1, 
          notePrev = 2,
          noteRepeat = 1;
// const int notePlay = 1;
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

String lookupChar(int i) {
  // Serial.println("Looking up for: " + String(i));
  switch (i) {
    case 32:
      return " ";
    case 33:
      return "!";
    case 34:
      return '"';
    case 35:
      return "#";
    case 36:
      return "$";
    case 37:
      return '%';
    case 38:
      return "&";
    case 39:
      return "'";
    case 40:
      return "(";
    case 41:
      return ")";
    case 42:
      return "*";
    case 43:
      return "+";
    case 44:
      return ",";
    case 45:
      return "-";
    case 46:
      return ".";
    case 47:
      return "/";
    case 48:
      return "0";
    case 49: 
      return "1";
    case 50:
      return "2";
    case 51:
      return "3";
    case 52:
      return "4";
    case 53:
      return "5";
    case 54:
      return "6";
    case 55:
      return "7";
    case 56:
      return "8";
    case 57:
      return "9";
    case 58:
      return ":";
    case 59: 
      return ";";
    case 60:
      return "<";
    case 61:
      return "=";
    case 62:
      return ">";
    case 63:
      return "?";
    case 64:
      return "@";
    case 65:
      return "A";
    case 66:
      return "B";
    case 67:
      return "C";
    case 68:
      return "D";
    case 69:
      return "E";
    case 70:
      return "F";
    case 71:
      return "G";
    case 72:
      return "H";
    case 73:
      return "I";
    case 74:
      return "J";
    case 75:
      return "K";
    case 76:
      return "L";
    case 77:
      return "M";
    case 78:
      return "N";
    case 79:
      return "O";
    case 80:
      return "P";
    case 81:
      return "Q";
    case 82:
      return "R";
    case 83:
      return "S";
    case 84:
      return "T";
    case 85:
      return "U";
    case 86:
      return "V";
    case 87:
      return "W";
    case 88:
      return "X";
    case 89:
      return "Y";
    case 90:
      return "Z";
    default:
      return "N/A";
  }
}

void printButton(char key) {
  lcd.setCursor(0, 0);
  lcd.print("Button: " + String(key));
}

// Keypad Event
void keypadEvent(KeypadEvent key){
  Serial.println(key);
  //Serial.println("type: " + String(keypad.getState()));
  //Serial.println(PRESSED);
  switch (buttons.getState()){
  case PRESSED:
    if (key == 'a') {
      Serial.println("Pressed Prev");
      usbMIDI.sendControlChange(nextPrevCC, notePrev, chan);
    }

    if (key == 'b') {
      Serial.println("Pressed Play");
        usbMIDI.sendRealTime(usbMIDI.Start);
      }

    if (key == 'c') {
      Serial.println("Pressed Next");
      usbMIDI.sendControlChange(nextPrevCC, noteNext, chan);
    }

    if (key == 'd') {
      Serial.println("Pressend Panic!");
      usbMIDI.sendRealTime(usbMIDI.Stop);
    }

    if (key == 'e') {
      Serial.println("Pressed Repeat");
      usbMIDI.sendControlChange(repeatCC, noteRepeat, chan);
    }

    if (key == 'f') {
      Serial.println("Pressed End");
    }

    break;

  case HOLD:
    Serial.println("HOLD IT");
    break;
  }
}

void printBPM(String bpm) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BPM: " + bpm);
}

void sysexHandler(byte *data, unsigned int size) {
  Serial.println("------");

  if (data[1] == 66 && data[4] == 247) {
    printBPM(lookupChar(data[2]) + lookupChar(data[3]));
  }

  if (data[1] == 66 && data[4] != 247) {
    printBPM(lookupChar(data[2]) + lookupChar(data[3]) + lookupChar(data[4]));
  }
  
  for (int i = 0; i < size; i++) {
    Serial.print(lookupChar(data[i]));
  }

  Serial.println(" ");

  for (int i = 0; i < size; i++) {
    Serial.print(String(data[i]));
  }

  Serial.println(" ");

  Serial.println("=============");
}

void setup() {
  lcd.begin(20, 4);

  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    Serial.println(thisChar);
    lcd.print(thisChar);
  }

  usbMIDI.setHandleSystemExclusive(sysexHandler);
  
  // Setup buttons
  buttons.addEventListener(keypadEvent);
  buttons.setHoldTime(1500);
}

void loop() {
  usbMIDI.read();
  MIDI_Controller.refresh();

  buttons.getKey();
}