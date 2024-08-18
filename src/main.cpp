#include <Arduino.h>
#include <HomeSpan.h>
#include "DEV_LED.h"


#define NUM_LEDS 19
#define LED_PIN 16



void setup() {
  Serial.begin(115200);

	homeSpan.begin(Category::Lighting, "Homespan RGB");

	new SpanAccessory();
	new Service::AccessoryInformation();
	new Characteristic::Identify();
	/*new Characteristic::Name("RGB LED");*/
	new DEV_RGB_LED(LED_PIN, NUM_LEDS);
}

void loop() {
	homeSpan.poll();
}

