#include <FastLED.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define LED_PIN 6          // Data pin connected to the LED strip
#define NUM_LEDS 62        // Number of LEDs in your strip (including separators)

#define GPS_RX_PIN 10      // RX pin for GPS module
#define GPS_TX_PIN 11      // TX pin for GPS module

CRGB leds[NUM_LEDS];
TinyGPSPlus gps;
SoftwareSerial ss(GPS_TX_PIN, GPS_RX_PIN);

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS); // Define LED strip type and pin
  FastLED.setBrightness(100);  // Set the brightness (0-255, adjust as needed)

  Serial.begin(9600); // Initialize hardware serial for debugging (optional)
  ss.begin(9600);     // Initialize SoftwareSerial for GPS communication
}

void loop() {
  Serial.println(".");
  while (ss.available() > 0) {

    if (gps.encode(ss.read())) {
      if (gps.time.isValid() && gps.date.isValid()) {
        int hour = gps.time.hour();
        int minute = gps.time.minute();
        int second = gps.time.second();
        int day = gps.date.day();
        int month = gps.date.month();
        int year = gps.date.year() - 2000; // Assume year in the 21st century

        displaySecondsInBinary(second);
        displayMinutesInBinary(minute);
        displayHoursInBinary(hour);
        displayDayInBinary(day);
        displayMonthInBinary(month);
        // Year display not implemented in this example

        showLEDs(); // Consolidate FastLED.show() calls

        delay(100); // Delay to update the display every second
      }
    }
  }
}

void displaySecondsInBinary(int second) {
  for (int led = 0; led < 6; led++) {
    int bitValue = (second >> led) & 1;
    leds[led] = bitValue == 1 ? CRGB::Red : CRGB::Gray;
  }
  // Insert a black LED here for separation
  leds[6] = CRGB::Black;
}

void displayMinutesInBinary(int minute) {
  for (int led = 7; led < 14; led++) {
    int bitValue = (minute >> (led - 7)) & 1;
    leds[led] = bitValue == 1 ? CRGB::Red : CRGB::Gray;
  }
  // Insert a black LED here for separation
  leds[14] = CRGB::Black;
}

void displayHoursInBinary(int hour) {
  for (int led = 15; led < 19; led++) {
    int bitValue = (hour >> (led - 15)) & 1;
    leds[led] = bitValue == 1 ? CRGB::Red : CRGB::Gray;
  }
  // Insert a black LED here for separation
  leds[19] = CRGB::Black;
}

void displayDayInBinary(int day) {
  for (int led = 20; led < 25; led++) {
    int bitValue = (day >> (led - 20)) & 1;
    leds[led] = bitValue == 1 ? CRGB::Red : CRGB::Gray;
  }
  // Insert a black LED here for separation
  leds[25] = CRGB::Black;
}

void displayMonthInBinary(int month) {
  for (int led = 26; led < 30; led++) {
    int bitValue = (month >> (led - 26)) & 1;
    leds[led] = bitValue == 1 ? CRGB::Red : CRGB::Gray;
  }
}

void showLEDs() {
  FastLED.show();
}
