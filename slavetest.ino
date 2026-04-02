#include <Wire.h>
#include <ESP32Servo.h>

// Motor pins
#define enA 33  // Left motor PWM
#define enB 25  // Right motor PWM
#define INa 26  // Left motor direction
#define INb 27
#define INc 14  // Right motor direction
#define INd 12

// Servo object
Servo myServo;
int steeringAngle = 90;  // Default servo angle
int servoPin = 13;       // Servo control pin

// I2C slave address
#define SLAVE_ADDRESS 0x08

// Structure for receiving data
struct DataPacket {
    int servoAngle;
    int rightSpeed;
    int leftSpeed;
};

volatile DataPacket receivedData;  // Variable to store received data

void setup() {
    Serial.begin(115200);
    Serial.println("Initialising slave...");

    // Initialise I2C as slave
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);

    // Configure motor direction pins
    pinMode(INa, OUTPUT);
    pinMode(INb, OUTPUT);
    pinMode(INc, OUTPUT);
    pinMode(INd, OUTPUT);

    // Attach the servo
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myServo.setPeriodHertz(50);  // 50Hz servo frequency
    myServo.attach(servoPin, 1000, 2000);  // Standard pulse widths
    myServo.write(steeringAngle);  // Set servo to neutral

    Serial.println("Slave initialised and ready.");
}

void loop() {
    // Update servo position and motor speeds based on received data
    myServo.write(receivedData.servoAngle);  // Update servo position
    analogWrite(enA, constrain(receivedData.leftSpeed, 0, 255));  // Update left motor speed
    analogWrite(enB, constrain(receivedData.rightSpeed, 0, 255));  // Update right motor speed

    // Set motor direction for forward motion
    digitalWrite(INa, HIGH);
    digitalWrite(INb, LOW);
    digitalWrite(INc, LOW);
    digitalWrite(INd, HIGH);

    // Debugging output
    Serial.print("Servo Angle: ");
    Serial.println(receivedData.servoAngle);
    Serial.print("Left Speed: ");
    Serial.println(receivedData.leftSpeed);
    Serial.print("Right Speed: ");
    Serial.println(receivedData.rightSpeed);

    delay(50);  // Small delay for stability
}

void receiveEvent(int numBytes) {
    // Ensure the received data matches the expected size
    if (numBytes == sizeof(receivedData)) {
        Wire.readBytes((char*)&receivedData, sizeof(receivedData));
        Serial.println("Data received:");
        Serial.print("Servo Angle: ");
        Serial.println(receivedData.servoAngle);
        Serial.print("Left Speed: ");
        Serial.println(receivedData.leftSpeed);
        Serial.print("Right Speed: ");
        Serial.println(receivedData.rightSpeed);
    } else {
        Serial.println("Incomplete data received.");
    }
}
