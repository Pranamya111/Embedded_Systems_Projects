
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial bluetooth(2, 8);

const int relayPin = 3;                  // Pin number 3 of arduino connected to Relay
int relayState = 0;


const int mpin = 13;                        // Initializing pin number
int mstate = 0;

const int sw1 = 4;
int sw1state = 0;
const int sw2 = 5;
int sw2state = 0;

int d = 0;


void setup() {
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(mpin, INPUT);
  pinMode(relayPin, OUTPUT);
  relayState = LOW;
  digitalWrite(relayPin, relayState);
  delay(1000);
  bluetooth.begin(9600);
  Serial.begin(9600);


  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.print("   Irrigation");
  lcd.setCursor(0, 1);                    // Sets cursor at Column 0 and Line 1
  lcd.print("     System");
  delay(2000);

}

void loop() {
loop0:
  lcd.clear();
  lcd.print("1: Auto");
  lcd.setCursor(0, 1);                    // Sets cursor at Column 0 and Line 1
  lcd.print("2: Manual");
  sw1state = digitalRead(sw1);
  sw2state = digitalRead(sw2);
  if (sw1state == LOW)
  {
    lcd.clear();
    lcd.print("Auto Mode");
    Serial.print("Auto Mode");
    goto AUTO;
  }
  if (sw2state == LOW)
  {
    lcd.clear();
    lcd.print("Manual mode");
    Serial.print("Manual mode");
    goto MANUAL;
  }
  delay(250);
  goto loop0;

AUTO:
  sw2state = digitalRead(sw2);
  if (sw2state == LOW)
  {
    lcd.clear();
    lcd.print("Manual Mode");
    Serial.print("Manual Mode");
    bluetooth.print("Manual Mode");
    goto MANUAL;

  }
  mstate = digitalRead(mpin);
  if (mstate == HIGH)  //no moisture
  {
    relayState = HIGH;   //relay ON
    digitalWrite(relayPin, relayState);
    delay(500);
    lcd.clear();     // Clears LCD
    lcd.setCursor(0, 0);
    lcd.print("    MOTOR ON");
    lcd.setCursor(0, 1);
    lcd.print(" MOISTUREabsent");
    bluetooth.println("MOTOR ON");
    bluetooth.println("MOISTUREabsent");
    Serial.println("MOTOR ON");
    Serial.println("MOISTUREabsent");
    delay(1000);
  }
  if (mstate == LOW) //moisture present
  {
    relayState = LOW;  //relay OFF
    digitalWrite(relayPin, relayState);
    delay(500);
    lcd.clear();     // Clears LCD
    lcd.print("MOTOR OFF");
    lcd.setCursor(0, 1);
    lcd.print("MOISTUREpresent");
    bluetooth.println("MOTOR OFF");
    bluetooth.print("MOISTUREpresent");
    Serial.println("MOTOR OFF");
    Serial.print("MOISTUREpresent");
    delay(1000);
  }


  goto AUTO;


MANUAL:
sw1state = digitalRead(sw1);
  
if (sw1state == LOW)
{
  lcd.print("Auto Mode");
  Serial.print("Auto Mode");
  bluetooth.print("Auto Mode");
  goto AUTO;

}

  while (bluetooth.available())        // Check if any data received in RX(Pin 2) of bluetooth
  {
    delay(200);
    if (bluetooth.find("$"))           // Search for a symbol $ in the data sent via buetooth
    {
      delay(200);
      char d = bluetooth.read();
      lcd.print(d);                    // Print the value of d
      delay(1000);
      if (d == '1')
      {
        relayState = HIGH;  //relay on
        digitalWrite(relayPin, relayState);
        lcd.clear();     // Clears LCD
        lcd.print("MOTOR ON ");
        lcd.setCursor(0, 1);
        lcd.print(" MOISTUREabsent");
        bluetooth.println("MOTOR ON ");
        Serial.println("MOTOR ON");
        delay(5000);
      }
      if (d == '2')
      {
        relayState = LOW;
        digitalWrite(relayPin, relayState);
        lcd.clear();     // Clears LCD
        lcd.print("MOTOR OFF ");
        lcd.setCursor(0, 1);
        lcd.print("MOISTUREpresent");
        bluetooth.println("MOTOR OFF  ");
        Serial.println("MOTOR OFF");
        delay(1000);
      }
    } 
    d = 0;
  }
  goto MANUAL;
}


//
//if moisture sensor is not shorted then moisture is absent so relay should ON
//if moisture sensor is shorted then moisture is present so relay should OFF
