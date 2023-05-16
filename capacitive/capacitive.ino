//////////////////////////////////////////////////////////////////////////
// LEDs:
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN  4   // <= back  //  front: 3 
#define NUMPIXELS     258 // <= back  //  front: 130

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_BGR + NEO_KHZ800);


//////////////////////////////////////////////////////////////////////////
// Capacitive:
// TODO: test alternative? https://github.com/martin2250/ADCTouch/

int pins[] = {7, 11}; // pins number of the capacitive sensors
//int pins[] = {6,7,8, 10,11,12}; // pins number of the capacitive sensors

const int PIN_QTT = sizeof pins / sizeof pins[0]; // compute number of pins
float newValue[PIN_QTT] = {0};
float oldValue[PIN_QTT] = {0};

float smoothCap(int pin);
uint8_t readCapacitivePin(int pinToMeasure);


//////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  pixels.begin();
}


//////////////////////////////////////////////////////////////////////////
void loop() {
  for (int i = 0; i < PIN_QTT; i++) {
    Serial.print( smoothCap(i) );
    Serial.print(" ");
  }
  Serial.println("0 75 200");               // TODO get max for automatic calibration

  bool touch = (smoothCap(0) > 75) || (smoothCap(1) > 75);

  int amp = touch? 127 : 0;

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(amp, amp, amp));
  }
  pixels.show();
}


//////////////////////////////////////////////////////////////////////////
float smoothCap(int pin) {
  const float COEF = 0.6;

  // read touch sensor value a 1st time:
  newValue[pin] = readCapacitivePin(pins[pin]);

  // improve estimation:
  for (int i = 0; i < 9; i++)
    newValue[pin] += readCapacitivePin(pins[pin]);

  // smooth filter
  newValue[pin] = newValue[pin] * COEF + oldValue[pin] * (1 - COEF);

  // backup
  oldValue[pin] = newValue[pin];

  return newValue[pin];
}


//////////////////////////////////////////////////////////////////////////
// readCapacitivePin

//  Input: Arduino pin number (with a 1M pull up)
//  Output: A number, from 0 to 10 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher

uint8_t readCapacitivePin(int pinToMeasure) {

  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;

  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.

  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));

  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  delay(1);

  uint8_t SREG_old = SREG; //back up the AVR Status Register

  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();

  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.

  uint8_t cycles = 20; // TODO: find why it was 17 before!
       if (*pin & bitmask) { cycles = 0;}
  else if (*pin & bitmask) { cycles = 1;}
  else if (*pin & bitmask) { cycles = 2;}
  else if (*pin & bitmask) { cycles = 3;}
  else if (*pin & bitmask) { cycles = 4;}
  else if (*pin & bitmask) { cycles = 5;}
  else if (*pin & bitmask) { cycles = 6;}
  else if (*pin & bitmask) { cycles = 7;}
  else if (*pin & bitmask) { cycles = 8;}
  else if (*pin & bitmask) { cycles = 9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}
  else if (*pin & bitmask) { cycles = 17;}
  else if (*pin & bitmask) { cycles = 18;}
  else if (*pin & bitmask) { cycles = 19;}

  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
  SREG = SREG_old;

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}
