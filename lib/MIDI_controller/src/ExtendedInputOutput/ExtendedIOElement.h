#ifndef EXTENDEDIOELEMENT_H_
#define EXTENDEDIOELEMENT_H_

#include "Arduino.h"
#include "../Helpers/Linked_List.h"

#ifdef __AVR__
typedef uint8_t pin_t;
#else
typedef uint16_t pin_t;
#endif
typedef int analog_t;

class ExtendedIOElement
{
public:
  ExtendedIOElement(pin_t length)
      : length(length)
  {
    INSERT_INTO_LINKED_LIST(this, first, last);

    start = offset;
    end = offset + length;
    offset = end;
  }
  ~ExtendedIOElement()
  {
    DELETE_FROM_LINKED_LIST(this, first, last);
  }

  virtual void pinMode(pin_t pin, uint8_t mode){};
  virtual void digitalWrite(pin_t pin, uint8_t mode){};
  virtual int digitalRead(pin_t pin){};
  virtual analog_t analogRead(pin_t pin){};
  virtual void analogWrite(pin_t pin, analog_t val){};

  virtual void begin(){};
  virtual void refresh(){};
  virtual void reset(){};
  virtual void print(){};

  pin_t pin(pin_t p);
  pin_t getEnd();
  pin_t getStart();

  ExtendedIOElement *getNext();
  static ExtendedIOElement *getFirst();

protected:
  pin_t length, start, end;

  ExtendedIOElement *next = nullptr, *previous = nullptr;

  static pin_t offset;
  static ExtendedIOElement *last;
  static ExtendedIOElement *first;
};

#endif // EXTENDEDIOELEMENT_H_