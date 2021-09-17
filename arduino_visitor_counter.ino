// Library to work with LCD
#include <LiquidCrystal.h>

#define BUZ 12 // macro for Buzzer pin
// macro for Ultrasonic Distance Sensors
#define TR1 9  // TRIG pin 
#define EC1 8  // ECHO pin
#define TR2 11 // TRIG pin 
#define EC2 10 // ECHO pin

// Creating an object of LCD
// Syntax: class_name object_name(parameters)
// LiquidCrystal object(registerSelect_rs, enable_e, dataPin4_db4, dataPin5_db5, dataPin6_db6, dataPin7_db7)
LiquidCrystal disp(2, 3, 4, 5, 6, 7);

int totVisitors = 0, inNow = 0; // counter variables

float ultrSncDist(int trigPin, int echoPin) {
  // Function to return distance of object measured by Ultrasonic Distance Sensor (HC-SR04)
  float interval, dist;
  
  // Mechanism to measure the distance
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  interval = pulseIn(echoPin, HIGH); // time taken to receive the signal back
  dist = (interval / 2) / 29.1; // distance in cm
  
  return dist;
}

void valPrint() {
  // Function to print total visitors and currently inside on LCD
  disp.setCursor(11, 0);
  disp.print("     ");
  disp.setCursor(11, 1);
  disp.print("     ");
  disp.setCursor(11, 0);
  disp.print(totVisitors);
  disp.setCursor(11, 1);
  disp.print(inNow);
}

void setup() {
  Serial.begin(9600);
  disp.begin(16, 2); // 16x2 display
  
  // Pins for Ultrasonic Distance Sensor 1
  pinMode(TR1, OUTPUT); // TRIG pin 
  pinMode(EC1, INPUT);  // ECHO pin
  
  // Pins for Ultrasonic Distance Sensor 2
  pinMode(TR2, OUTPUT); // TRIG pin 
  pinMode(EC2, INPUT);  // ECHO pin
  
  pinMode(BUZ, OUTPUT); // Pin for Buzzer
  
  // Permanent text on LCD
  disp.setCursor(0, 0);
  disp.print("Tot Visits:");
  disp.setCursor(0, 1);
  disp.print("Inside now:");
  valPrint();
}

void loop() {
  digitalWrite(BUZ, LOW); // Buzzer off
  
  bool senseBy1 = 0, senseBy2 = 0; // flag values to check the state of sensors
  float dist1 = ultrSncDist(TR1, EC1), dist2 = ultrSncDist(TR2, EC2);
  
  //------------check if visitors ENTER-------------
  if (dist1 <= 300) {
    senseBy1 = 1; // if visitor passes sensor 1
  }
  while(senseBy1) {
    // keep checking if visitor passed sensor 2
    dist2 = ultrSncDist(TR2, EC2);
    if (dist2 <= 300) senseBy2 = 1;
    if (senseBy2) {
      totVisitors++; 
      inNow++;
      valPrint();
      delay(1000); 
      digitalWrite(BUZ, HIGH); // Buzzer on for 0.5 sec
      delay(500); // total 1.5 sec given to pass sensor 2
      return;
    }
  }
  //------------------------------------------------
  
  //-------------check if visitors EXIT-------------
  if (dist2 <= 300) {
    senseBy2 = 1; // if visitor passes sensor 2
  }
  while(senseBy2) {
    // keep checking if visitor passed sensor 1
    dist1 = ultrSncDist(TR1, EC1);
    if (dist1 <= 300) senseBy1 = 1;
    if (senseBy1) {
      digitalWrite(BUZ, HIGH); // Buzzer on
      inNow--;
      valPrint();
      delay(1000); 
      digitalWrite(BUZ, HIGH); // Buzzer on for 0.5 sec
      delay(500); // total 1.5 sec given to pass sensor 1
      return;
    }
  }
  //------------------------------------------------
}