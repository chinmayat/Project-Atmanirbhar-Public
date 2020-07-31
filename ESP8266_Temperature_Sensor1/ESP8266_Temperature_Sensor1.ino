/**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include "DHT.h"

#define DHTPIN D4 
#define DHTTYPE DHT11



//-------- Customise these values -----------
const char* ssid = "SwXXXXe";
const char* password = "XXXXXXXXXX1";

#define ORG "vXXXXXw"
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "0xxxx1B"
#define TOKEN "4xxxxxxxxxxxxxxxc"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic1[] = "iot-2/evt/status1/fmt/json";
char topic2[] = "iot-2/evt/status2/fmt/json";

char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 Serial.begin(115200);
 Serial.println();

 Serial.print("Connecting to "); Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 } 
 Serial.println("");

 Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

int counter = 0;

void loop() {

 if (!!!client.connected()) {
   Serial.print("Reconnecting client to ");
   Serial.println(server);
   while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
   }
   Serial.println();
 }
// String payload = "{"d":{"myName":"ESP8266.Test1","counter":";
 //payload += counter;
 //payload += "}}";

float humidity = dht.readHumidity();
         float temperature = dht.readTemperature();

String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload += ",\"temperature\":";
              payload += temperature;
              payload += "}}";
 
 
 Serial.print("Sending payload: ");
 Serial.println(payload);
 
 if (client.publish(topic1, (char*) payload.c_str())) {
   Serial.println("Publish ok");
 } else {
   Serial.println("Publish failed");
 }
 delay(10000);
}
