#define BLYNK_TEMPLATE_ID "*****"
#define BLYNK_TEMPLATE_NAME "*****"
#define BLYNK_AUTH_TOKEN "*****"

//Include necessary libraries
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



//Define WiFi credentials
char ssid[] = "*****";
char pass[] = "*****";



BlynkTimer timer;

//Setup function
void setup() {

  Serial.begin(9600); // Initialize serial communication with Arduino Uno

  WiFi.begin(ssid, pass);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  //Print connection status
  Serial.println("Connected to WiFi");
}


void loop() {
  Blynk.run();
  timer.run();

  if (Serial.available()) {
    char inChar = (char)Serial.read(); 

    if (inChar == '#') {
      // Where newline character is received, process the message
      Blynk.logEvent("pest_caught", "PEST CAUGHT");
    }

    if (inChar == '*') {
      // Where newline character is received, process the message
      Blynk.logEvent("pest_awaiting", "PEST AWAITING");
  }
}
}

