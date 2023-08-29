#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <TimeLib.h>

#define LED_PIN    5
#define NUM_LEDS   60

CRGB leds[NUM_LEDS];

const char *ssid = "aotiot";
const char *password = "joojoojoo";

const char* ntp_server_name = "pool.ntp.org";
const int NTP_PACKET_SIZE = 48;
byte packet_buffer[NTP_PACKET_SIZE];
WiFiUDP udp;

const int time_zone = 3;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  connectToWiFi();
  setupNTP();
}

void loop() {
  if (timeStatus() == timeSet) {
    displayTime();
  }
  delay(500);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setupNTP() {
  udp.begin(123);
  setSyncProvider(getNtpTime);
  setSyncInterval(300); // Sync time every 5 minutes
}

time_t getNtpTime() {
  sendNTPpacket();
  delay(1000);
  
  if (udp.parsePacket()) {
    udp.read(packet_buffer, NTP_PACKET_SIZE);
    unsigned long secs_since_1900 =  (unsigned long)packet_buffer[40] << 24 | 
                                      (unsigned long)packet_buffer[41] << 16 |
                                      (unsigned long)packet_buffer[42] << 8 | 
                                      (unsigned long)packet_buffer[43];
    return secs_since_1900 - 2208988800UL + time_zone * SECS_PER_HOUR;
  }

  Serial.println("No NTP Response :-(");
  return 0;
}

void sendNTPpacket() {
  memset(packet_buffer, 0, NTP_PACKET_SIZE);
  packet_buffer[0] = 0b11100011;
  packet_buffer[1] = 0;
  packet_buffer[2] = 6;
  packet_buffer[3] = 0xEC;
  packet_buffer[12] = 'p';
  packet_buffer[13] = 'o';
  packet_buffer[14] = 'o';
  packet_buffer[15] = 'l';
  udp.beginPacket(ntp_server_name, 123);
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void displayTime() {
  time_t current_time = now();
  int hours = hour(current_time);
  int minutes = minute(current_time);
  int seconds = second(current_time);

  // Calculate the position for each hand on the clock face (0-59)
//  int hours_led_index = map(((hours % 12) * 5 + minutes / 12), 0, 59, 0, NUM_LEDS - 1); // Corrected mapping
//  int hours_led_index = map(((hours % 12) * 5 + minutes / 12), 0, 59, 0, NUM_LEDS - 1);

  //  int hours_led_index = map((hours % 12) * 5 + map(minutes, 0, 59, 0, 4), 0, 59, 0, NUM_LEDS - 1);
//    int hours_led_index = map((hours % 12) * 5 + minutes / 12, 0, 59, 0, NUM_LEDS - 1);
    int hours_led_index = map((hours % 12) * 5 + (minutes * 5) / 60, 0, 59, 0, NUM_LEDS - 1);



  int minutes_led_index = map(minutes, 0, 59, 0, NUM_LEDS - 1);
  int seconds_led_index = map(seconds, 0, 59, 0, NUM_LEDS - 1);

  
  Serial.println(hours_led_index);
  Serial.println(hours);
    
  Serial.println(minutes_led_index);
  Serial.println(minutes);
  
  Serial.println(seconds_led_index);  
  Serial.println(seconds);


  fill_solid(leds, NUM_LEDS, CRGB::Black);
  
  
  
//  leds[seconds_led_index] = CRGB::Blue;
//  leds[minutes_led_index] = CRGB::Green;
  leds[hours_led_index] = CRGB::Red;
  leds[seconds_led_index] = CRGB::Blue;
  leds[minutes_led_index] = CRGB::Green;

  if (hours_led_index == minutes_led_index) {
    leds[hours_led_index] = CRGB::Yellow;
  }


  FastLED.show();
}

