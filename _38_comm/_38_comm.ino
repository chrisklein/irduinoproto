/*
 Chat  Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.

 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.


 Circuit:
 * WiFi shield attached

 created 18 Dec 2009
 by David A. Mellis
 modified 23 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "38alleys";     //  your network SSID (name) 
char pass[] = "3201999999";    // your network password
char servername[] = "twitter.com";

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient client;

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // start serial port:
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
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

void loop() {
  if(client.available()){  
    char c1 = client.read();
    Serial.print(c1);
  }
  // wait for a new client.
  
//  Serial.print("printing client :");
//  Serial.println(client);
//  Serial.print("is client connected :");
//  Serial.println(client.connected());

  // when the client sends the first byte, say hello:
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
  
  if(client.connect(servername, 80)){
    Serial.println("connected");
    client.println("GET /1/statuses/user_timeline.xml?screen_name=arduino HTTP/1.1");
    client.println(); 
    
  }
}
