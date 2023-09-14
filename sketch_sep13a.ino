#include <ArduinoJson.h>

const int trigPin = 9;
const int echoPin = 10;
const int redLED = 2;
const int yellowLED = 3;
const int greenLED = 4;
int pedestrianCount = 0;
int duration = 0;
int distance = 0;
int interval = 5000;
int state = 0;
bool pedestrian = false;
unsigned long currentTime = 0;
unsigned long previousMillis = 0;
StaticJsonDocument<200> jsonDoc;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(greenLED, HIGH);
  Serial.begin(9600);
}

void loop() {
  currentTime = millis();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 100) {
    pedestrian = true;
  }
  else {
    pedestrian = false;
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    state = 0;
    previousMillis = currentTime;
  }

  if ((currentTime - previousMillis >= interval) && (state == 0) && (pedestrian == true)) {
      previousMillis = currentTime;
      state = 1;
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
  }
  else if ((currentTime - previousMillis >= interval) && (state == 1) && (pedestrian == true)) {
      previousMillis = currentTime;
      state = 2;
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
      pedestrianCount++;
  }
  else if ((currentTime - previousMillis >= interval) && (state == 2) && (pedestrian == true)) {
      previousMillis = currentTime;
      state = 0;
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
  }
  
  jsonDoc["distance"] = distance;
  jsonDoc["pedestrianCount"] = pedestrianCount;
  jsonDoc["state"] = state;

  serializeJson(jsonDoc, Serial);
  Serial.println();
  delay(500);
}