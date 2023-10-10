#include <SoftwareSerial.h>

// Define pin numbers for Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 10;

// Define pin numbers for GSM Module (SoftwareSerial)
SoftwareSerial gsm(7, 8);

// Define phone number for receiving alerts
String alertPhoneNumber = "+1234567890"; // Replace with your own number

// Define distance thresholds for alerts
int lowLevelThreshold = 30; // Adjust as needed
int criticalLevelThreshold = 15; // Adjust as needed

void setup() {
  // Initialize Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize GSM Module
  gsm.begin(9600);
}

void loop() {
  // Measure distance from Ultrasonic Sensor
  int distance = measureDistance();

  // Send alerts based on distance measurements
  if (distance < criticalLevelThreshold) {
    sendAlert("CRITICAL: Water level is very low!");
  } else if (distance < lowLevelThreshold) {
    sendAlert("ALERT: Water level is low.");
  }

  delay(60000); // Wait for 1 minute before taking the next measurement
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int cm = duration * 0.034 / 2;

  return cm;
}

void sendAlert(String message) {
  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  gsm.println("AT+CMGS=\"" + alertPhoneNumber + "\""); // Send message to alertPhoneNumber
  delay(1000);
  gsm.println(message); // Send the actual message
  delay(1000);
  gsm.println((char)26); // CTRL+Z to send message
  delay(1000);
}
