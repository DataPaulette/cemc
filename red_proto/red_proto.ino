//#define DEBUG_PRINT

//////////////////////////////////////////////////////////////////////////
// LEDs control:
//////////////////////////////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>

const int PIN_BACK        = 4;
const int NUMPIXELS_BACK  = 258;

const int PIN_FRONT       = 3;
const int NUMPIXELS_FRONT = 130;

Adafruit_NeoPixel back = Adafruit_NeoPixel(NUMPIXELS_BACK, PIN_BACK, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel front = Adafruit_NeoPixel(NUMPIXELS_FRONT, PIN_FRONT, NEO_BGR + NEO_KHZ800);


void setAllOff(Adafruit_NeoPixel& strip, const int NUMPIXELS);
void breathe(Adafruit_NeoPixel& strip, const int NUMPIXELS);
void Animation(Adafruit_NeoPixel& strip, const int NUMPIXELS, const int NB_BLOBS);



//////////////////////////////////////////////////////////////////////////
// Capacitive sensing:
//////////////////////////////////////////////////////////////////////////

int pins[] = {7, 11}; // pins number of the capacitive sensors

// 6 sensing areas are almost possible but they need some hardware mods:
//int pins[] = {6,7,8, 10,11,12};

const int PIN_QTT = sizeof pins / sizeof pins[0]; // compute number of pins
float newValue[PIN_QTT] = {0};
float oldValue[PIN_QTT] = {0};
float cap_min[PIN_QTT]  = {0};
float cap_max[PIN_QTT]  = {0};
float cap_threshold[PIN_QTT] = {0};


bool wasTouched();
float smoothCap(int index);
uint8_t readCapacitivePin(int pinToMeasure);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void setup() {
#ifdef DEBUG_PRINT
  Serial.begin(115200);
#endif
  front.begin();
  back.begin();

  // default capacitive sensing extrema:
  for (int i = 0; i < PIN_QTT; i++) {
    cap_min[i] = 50;
    cap_max[i] = 50;
    smoothCap(i); // burn the 1st measures
  }
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void loop() {
  bool touch = wasTouched();

  if (touch) {
    breathe(front, NUMPIXELS_FRONT);
    setAllOff(back,  NUMPIXELS_BACK);
  }
  else
  {
    Animation(back, NUMPIXELS_BACK, 6);
    setAllOff(front,   NUMPIXELS_FRONT);
  }

}



//////////////////////////////////////////////////////////////////////////
// LEDs
//////////////////////////////////////////////////////////////////////////

void setAllOff(Adafruit_NeoPixel& strip, const int NUMPIXELS) {
  for (int i = 0; i < NUMPIXELS; i++)  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}



// LEDs breathing
void breathe(Adafruit_NeoPixel& strip, const int NUMPIXELS) {

  int amplitude = 20 + (exp(sin(millis() / 1000.0 * PI)) - 0.368) * 42.546;

  for (int j = 0; j < NUMPIXELS; j++) {
    strip.setPixelColor(j, strip.Color(amplitude, amplitude, amplitude));
  }
  strip.show();
}



// LEDs animation by blobs
void Animation(Adafruit_NeoPixel& strip, const int NUMPIXELS, const int NB_BLOBS) {

  const int MAX_BRIGHTNESS = 255;
  const int BRI = MAX_BRIGHTNESS;
  const int GAP = 8;
  static int scan = 0;

  for (int i = 0; i < NUMPIXELS; i++)  {

    strip.setPixelColor(i, strip.Color(0, 0, 0));

    for (int nb_blobs = 0; nb_blobs < NB_BLOBS; nb_blobs++) {
      if (i == scan - nb_blobs * GAP ) {
        PixelColorBrightness(strip, i, BRI, BRI, BRI, MAX_BRIGHTNESS);
      }
      else if ((i == scan - nb_blobs * GAP + 1) || (i == scan - nb_blobs * GAP - 1)) {
        PixelColorBrightness(strip, i, BRI, BRI, BRI, MAX_BRIGHTNESS * (2. / 3));
      }
      else if ((i == scan - nb_blobs * GAP + 2) || (i == scan - nb_blobs * GAP - 2)) {
        PixelColorBrightness(strip, i, BRI, BRI, BRI, MAX_BRIGHTNESS * (1. / 5));
      }
    }
  }
  scan = (scan + 1) % (NUMPIXELS + GAP * (NB_BLOBS - 1) + 2);

  strip.show();
}



void PixelColorBrightness(Adafruit_NeoPixel& pixels, uint16_t n,
                          uint8_t r, uint8_t g, uint8_t b, uint16_t brightness) {

  pixels.setPixelColor(n, (brightness * r / 255),
                          (brightness * g / 255),
                          (brightness * b / 255));
}



//////////////////////////////////////////////////////////////////////////
// capacitive sensing
//////////////////////////////////////////////////////////////////////////

bool wasTouched() {
  bool touch = 0;
  float measure = 0;

  for (int i = 0; i < PIN_QTT; i++) {
    measure = smoothCap(i);

    // update extrema
    cap_min[i] = min(measure, cap_min[i]);
    cap_max[i] = max(measure, cap_max[i]);

    // find thresholds:
    float range = cap_max[i] - cap_min[i];
    cap_threshold[i] = cap_min[i] + range / 2;

    // accumulate touch data (can be separate too)
    touch = touch || (measure > cap_threshold[i]);

#ifdef DEBUG_PRINT
    Serial.print( measure );
    Serial.print(" ");
#endif
  }

#ifdef DEBUG_PRINT
  // visualize (optional)
  float global_min = 9999;
  float global_max = 0;
  for (int i = 0; i < PIN_QTT; i++) {
    global_min = min(global_min, cap_min[i]);
    global_max = max(global_max, cap_max[i]);
  }
  Serial.print(global_min);
  Serial.print(" ");
  Serial.print(cap_threshold[0]);
  Serial.print(" ");
  Serial.print(cap_threshold[1]);
  Serial.print(" ");
  Serial.print(global_max);
  Serial.print(" ");
  Serial.println(" 0");
#endif

  return touch;
}



float smoothCap(int index) {
  const float COEF = 0.6;

  // read touch sensor value a 1st time:
  newValue[index] = readCapacitivePin(pins[index]);

  // improve estimation (more loops = more accurate but slower):
  for (int i = 0; i < 9; i++)
    newValue[index] += readCapacitivePin(pins[index]);

  // smooth filter
  newValue[index] = newValue[index] * COEF + oldValue[index] * (1 - COEF);

  // backup
  oldValue[index] = newValue[index];

  return newValue[index];
}



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

  uint8_t cycles = 20;
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
