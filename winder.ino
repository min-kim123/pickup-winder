#include <LiquidCrystal.h>

//LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 13, 12);

//pins
const int reedPin = 4;
const int switchLeftPin = 5;
const int switchRightPin = 6;
const int motorIn1 = 3;
const int motorIn2 = 2;
const int motorEnablePin = 11;  

volatile int count = 0;
bool lastReedState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 100;

void setup() {
  //lcd
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Turns: ");
  lcd.setCursor(0, 1);
  lcd.print(count);

  //pins
  pinMode(reedPin, INPUT);  //external pull-up resistor in use
  pinMode(switchLeftPin, INPUT_PULLUP);
  pinMode(switchRightPin, INPUT_PULLUP);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);

}

void loop() {
  bool leftActive = digitalRead(switchLeftPin) == LOW;
  bool rightActive = digitalRead(switchRightPin) == LOW;
  analogWrite(motorEnablePin, 50); //speed = 100 out of 255

  //handle motor control
  if (leftActive) {
    //clockwise
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
  } else if (rightActive) {
    //counterclockwise
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
  } else {
    //off
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
  }

  bool currentReedState = digitalRead(reedPin);
  if (lastReedState == HIGH && currentReedState == LOW) {
    if (millis() - lastDebounceTime > debounceDelay) {
      //count based on direction
      if (leftActive) {
        count++;
      } else if (rightActive) {
        count--;
      }

      //update display
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(count);

      lastDebounceTime = millis();
    }
  }
  lastReedState = currentReedState;
  //delay(1000);
}
