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
String cwmodeCmd = "AT+CWMODE=3\r\n";
String checkConnection = "AT+CIFSR\r\n";

SoftwareSerial esp(RX, TX);

void setup() {
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
      if(sendCommandToESP8266(checkConnection, 3, "192.168") == true) {
        blink();  
        commStat = 4;
      }
      break;
    case 4: 
      atomicSend("AT+CIPMUX=1\r\n", 3);
      if(!sendCommandToESP8266("AT+CIPSTART=0,\"TCP\",\"google.com\",80\r\n", 3, "CONNECT")) break;;
      if(!sendCommandToESP8266("AT+CIPSEND=0,16\r\n", 5, "OK"))break;
      // command to be sent has to have matched length
      // \r or \n is considered one byte
      //if(!sendCommandToESP8266("GET / HTTP/1.0\r\n", 20, "OK")) break;
      atomicSend("GET / HTTP/1.0\r\n", 20);
      commStat = 5;
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

void atomicSend(String command, int maxTime) {
  int count = 0;
  esp.print(command);
  while(count < maxTime) {
    while (esp.available() > 0) {
      char oneChar= esp.read();
      Serial.print(oneChar);
    }
    delay(200);
    ++count;
  } 
  Serial.print(command + "\n");
}

boolean sendCommandToESP8266(String command, int maxTime, char readReplay[]) {
  int countTimeCommand = 0;
  esp.print(command);
  while (countTimeCommand < maxTime)
  {
    if (esp.find(readReplay))
    {
      Serial.print(command + " success\n");
      delay(200);
      return true;
    }
    delay(200);
    ++countTimeCommand;
  }
  Serial.print(command + " fail\n");
  return false;
}
