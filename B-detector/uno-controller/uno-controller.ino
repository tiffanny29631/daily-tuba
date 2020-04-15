#include <SoftwareSerial.h>
#include <ArduinoJson.h>

int trigPin = 9;
int echoPin = 10;
int redLed = 13;
int greenLed = 12;
int buzzPin = 11;
long duration, cm, inches;
bool isAlarming = false;

// 0 for starting;
// 1 for esp comm established
// 2 for wifi setup done
// tbd
int commStat = 0;

// Pin Definitions
#define RX  5
#define TX  3

String awskey = "asdf";
String host = "asdf";
String path = "asdf";
String port = "asdf";

String setupCmd = "AT+CWJAP=\"    \",\"    \"";
String pingCmd = "AT\r\n";
String cwmodeCmd = "AT+CWMODE=1\r\n";
String checkConnection = "AT+CIFSR\r\n";
  
SoftwareSerial esp(RX, TX);

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  //digitalWrite(greenLed, HIGH);
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

  switch(commStat) {
    case 0:
      if(sendCommandToESP8266(pingCmd, 5, "OK") == true) {
        blink();
        commStat = 1;
      }
      break;
    case 1: 
      if(sendCommandToESP8266(cwmodeCmd, 2, "OK") == true) {
        blink();  
        commStat = 2;
      }
      break;
    case 2:
      if(sendCommandToESP8266(setupCmd, 10, "OK") == true) {
        blink();  
        commStat = 3;
      }
      break;
    case 3: 
      if(sendCommandToESP8266(checkConnection, 2, "192.168") == true) {
        blink();  
        commStat = 3;
      }
      break;
    default:
      break;
  }
}

void objectDetected() {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  tone(buzzPin, 1000);
  isAlarming = true;
}
void objectCleared() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  noTone(buzzPin);
  isAlarming = false;
}

void blink() {
  digitalWrite(greenLed, HIGH);
  delay(100);
  digitalWrite(greenLed, LOW);
  } 

boolean sendCommandToESP8266(String command, int maxTime, char readReplay[]) {
  int countTimeCommand = 0;
  esp.print(command);
  while (countTimeCommand < maxTime)
  {
    if (esp.find(readReplay))
    {
//      int availableBytes = esp.available();
//      for(int i = 0; i < availableBytes; ++i) {
//        char reply = esp.read();
//        Serial.print(reply);  
//      }
//      Serial.print("\n");
      Serial.print(command + " reply success\n");
      delay(500);
      return true;
    }
    delay(200);
    ++countTimeCommand;
  }
  Serial.print(command + " fail\n");
  return false;
}
