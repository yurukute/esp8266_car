#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

//FIRMWARE CONFIGURATION
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLwxz7Etgp"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "s3_jVWTCNlIVRDlTlMGAvxWOXxsJ84PT"

//Define pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define moistPin D5
#define servoPin D8

//Define servo angles
#define UPPER 45
#define LOWER 0

//Joystick's axises
#define CENTER 50
int x = CENTER;
int y = CENTER;

Servo servo;

void setup() {
  Serial.begin(9600);

  //Set the motor pins as output pins
  int motors[] = {IN1, IN2, IN3, IN4};
  for(int i = 0; i < 4; i++)
    pinMode(motors[i], OUTPUT);

  //Set the sensor pin as an output pin
  pinMode(moistPin, OUTPUT);

  //Attach servo
  servo.attach(D8, 544, 2400);
  servo.write(UPPER);

  //Initialize the Blynk library
  char ssid[] = "STS"; //Enter your WIFI name
  char pass[] = "12341234"; //Enter your WIFI password
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
  smartCar();
  delay(100);
}

//Get Blynk values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}

BLYNK_WRITE(V1) {
  y = param[0].asInt();
}

BLYNK_WRITE(V2) {
  //int btnClicked = param.asInt();
  if(param.asInt()){
    servo.write(LOWER);
    if (readMoisture() >= 800){
      water();
    }
    servo.write(UPPER);
    Blynk.virtualWrite(V2, LOW);
  }
}

int readMoisture(){
  digitalWrite(moistPin, HIGH); // Turn the sensor ON
  delay(10);                    // Allow power to settle
  int val = analogRead(A0);     // Read the analog value form sensor
  digitalWrite(moistPin, LOW);  // Turn the sensor OFF
  return val;
}

void water(){
  Serial.print("Watering");
  delay(3000);
}
//Joystick handling
void smartCar(){
  int minRange = CENTER - 20;
  int maxRange = CENTER + 20;
  if (y > maxRange) {
    carForward();
    Serial.println("carForward");
  }
  else if (y < minRange) {
    carBackward();
    Serial.println("carBackward");
  }
  else if (x < minRange) {
    carLeft();
    Serial.println("carLeft");
  }
  else if (x > maxRange) {
    carRight();
    Serial.println("carRight");
  }
  else if (minRange <= x && x <= maxRange 
            && minRange <= y && y <= maxRange){
    carStop();
    Serial.println("carStop");
  }
}

//Motor movement functions
void carForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  int motors[] = {IN1, IN2, IN3, IN4};
  for(int i = 0; i < 4; i++)
    digitalWrite(motors[i], LOW);
}