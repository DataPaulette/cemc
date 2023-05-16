#include <Adafruit_NeoPixel.h>

// potentiometers: A0, A1, A2
#define BUTTON_PIN     3

// NeoPixel
#define NEOPIXEL_PIN  2    // NOT THE USUAL PIN
#define NUMPIXELS     300
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_BGR + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  pinMode(3, INPUT_PULLUP);
  pixels.begin();
}


void loop() {
  // potentiometers are inverted
  int R = 255-analogRead(A0) / 4 ; // from 10 bits to 8 bits
  int G = 255-analogRead(A1) / 4 ; // from 10 bits to 8 bits
  int B = 255-analogRead(A2) / 4 ; // from 10 bits to 8 bits

  Serial.print(R);  Serial.print(" ");
  Serial.print(G);  Serial.print(" ");
  Serial.print(B);
  
  // fix extremas (eye candy to remove eventually):
  Serial.print(" -20 260 ");

  // button :
  Serial.println(digitalRead(3)*260 - 10);

  // NeoPixel tests:
  pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}
