//Nyt on kuiskailtu Blondille joka  värjäsi hiukset ruskeaksi - tekoälylle.
//Laitan alle koodihahmotelman, jos tsiigailet öögillä ja kerrot parannukset...

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FastLED.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* url = "https://api.spot-hinta.fi/TodayAndDayForward?HomeAssistant=true";

#define DATA_PIN D1 // Define the data pin for your LED matrix
#define NUM_LEDS_X 8 // Number of LEDs in the X direction (columns)
#define NUM_LEDS_Y 24 // Number of LEDs in the Y direction (rows)
CRGB leds[NUM_LEDS_X * NUM_LEDS_Y];

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS_X * NUM_LEDS_Y);
  FastLED.setBrightness(50);

  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Fetch data from API and display on LED matrix
    fetchAndDisplay();
  }
  
  // Delay before the next iteration
  delay(60000); // Wait for a minute before making the next request
}

void connectToWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void fetchAndDisplay() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect("api.spot-hinta.fi", 443)) {
      Serial.println("Connected to API");

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: api.spot-hinta.fi\r\n" +
                   "Connection: close\r\n\r\n");

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, client);

      if (!error) {
        double priceNoTax = doc["PriceNoTax"];
        int valueColumn = map(priceNoTax, 0, 100, 0, NUM_LEDS_X - 1);

        CRGB color;
        if (priceNoTax < 33.33) {
          color = CRGB::Green;
        } else if (priceNoTax < 66.66) {
          color = CRGB::Yellow;
        } else {
          color = CRGB::Red;
        }

        FastLED.clear();
        for (int y = 0; y < NUM_LEDS_Y; y++) {
          leds[y * NUM_LEDS_X + valueColumn] = color;
        }

        FastLED.show();

        Serial.print("Value Column: ");
        Serial.println(valueColumn);
        Serial.print("PriceNoTax: ");
        Serial.println(priceNoTax);
      } else {
        Serial.println("Error parsing JSON");
      }
    } else {
      Serial.println("Connection to API failed");
    }

    client.stop();
  }
}
