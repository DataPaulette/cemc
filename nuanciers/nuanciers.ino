#include <Adafruit_NeoPixel.h>

// potentiometers: A0, A1, A2
#define BUTTON_PIN 3

// NeoPixel
#define NEOPIXEL_PIN 2  // NOT THE USUAL PIN !!
#define NUMPIXELS 300
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int LUM_MAX = 200;
const int ecart = 8;
int e = 0;
const int NB_BLOBS_PULSE = 10;
int buttonPin = 3;
int animation = 0;

void setup() {
  Serial.begin(115200);
  pinMode(3, INPUT_PULLUP);
  pixels.begin();
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    animation++;
    delay(1000);
    if (animation > 2) {
      animation = 0;
    }
    launchAnimation(animation);
  }

  // fix extremas (eye candy to remove eventually):
  Serial.print(" -20 260 ");
  Serial.print(" \t ");
  Serial.print(animation);
  Serial.print(" \t ");
  // button :
  Serial.println(buttonState);

  delay(100);  // Pause before next pass through loop
}

void launchAnimation(int animation) {
  switch (animation) {
    case 0:
      while (digitalRead(buttonPin)) {
        simple();
      }
      break;
    case 1:
      // nuancierOne();
      // nuancierTwo();
      nuancierThree();
      break;
    case 2:
      wave(NB_BLOBS_PULSE);
      break;
  }
}
// variation couleur potar (RGB)
void simple() {
  // Set all pixel colors to 'off'
  pixels.clear();
  // potentiometers are inverted and limited to 127
  int R = (255 - analogRead(A0) / 4) / 2;  // from 10 bits to 8 bits
  int G = (255 - analogRead(A1) / 4) / 2;  // from 10 bits to 8 bits
  int B = (255 - analogRead(A2) / 4) / 2;  // from 10 bits to 8 bits

  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 1
void nuancierOne() {
  // Set all pixel colors to 'off'
  pixels.clear();
  // potentiometers are inverted and limited to 127
  int R = (255 - analogRead(A0) / 4) / 2;  // from 10 bits to 8 bits
  int G = (255 - analogRead(A1) / 4) / 2;  // from 10 bits to 8 bits
  int B = (255 - analogRead(A2) / 4) / 2;  // from 10 bits to 8 bits

  //Bloc 1
  for (int i = 0; i < 141; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  //Bloc 2
  for (int i = 142; i < 222; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  //Bloc 3
  for (int i = 223; i < 285; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 2
void nuancierTwo() {
  // Set all pixel colors to 'off'
  pixels.clear();
  // potentiometers are inverted and limited to 127
  int R = (255 - analogRead(A0) / 4) / 2;  // from 10 bits to 8 bits
  int G = (255 - analogRead(A1) / 4) / 2;  // from 10 bits to 8 bits
  int B = (255 - analogRead(A2) / 4) / 2;  // from 10 bits to 8 bits
  //Bloc 1
  for (int i = 0; i < 15; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  for (int i = 112; i < 216; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  //Bloc 2
  for (int i = 16; i < 51; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 88; i < 105; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 217; i < 240; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 253; i < 282; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  //Bloc 3
  for (int i = 52; i < 87; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  for (int i = 106; i < 111; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  for (int i = 241; i < 252; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 3
void nuancierThree() {
  // Set all pixel colors to 'off'
  pixels.clear();
  // potentiometers are inverted and limited to 127
  int R = (255 - analogRead(A0) / 4) / 2;  // from 10 bits to 8 bits
  int G = (255 - analogRead(A1) / 4) / 2;  // from 10 bits to 8 bits
  int B = (255 - analogRead(A2) / 4) / 2;  // from 10 bits to 8 bits

  //Bloc 1
  for (int i = 0; i < 21; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  for (int i = 49; i < 63; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  for (int i = 130; i < 156; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  for (int i = 250; i < 267; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  for (int i = 268; i < 300; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, R, R));
  }
  //Bloc 2
  for (int i = 22; i < 48; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 85; i < 105; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 157; i < 189; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  for (int i = 223; i < 249; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(G, G, G));
  }
  //Bloc 3
  for (int i = 64; i < 84; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  for (int i = 105; i < 129; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  for (int i = 190; i < 222; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(B, B, B));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// animation wave color
void wave(const int& NB_BLOBS) {
  for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    for (int nb_blobs = 0; nb_blobs < NB_BLOBS; nb_blobs++) {
      if (i == e - nb_blobs * ecart) {
        PixelColorBrightness(pixels, i, 60, 200, 5, LUM_MAX);
      } else if ((i == e - nb_blobs * ecart + 1) || (i == e - nb_blobs * ecart - 1)) {
        PixelColorBrightness(pixels, i, 60, 200, 5, LUM_MAX * (2. / 3));
      } else if ((i == e - nb_blobs * ecart + 2) || (i == e - nb_blobs * ecart - 2)) {
        PixelColorBrightness(pixels, i, 60, 200, 5, LUM_MAX * (1. / 5));
      }
    }
  }
  e = (e + 1) % (NUMPIXELS + ecart * (NB_BLOBS - 1) + 2);
  pixels.show();
}
void PixelColorBrightness(Adafruit_NeoPixel& pixels, uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint16_t brightness) {
  pixels.setPixelColor(n, (brightness * r / 255), (brightness * g / 255), (brightness * b / 255));
}
void pixelsOff() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}