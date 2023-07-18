#include <Arduino.h>
#include "arduino_secrets.h"
#include "connection_matrix.h"
#include "WiFiS3.h"

ConnectionMatrix matrix;

#define CONNECT_TIME 20000
#define TIMEOUT_TIME 5000

WiFiServer server(80);


void setup() {

  Serial.begin(9600);
  matrix.begin();    
  matrix.bad();

  Serial.print("Attempting to connect to "); Serial.println(SECRET_SSID);
  WiFi.begin(SECRET_SSID, SECRET_PASS);

  size_t start_time = millis();
  size_t dot_time = millis();

  while ((WiFi.status() != WL_CONNECTED) && ((CONNECT_TIME + start_time) > millis())){
    if (dot_time + 250 < millis()){
      Serial.print(".");
      matrix.bad();
      dot_time = millis();
    }
  }

  Serial.println("\nConnected to Wifi");
  Serial.print("Arduino Local IP: "); Serial.println(WiFi.localIP());
  matrix.good();
}

void loop() {
  if (WiFi.status()!=WL_CONNECTED){
    matrix.bad();
    delay(250);
    matrix.off();
    delay(250);
  } else {
    matrix.good();
    delay(500);
  }
}

