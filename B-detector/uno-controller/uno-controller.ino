#include <SoftwareSerial.h>
#include <ArduinoJson.h>

int trigPin = 9;
int echoPin = 10;
int redLed = 13;
int greenLed = 12;
int buzzPin = 11;
long duration, cm, inches;
bool isAlarming = false;

// Pin Definitions
#define RX  2
#define TX  3

//String wifi = "wifi";
//String pass = "pass"
// String awskey = "key"
String awskey = "asdf";
String host = "asdf";
String path = "asdf";
String port = "asdf";

SoftwareSerial esp(RX, TX);

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  digitalWrite(greenLed, HIGH);
  noTone(buzzPin);

  Serial.begin(9600);
  esp.begin(115200);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

//  inches = (duration / 2) / 20;
//  if (inches < 35 || inches > 1000) {
//    if (isAlarming == false){ objectDetected(); }
//  }else {
//    if (isAlarming == true) { objectCleared(); }
//  }

  if(sendCommandToESP8266("AT", 5, "OK") == true) blink();
  //delay(1000);
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

void blink() {
  objectDetected();
  delay(500);
  objectCleared();
  }

boolean sendCommandToESP8266(String command, int maxTime, char readReplay[]) {
  int countTimeCommand = 0;
  Serial.print("Command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < maxTime)
  {
    esp.println(command);
    if (esp.find(readReplay))
    {
      Serial.print("Success");
      return true;
    }
    ++countTimeCommand;
  }
  return false;
}
