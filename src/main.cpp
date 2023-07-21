#include <Arduino.h>
#include "arduino_secrets.h"
#include "connection_matrix.h"
#include "WiFiS3.h"

ConnectionMatrix matrix;

#define CONNECT_TIME 20000
#define TIMEOUT_TIME 5000

WiFiServer server(80);

//Loop variables
size_t currentTime  = 0;  
size_t previousTime = 0;
String currentLine = "";
String header = "";
String ledBuiltInState = "OFF";

void setup() {

  Serial.begin(9600);
  matrix.begin();    
  matrix.render(OFF);

  Serial.print("Attempting to connect to "); Serial.println(SECRET_SSID);
  WiFi.begin(SECRET_SSID, SECRET_PASS);

  size_t startTime = millis();
  size_t dotTime = millis();

  while ((WiFi.status() != WL_CONNECTED) && ((CONNECT_TIME + startTime) > millis())){
    if (dotTime + 250 < millis()){
      Serial.print(".");
      matrix.render(BAD);
      dotTime = millis();
    }
  }
  // If connection failed while(1) --> you need to restart
  if (WiFi.status()!= WL_CONNECTED) {
    Serial.println("Connection failed!!!");
    while(1);
  }

  // Connection GOOD!!!
  Serial.println("\nConnected to Wifi");
  Serial.print("Arduino Local IP: "); Serial.println(WiFi.localIP());
  matrix.render(GOOD);
  server.begin();

  // Setup led built in
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  WiFiClient client = server.available();

  // If there is a client that is pinging the server (Arduino) the if will be executed
  // If not the client object is empty
  if (client){
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New client detected");
    currentLine = "";

    while(client.connected() && (currentTime - previousTime) <= TIMEOUT_TIME){
      currentTime = millis();
      if(client.available()){
        char c = client.read();
        Serial.print(c);
        header += c;

        // If c == \n and lenght of currentLine == 0 the client request is completed
        if (c == '\n') {
          if (currentLine.length() ==0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println("");

            if (header.indexOf("GET /on") >= 0){
              Serial.println("LED ON");
              digitalWrite(LED_BUILTIN, HIGH);
              ledBuiltInState = "ON";
              matrix.render(ON_TXT);
            } else if (header.indexOf("GET /off") >= 0){
              Serial.println("LED OFF");
              digitalWrite(LED_BUILTIN, LOW);
              ledBuiltInState = "OFF";
              matrix.render(OFF_TXT);
            }
            break;
            
          } else {
            currentLine = "";
          }
        } else if(c != '\r'){
          currentLine += c;
        }
      }
    }

    // Once the while is done the response to the client is completed
    header = "";
    client.stop();
    Serial.println("Client disconnected");  
  }
  delay(1);
}

