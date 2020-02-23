#include <SoftwareSerial.h>

int trigPin = 9;
int echoPin = 10;
int redLed = 13;
int greenLed = 12;
int buzzPin = 11;
long duration, cm, inches;
bool isAlarming = false;

// Pin Definitions
#define ESP8266_PIN_RX  0
#define ESP8266_PIN_TX  1

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  digitalWrite(greenLed, HIGH);
  noTone(buzzPin);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  inches = (duration / 2) / 20;
  if (inches < 35 || inches > 1000) {
    if (isAlarming == false){ objectDetected(); }
  }else {
    if (isAlarming == true) { objectCleared(); }
  }
  delay(1000);
}

void objectDetected() {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  //tone(buzzPin, 1000);
  isAlarming = true;
}
void objectCleared() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  noTone(buzzPin);
  isAlarming = false;
}
