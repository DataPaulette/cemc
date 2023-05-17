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

int animation = 0;
bool oldButtonState = 1; // bc pull up

void setup() {
  Serial.begin(115200);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pixels.begin();
}

void loop() {
  launchAnimation(animation);

  bool buttonState = digitalRead(BUTTON_PIN);
  // detect falling edge  
  if (oldButtonState == 1 && buttonState == 0) {
    animation++;
    if (animation > 2) {
      animation = 0;
    }
  }
  oldButtonState = buttonState;

  Serial.print(animation);
  Serial.print(" \t ");
  // button :
  Serial.println(buttonState);
}

void launchAnimation(int animation) {
  const float COEF = 0.4;
  static float oldP1 = 0;
  static float oldP2 = 0;
  static float oldP3 = 0;

  // potentiometers are inverted and limited to 127
  int P1 = (255 - analogRead(A0) / 4) / 2;  // from 10 bits to 7 bits
  int P2 = (255 - analogRead(A1) / 4) / 2;  // from 10 bits to 7 bits
  int P3 = (255 - analogRead(A2) / 4) / 2;  // from 10 bits to 7 bits
  P1 = P1 * COEF + oldP1 * (1-COEF);
  P2 = P2 * COEF + oldP2 * (1-COEF);
  P3 = P3 * COEF + oldP3 * (1-COEF);
  oldP1 = P1;
  oldP2 = P2;
  oldP3 = P3;

  Serial.print(P1);
  Serial.print(" \t ");
  Serial.print(P2);
  Serial.print(" \t ");
  Serial.print(P3);
  Serial.print(" \t ");

  switch (animation) {
    case 0:
     simple(P1, P2, P3);      
      break;
    case 1:
    //Select nuancier
      // nuancierOne(P1, P2, P3);
      nuancierTwo(P1, P2, P3);
      // nuancierThree(P1, P2, P3);
      break;
    case 2:
      wave(NB_BLOBS_PULSE, P1, P2, P3);
      break;
  }
}

// variation couleur potar (RGB)
void simple(int R, int G, int B ) {
  R *= 0.9;
  G *= 0.9;
  // Set all pixel colors to 'off'
  pixels.clear();

  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 1
void nuancierOne(int P1, int P2, int P3) {
  // Set all pixel colors to 'off'
  pixels.clear();
  //Bloc 1
  for (int i = 0; i < 141; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  //Bloc 2
  for (int i = 142; i < 222; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  //Bloc 3
  for (int i = 223; i < 285; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 2
void nuancierTwo(int P1, int P2, int P3) {
  // Set all pixel colors to 'off'
  pixels.clear();

  //Bloc 1
  for (int i = 0; i < 15; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  for (int i = 108; i < 213; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  //Bloc 2
  for (int i = 15; i < 48; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 84; i < 102; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 213; i < 237; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 249; i < 282; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  //Bloc 3
  for (int i = 48; i < 84; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  for (int i = 102; i < 108; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  for (int i = 237; i < 249; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// variation intensity potar (1=bloc-1 / 2=bloc-2 / 3=bloc-3) -> nuancier 3
void nuancierThree(int P1, int P2, int P3) {
  // Set all pixel colors to 'off'
  pixels.clear();
  //Bloc 1
  for (int i = 0; i < 21; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  for (int i = 48; i < 63; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  for (int i = 129; i < 156; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  for (int i = 249; i < 268; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  for (int i = 268; i < 300; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P1, P1, P1));
  }
  //Bloc 2
  for (int i = 21; i < 48; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 84; i < 105; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 156; i < 189; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  for (int i = 222; i < 249; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P2, P2, P2));
  }
  //Bloc 3
  for (int i = 63; i < 84; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  for (int i = 105; i < 129; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  for (int i = 189; i < 222; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(P3, P3, P3));
  }
  pixels.show();  // Send the updated pixel colors to the hardware.
}
// animation wave color
void wave(const int& NB_BLOBS, int R, int G, int B) {
  for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    for (int nb_blobs = 0; nb_blobs < NB_BLOBS; nb_blobs++) {
      if (i == e - nb_blobs * ecart) {
        PixelColorBrightness(pixels, i, R, G, B, LUM_MAX);
      } else if ((i == e - nb_blobs * ecart + 1) || (i == e - nb_blobs * ecart - 1)) {
        PixelColorBrightness(pixels, i, R, G, B, LUM_MAX * (2. / 3));
      } else if ((i == e - nb_blobs * ecart + 2) || (i == e - nb_blobs * ecart - 2)) {
        PixelColorBrightness(pixels, i, R, G, B, LUM_MAX * (1. / 5));
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