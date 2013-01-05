
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
#include <aJSON.h>

char ssid[] = "38alleys";     //  your network SSID (name) 
char pass[] = "3201999999";    // your network password
char servername[] = "h2lo-api.herokuapp.com";

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient client;

char json[] = "{\"id\":\"1\"}";

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

#define BUFSIZE 255

void loop() {
    
//    char rspValue[1];
//    rspValue[0] = rsp;
//    
//    char* idValue;
//    
//    aJsonObject* jsonObject1 = aJson.parse(rspValue);
//    aJsonObject* personId1 = aJson.getObjectItem(jsonObject1, "id");
//    idValue = personId1->valuestring;
//    Serial.print("json rsp = ");
//    Serial.print(rsp);
//    Serial.print(" rspValue = ");
//    Serial.print(rspValue);
//    Serial.print("id = ");
//    Serial.println(idValue);
//    char* value;
//    delay(1000);
    
//    aJsonObject* jsonObject = aJson.parse(json);
//    aJsonObject* personId = aJson.getObjectItem(jsonObject, "id");
//    value = personId->valuestring;
//    Serial.print(value);
    
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
    client.println("GET /service/person/5 HTTP/1.1");
    client.println("Host: h2lo-api.herokuapp.com");
    client.println();  
    
    
    char clientLine[BUFSIZE];
    int index = 0;
    
    while(client.connected()){
      if(client.available()){  
        Serial.println("available");
        char c = client.read();
        
      //  fill buffer with url
        if(c != '\n' && c != '\r'){
  
          // s if we run out of buffer, overwrite the end
          if(index >= BUFSIZE) {
            break;
            //index = BUFSIZE -1;
          }
  
          clientLine[index] = c;
          index++;
  
  //          Serial.print("client-c: ");
  //          Serial.println(c);
          continue;
        }
        
        
      } // end if
      String urlString = String(clientLine);
      Serial.println(urlString);
      client.stop();
    } // end while
    
    
    
    
    
  }
}
