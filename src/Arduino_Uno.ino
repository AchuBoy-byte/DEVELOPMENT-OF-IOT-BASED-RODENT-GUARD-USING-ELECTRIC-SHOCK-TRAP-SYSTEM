#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define IR_PIN 9          // PIR sensor pin
#define RELAY_PIN 7        // Relay control pin
#define SERVO_PIN 11        // Servo motor control pin
#define BUZZER_PIN 5       // Buzzer control pin
#define RESET_BUTTON_PIN 6 // Reset button pin

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD object

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX pins for ESP-01S
Servo servo; // Servo motor object

bool isTrapOpen = true; // Variable to track trap door status
bool isTrapReset = true; // Variable to track trap reset status

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  servo.attach(SERVO_PIN);

  Serial.begin(9600); // Initialize serial communication
  espSerial.begin(9600);

  displayWelcomeMessage(); // Display welcome messages
}

void loop() {
  if (isTrapReset) {
    displayAwaitingMessage(); // Display awaiting message if trap is reset
    if (digitalRead(IR_PIN) == LOW) {
      activateTrap(); // Activate trap if PIR sensor detects motion
      pestCaught(); // Send PIR sensor data to ESP-01S
    }
  }

  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    resetTrap(); // Reset trap if reset button is pressed
    pestAwaiting(); // Send Button data to ESP-01S
  }
}

void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IoT RODENT GUARD");
  Serial.println("System Start");
  Serial.println("Pest Awaiting");
  delay(5000);
}

void displayAwaitingMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PEST AWAITING");
}

void activateTrap() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PEST CAUGHT");

  isTrapReset = false; // Trap is now activated, reset button disabled

  servo.write(120); // Close the trap door
  delay(2000); // Allow some time for the door to close

  digitalWrite(RELAY_PIN, HIGH); // Turn on electric shock panel
  tone(BUZZER_PIN, 500); // Activate buzzer
  delay(20000); // Buzzer will sound for 20 seconds or until reset

  digitalWrite(RELAY_PIN, LOW); // Turn off electric shock panel
  noTone(BUZZER_PIN); // Turn off buzzer
}

void resetTrap() {
  servo.write(0); // Open the trap door
  delay(2000); // Allow some time for the door to open

  isTrapOpen = true;
  isTrapReset = true;

  displayAwaitingMessage(); // Display awaiting message
}

void pestCaught() {
  espSerial.println("#"); // Send PIR sensor data to ESP-01S
  Serial.println("Pest Caught");
}

void pestAwaiting() {
  espSerial.println("*"); // Send PIR sensor data to ESP-01S
  Serial.println("Pest Awaiting");
}
