#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Define pins for the LCD
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial bluetooth(2, 8);

// Define motor pins
const int motorLeftForward = 10;
const int motorLeftBackward = 11;
const int motorRightForward = 7;
const int motorRightBackward = 9;

// Define IR sensor pin
const int irPin = 13;
int irState = 0;

// Bluetooth setup
char voiceCommand;

// Setup
void setup() {
  // Initialize LCD
  lcd.init();
  lcd.print("Voice Control");

  // Setup motor pins as output
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  // Setup IR sensor as input
  pinMode(irPin, INPUT_PULLUP);

  // Initialize serial communication
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Welcome message
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check for obstacle
 irState = digitalRead(irPin);       // Check for Obstacle Detection----

  if (irState == LOW) { 
    stopRobot();
    lcd.clear();
    lcd.print("Obstacle Detected");
    delay(1000);
    return;
  }

  // Check for Bluetooth commands
  if (bluetooth.available()) {
    char voiceCommand = bluetooth.read();
    executeCommand(voiceCommand);
  }
}

// Function to execute commands
void executeCommand(char command) {
  lcd.clear();
  switch (command) {
    case 'F': // Forward
      lcd.print("Moving Forward");
      moveForward();
      break;

    case 'B': // Backward
      lcd.print("Moving Backward");
      moveBackward();
      break;

    case 'L': // Left
      lcd.print("Turning Left");
      turnLeft();
      break;

    case 'R': // Right
      lcd.print("Turning Right");
      turnRight();
      break;

    case 'S': // Stop
      lcd.print("Stopping");
      stopRobot();
      break;

    default:
      lcd.print("Invalid Command");
      stopRobot();
      break;
  }
}

// Movement functions
void moveForward() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void moveBackward() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}

void turnLeft() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void turnRight() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
}

void stopRobot() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
}
