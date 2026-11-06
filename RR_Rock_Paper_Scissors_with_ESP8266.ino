/*
* Rajan Robotics
* Created by Balaji for Rajan Robotics
*
*/

#include <Servo.h>

// Updated Pin configurations according to your wiring
const int servoPin = 14;  // D5 on NodeMCU (GPIO 14)
const int trigPin  = 13;  // D7 on NodeMCU (GPIO 13) - Orange wire
const int echoPin  = 15;  // D8 on NodeMCU (GPIO 15) - Red wire

// Detection Thresholds
const int targetDistanceCm = 10;   // Distance in cm to trigger a roll (hand close)
const unsigned long cooldown = 2000; // 2 seconds delay before allowing another roll

Servo choiceServo;
unsigned long lastTriggerTime = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize Ultrasonic Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize Servo and set to default starting position (Rock)
  choiceServo.attach(servoPin, 544, 2400); 
  choiceServo.write(0); 
  
  // Seed the random number generator using an unconnected analog pin noise
  randomSeed(analogRead(A0));
  
  Serial.println("System Ready! Place hand closer than 10cm to play.");
}

void loop() {
  long duration;
  int distance;

  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Trigger the sensor with a 10 microsecond high pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pulse travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout to avoid hanging
  
  // Calculate distance in cm
  distance = duration / 29.1 / 2;

  // // Print distance for debugging via Serial Monitor
  // if (distance > 0 && distance < 200) {
  //   Serial.print("Distance: ");
  //   Serial.print(distance);
  //   Serial.println(" cm");
  // }

  // Check if hand is detected within range and cooldown period has expired
  if (distance > 0 && distance <= targetDistanceCm && (millis() - lastTriggerTime > cooldown)) {
    lastTriggerTime = millis();
    
    Serial.println("--- Hand Detected! Shuffling... ---");
    
    // Quick jitter movement to animate a visual "shuffle / shake" phase
    for(int i = 0; i < 3; i++) {
      choiceServo.write(30);
      delay(150);
      choiceServo.write(0);
      delay(150);
    }

    // Generate random choice: 0 = Rock, 1 = Paper, 2 = Scissors
    int gameChoice = random(0, 3); 
    
    if (gameChoice == 0) {
      Serial.println("Result: ROCK (0 Degrees)");
      choiceServo.write(0); 
    } 
    else if (gameChoice == 1) {
      Serial.println("Result: PAPER (90 Degrees)");
      choiceServo.write(90); 
    } 
    else {
      Serial.println("Result: SCISSORS (180 Degrees)");
      choiceServo.write(180); 
    }
  }

  delay(60); // Small cycle delay to let the ultrasonic waves settle
}
