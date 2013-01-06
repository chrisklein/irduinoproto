/*
 
 GENERAL NOTES:
 For WEP or WPA, change the Wifi.begin() call accordingly.

 */

#include <SPI.h>
#include <WiFi.h>
#include <aJSON.h>

#define BUFSIZE 255 // For some reason 255 seems to be the magic number

char ssid[] = "38alleys";     //  your network SSID (name) 
char pass[] = "3201999999";    // your network password
char servername[] = "h2lo-api.herokuapp.com";  // the server we're going to pull data from

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);  // Connect to port 80
WiFiClient client;      // Define our WifiClient

const unsigned long requestInterval = 30*1000;    // delay between requests; 30 seconds

char json[] = "{\"id\":\"1\"}";  // Sample json for testing aJSON

boolean requested;                     // whether you've made a request since connecting
unsigned long lastAttemptTime = 0;     // last time you connected to the server, in milliseconds

String currentLine = "";               // string to hold the text from server
String jsonString = "";                // string to hold the json
boolean readingJsonString = false;     // if we're currently reading json
String personId = "";                  // string to hold the id of person json
boolean readingPersonId = false;       // if we're currently reading the tweet


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

  boolean isConnected = connectToServer();
  // Not sure we need this yet.
  // if(isConnected){ fetchData(); }
}


void loop() {
  if(client.connected()){
    // Loop code if client connected
    
    if(client.available()){
      // client available code
      fetchData();
    }
    
  }else{
    checkConnectionToServer();
  }
}


void fetchData(){
  char inChar = client.read();

  // add incoming byte to end of line:
  currentLine += inChar; 

  // if you get a newline, clear the line:
  if (inChar == '\n') {
    currentLine = "";
  }  
  
  // if the current line starts with {, it will be
  // followed by the json
  if(currentLine.endsWith("{")){
    // json is beginging.  Clear the json string:
    readingJsonString = true;
    jsonString = "";
  }
  
  // if we're currently reading the bytes of our json,
  // add them to the json String:
  if(readingJsonString){
    if (inChar != '}') {
      jsonString += inChar;
    } else {
       // If we've got a }, we know we're at the end
       // of our json (only in this instanc of course.
       jsonString += inChar;
       readingJsonString = false;
       Serial.println(jsonString);
       client.stop();
    }
  }
  
}


void checkConnectionToServer(){
  if (millis() - lastAttemptTime > requestInterval) {
    Serial.println("lost connection, attempting to reconnect");
    // if we're not connected, and two minutes have passed since
    // our last connection, then attempt to connect again:
    connectToServer();
  }
}


boolean connectToServer(){
  if(client.connect(servername, 80)){
    Serial.println("connected");
    client.println("GET /service/person/5 HTTP/1.1");
    client.println("Host: h2lo-api.herokuapp.com");
    client.println(); 
    return true;
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
  return false;
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
