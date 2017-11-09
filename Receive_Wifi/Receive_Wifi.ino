/*
 *  Simple HTTP get webclient test
 */

/* librairie to add http://arduino.esp8266.com/stable/package_esp8266com_index.json , outils(tyoe de carte-gestion des cartes-télécharger dernier en bas),outil type de carte adafruit huzza */

#include <ESP8266WiFi.h>

const char* ssid     = "rover1";
const char* password = "12345";

const char* host = "wifitest.adafruit.com";

void setup() {
  Serial.begin(9600);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 23;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\n');
    Serial.print(line);
  }
}
