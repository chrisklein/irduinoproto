/*
 
 GENERAL NOTES:
 For WEP or WPA, change the Wifi.begin() call accordingly.

 */

#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <aJSON.h>

#define BUFSIZE 255 // For some reason 255 seems to be the magic number

#define MQTT_SERVER "q.m2m.io"

char ssid[] = "";     //  your network SSID (name) 
char pass[] = "";    // your network password

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);   // Connect to port 80
WiFiClient wifiClient;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

const unsigned long requestInterval = 30*1000;    // delay between requests; 30 seconds

boolean requested;                     // whether you've made a request since connecting
unsigned long lastAttemptTime = 0;     // last time you connected to the server, in milliseconds

char message_buff[100];

void setup() {
  // start serial port:
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  // start the server:
  server.begin();

  // you're connected now, so print out the status:
  printWifiStatus();
}

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("callback hit");
  
  int i = 0;

  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));
  
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
  Serial.println("Payload: " + msgString);
}


void loop() {
  if (!client.connected()){
    // clientID, username, MD5 encoded password
    // .. connection code, but out for security reasons. 
  }
  
  // MQTT client loop processing
  client.loop();
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

