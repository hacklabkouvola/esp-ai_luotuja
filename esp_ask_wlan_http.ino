#include <ESP8266WiFi.h>

const char *apSsid = "ESP8266_AP";
const char *apPassword = "password";
IPAddress apIP(192, 168, 1, 1); // Static IP for AP mode

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Set IP configuration
  WiFi.softAP(apSsid, apPassword);

  // Print AP IP address
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Configure the web server
  server.on("/", HTTP_GET, []() {
    String html = "<html><body>";
    html += "<h1>Configure Wi-Fi</h1>";
    html += "<form method='get' action='setwifi'>";
    html += "SSID: <input type='text' name='ssid'><br>";
    html += "Password: <input type='password' name='password'><br>";
    html += "<input type='submit' value='Connect'>";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/setwifi", HTTP_GET, []() {
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");

    Serial.println("Setting Wi-Fi credentials...");
    Serial.print("New SSID: ");
    Serial.println(newSSID);
    
    // Attempt to connect to the specified Wi-Fi network
    WiFi.begin(newSSID.c_str(), newPassword.c_str());

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }

    Serial.println("\nConnected to Wi-Fi! All OK.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    
    server.send(200, "text/plain", "Connected to Wi-Fi! All OK.");
  });

  server.begin();
}

void loop() {
  server.handleClient();
  
  // If Wi-Fi connection fails, reboot the ESP8266
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection failed. Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

