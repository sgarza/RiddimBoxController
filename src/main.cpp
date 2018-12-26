#include <Arduino.h>
#include <LiquidCrystal.h>
#include <CapacitiveSensor.h>
#include <MIDI_Controller.h>
#include <Keypad.h>

byte controllerMode = 0; // Default mode 0 (0: Drum Machine, 1: Looper)

const byte ROWS = 2; //four rows
const byte COLS = 3; //four columns
const int chan = 1, vel = 127, rs = 6, en = 7;
const int bpmCC = 1, 
          swingCC = 2,
          nextPrevCC = 3,
          repeatCC = 4,
          endCC = 5,
          looperCC = 6;
const int noteNext = 1, 
          notePrev = 2,
          noteRepeat = 1,
          noteEnd = 1,
          noteLooperPlay = 1,
          noteLooperUndo = 2,
          noteBPMUp = 127,
          noteBPMDown = 1,
          noteSwingLeft = 1,
          noteSwingRight = 127;

char keys[ROWS][COLS] = {
  {'d','e','f'},
  {'a','b','c'}
};
byte rowPins[ROWS] = {8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad buttons = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal lcd(rs, en, 2, 3, 4, 5);

// Capacitive sensors
const byte capacitiveSendPin = 18;
const byte bpmUpPin = 22;
const byte bpmDownPin = 21;
const byte swingLeftPin = 20;
const byte swingRightPin = 19;
const long capThreshold = 4000;
int upCount = 0;
int downCount = 0;
int leftCount = 0;
int rightCount = 0;
const int countLimit = 20;

CapacitiveSensor bpmUp = CapacitiveSensor(capacitiveSendPin, bpmUpPin);
CapacitiveSensor bpmDown = CapacitiveSensor(capacitiveSendPin, bpmDownPin);
CapacitiveSensor swingRight = CapacitiveSensor(capacitiveSendPin, swingRightPin);
CapacitiveSensor swingLeft = CapacitiveSensor(capacitiveSendPin, swingLeftPin);

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

void printMode() {
  lcd.setCursor(0, 0);
  lcd.print("MODE:               ");
  lcd.setCursor(0, 0);
  if (controllerMode == 0) {
    lcd.print("MODE: Drum-Machine");
  }

  if (controllerMode == 1) {
    lcd.print("MODE: Looper");
  }
}

// Keypad Event
void keypadEvent(KeypadEvent key){
  Serial.println(key);
  //Serial.println("type: " + String(keypad.getState()));
  //Serial.println(PRESSED);
  switch (buttons.getState()){
  case PRESSED:
    if (key == 'a') {
      if (controllerMode == 0) {
        Serial.println("Pressed Prev");
        usbMIDI.sendControlChange(nextPrevCC, notePrev, chan);
      }

      if (controllerMode == 1) {
        Serial.println("Pressed Undo");
        usbMIDI.sendControlChange(looperCC, noteLooperUndo, chan);
      }
    }

    if (key == 'b') {
      if (controllerMode == 0) {
        Serial.println("Pressed Play");
        usbMIDI.sendRealTime(usbMIDI.Start);
      }

      if (controllerMode == 1) {
        Serial.println("Pressed Looper Play");
        usbMIDI.sendControlChange(looperCC, noteLooperPlay, chan);
      }
    }

    if (key == 'c') {
      if (controllerMode == 0) {
        Serial.println("Pressed Next");
        usbMIDI.sendControlChange(nextPrevCC, noteNext, chan);
      }

    }

    if (key == 'd') {
      Serial.println("Pressed Mode Toggle");

      if (controllerMode == 0) {
        controllerMode = 1;
      } else {
        controllerMode = 0;
      }

      printMode();
    }

    if (key == 'e') {
      if (controllerMode == 0) {
        Serial.println("Pressed Repeat");
        usbMIDI.sendControlChange(repeatCC, noteRepeat, chan);
      }
    }

    if (key == 'f') {
      if (controllerMode == 0) {
        Serial.println("Pressed End");
        usbMIDI.sendControlChange(endCC, noteEnd, chan);
      }
    }

    break;

  case HOLD:
    Serial.println("HOLD IT");
    if (key == 'b') {
      Serial.println("Pressend Panic!");
      usbMIDI.sendRealTime(usbMIDI.Stop);
    }
    break;
  }
}

void printBPM(String bpm) {
  lcd.setCursor(0, 3);
  if (bpm.length() == 2 ) {
    lcd.print("BPM: " + bpm + " ");
  } else {
    lcd.print("BPM: " + bpm);
  }
  
}

void printSwing(String swing) {
  // col, row
  lcd.setCursor(12, 3);
  lcd.print("Swing: " + swing);
}

void sysexHandler(byte *data, unsigned int size) {
  Serial.println("------");

  // IS BPM OF 2 DIGITS
  if (data[1] == 66 && data[4] == 247) {
    printBPM(lookupChar(data[2]) + lookupChar(data[3]));
  }

  // IS BPM OF 3 DIGITS
  if (data[1] == 66 && data[4] != 247) {
    printBPM(lookupChar(data[2]) + lookupChar(data[3]) + lookupChar(data[4]));
  }

  // IS SWING
  if (data[1] == 83) {
    printSwing(lookupChar(data[2]));
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

void checkCapacitors() {
  long up = bpmUp.capacitiveSensor(30);
  long down = bpmDown.capacitiveSensor(30);
  long left = swingLeft.capacitiveSensor(30);
  long right = swingRight.capacitiveSensor(30);

  if (up > capThreshold) {
    upCount += 1;
    if (upCount > countLimit) {
      Serial.println(String(up));
      upCount = 0;
      bpmUp.reset_CS_AutoCal();
      usbMIDI.sendControlChange(bpmCC, noteBPMUp, chan);
    }
  }

  if (down > capThreshold) {
    downCount += 1;
    if (downCount > countLimit) {
      Serial.println(String(down));
      downCount = 0;
      bpmDown.reset_CS_AutoCal();
      usbMIDI.sendControlChange(bpmCC, noteBPMDown, chan);
    }
  }

  if (left > capThreshold) {
    leftCount += 1;
    if (leftCount > countLimit) {
      Serial.println(String(left));
      leftCount = 0;
      swingLeft.reset_CS_AutoCal();
      usbMIDI.sendControlChange(swingCC, noteSwingLeft, chan);
    }
  }

  if (right > capThreshold) {
    rightCount += 1;
    if (rightCount > countLimit) {
      Serial.println(String(right));
      rightCount = 0;
      swingRight.reset_CS_AutoCal();
      usbMIDI.sendControlChange(swingCC, noteSwingRight, chan);
    }
  }
}

void setup() {
  lcd.begin(20, 4);

  lcd.setCursor(0, 0);
  printBPM(0);
  printSwing(0);
  printMode();

  usbMIDI.setHandleSystemExclusive(sysexHandler);
  
  // Setup buttons
  buttons.setHoldTime(350);
  buttons.addEventListener(keypadEvent);
}

void loop() {
  usbMIDI.read();
  MIDI_Controller.refresh();

  buttons.getKey();

  checkCapacitors(); 
}