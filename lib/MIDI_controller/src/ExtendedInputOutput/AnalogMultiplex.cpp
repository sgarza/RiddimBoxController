#include "AnalogMultiplex.h"
#include "ExtendedInputOutput.h"
using namespace ExtIO;

int AnalogMultiplex::digitalRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    return ExtIO::digitalRead(analogPin);
}
analog_t AnalogMultiplex::analogRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    ExtIO::analogRead(analogPin);
    return ExtIO::analogRead(analogPin);
}
void AnalogMultiplex::begin()
{
    // ExtIO::pinMode(analogPin, INPUT);
    for (uint8_t i = 0; i < nb_addressPins; i++)
        ExtIO::pinMode(addressPins[i], OUTPUT);
}
void AnalogMultiplex::pinMode(pin_t, uint8_t mode) {
    ExtIO::pinMode(analogPin, mode);
}

uint8_t AnalogMultiplex::pinToMuxAddress(pin_t pin)
{
    return pin & (nb_addresses - 1);
}
void AnalogMultiplex::setMuxAddress(uint8_t address)
{
    for (uint8_t i = 0; i < nb_addressPins; i++)
        ExtIO::digitalWrite(addressPins[i], address & (1 << i));
}
