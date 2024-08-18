// DEV_LED.h:
//
#include "Adafruit_NeoPixel.h"
#include <Arduino.h>
#include <HomeSpan.h>
#include <cmath>

struct RGB {
    int r;
    int g;
    int b;
};

RGB hsvToRgb(float h, float s, float v) {
    s /= 100;
    v /= 100;
    
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;
    
    float r_prime, g_prime, b_prime;

    if (0 <= h && h < 60) {
        r_prime = c;
        g_prime = x;
        b_prime = 0;
    } else if (60 <= h && h < 120) {
        r_prime = x;
        g_prime = c;
        b_prime = 0;
    } else if (120 <= h && h < 180) {
        r_prime = 0;
        g_prime = c;
        b_prime = x;
    } else if (180 <= h && h < 240) {
        r_prime = 0;
        g_prime = x;
        b_prime = c;
    } else if (240 <= h && h < 300) {
        r_prime = x;
        g_prime = 0;
        b_prime = c;
    } else if (300 <= h && h < 360) {
        r_prime = c;
        g_prime = 0;
        b_prime = x;
    } else {
        r_prime = 0;
        g_prime = 0;
        b_prime = 0;
    }
    
    RGB rgb;
    rgb.r = static_cast<int>(round((r_prime + m) * 255));
    rgb.g = static_cast<int>(round((g_prime + m) * 255));
    rgb.b = static_cast<int>(round((b_prime + m) * 255));
    
    return rgb;
}


struct DEV_RGB_LED : Service::LightBulb {

  SpanCharacteristic *power;
  SpanCharacteristic *H;
  SpanCharacteristic *S;
  SpanCharacteristic *V;

  Adafruit_NeoPixel *WS2812B;

  DEV_RGB_LED(int pin, int num_leds) : Service::LightBulb() {
    WS2812B = new Adafruit_NeoPixel(num_leds, pin);
    WS2812B->begin();
    /*WS2812B->show();*/

    power = new Characteristic::On();
    H = new Characteristic::Hue(0);
    S = new Characteristic::Saturation(0);
    V = new Characteristic::Brightness(0);
    V->setRange(0, 100, 1);
  }

  bool update() {
    bool p = power->getVal();
    float h, s, v, r, g, b;

    h = H->getVal();
    s = S->getVal();
    v = V->getVal();

    if (power->updated()) {
      p = power->getNewVal();
    }

    if (p == false) {
      WS2812B->clear();
      WS2812B->show();
      return true;
    }

    if (H->updated()) {
      h = H->getNewVal();
    }

    if (S->updated()) {
      s = S->getNewVal();
    }

    if (V->updated()) {
      v = V->getNewVal();
    }

		char cBuffer[255];
		sprintf(cBuffer, "H: %f, S: %f, V: %f\n", h, s, v);
		LOG1(cBuffer);
		RGB rgb = hsvToRgb(h, s, v);
		sprintf(cBuffer, "R: %d, G: %d, B: %d\n", rgb.r, rgb.g, rgb.b);
		LOG1(cBuffer);

    WS2812B->fill(WS2812B->Color(rgb.r, rgb.g, rgb.b));
    WS2812B->show();
    return true;
  }
};
