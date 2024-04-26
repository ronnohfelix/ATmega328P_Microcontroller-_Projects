// Aircraft Wheel Monitoring System

// Libraries for communication and LCD control
#include <Wire.h> // I2C communication library
#include <SoftwareSerial.h> // UART library
#include <LiquidCrystal_I2C.h> // LCD display library

// Creating an instance of the LiquidCrystal_I2C class for the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defining pin numbers for components and constants
#define rxPin 0 // Pin for receiving data
#define txPin 1 // Pin for transmitting data
#define brakeTempPinRightWheel A0 // Analog pin for the right wheel brake temperature sensor
#define brakeTempPinLeftWheel A1 // Analog pin for the left wheel brake temperature sensor
#define rightWheel 8 // PWM pin for controlling the speed of the right wheel
#define leftWheel 9 // PWM pin for controlling the speed of the left wheel
#define fanPinRightWheel 5 // Pin for the fan attached to the right wheel
#define fanPinLeftWheel 7 // Pin for the fan attached to the left wheel
#define rightWheelPressure A2 // Analog pin for the right wheel pressure sensor
#define leftWheelPressure A3 // Analog pin for the left wheel pressure sensor
#define brakePad 2 // Digital pin for the brake pad position sensor
#define ledPinRightWheel 10 // Pin for the LED indicator attached to the right wheel
#define ledPinLeftWheel 6 // Pin for the LED indicator attached to the left wheel

// Declaring variables to store wheel pressure values
float psi1; // Pressure in psi for the right wheel
float psi2; // Pressure in psi for the left wheel

// Creating a SoftwareSerial instance for serial communication
SoftwareSerial mySerial(rxPin, txPin);

// Setting up the initial configuration
void setup() {
// Initializing I2C communication
Wire.begin();

// Initializing SoftwareSerial communication
mySerial.begin(9600);

// Initializing the LCD with the number of columns and rows
lcd.begin(16, 2);

// Configuring pin modes for various components

// Configuring the brake temperature sensor pins as inputs
pinMode(brakeTempPinRightWheel, INPUT);
pinMode(brakeTempPinLeftWheel, INPUT);

// Configuring the pressure sensor pins as inputs
pinMode(rightWheelPressure, INPUT);
pinMode(leftWheelPressure, INPUT);

// Configuring the PWM pins for controlling wheel speed as outputs
pinMode(rightWheel, OUTPUT);
pinMode(leftWheel, OUTPUT);

// Configuring the LED pins as outputs

pinMode(ledPinRightWheel, OUTPUT);
pinMode(ledPinLeftWheel, OUTPUT);

// Configuring the fan pins as outputs
pinMode(fanPinRightWheel, OUTPUT);
pinMode(fanPinLeftWheel, OUTPUT);

// Configuring the brake pad position sensor pin as input
pinMode(brakePad, INPUT);
}

// Continuously running loop for real-time updates
void loop() {
// Setting the cursor to the start of the second line on the LCD
lcd.setCursor(0, 1);

// Reading the brake pad value and mapping it to a 0-100 range
int brakePadValue = digitalRead(brakePad);
brakePadValue = map(brakePadValue, 0, 1, 0, 100);

// Sending brake pad value to UART with a label
sendUARTData(brakePadValue, "Brake Pressure");

// Reading the brake position
int brakePosition = digitalRead(brakePad);

// Adjusting the speed of the right and left wheels based on the brake position
adjustRightWheelSpeed(brakePosition); // Right wheel speed
adjustLeftWheelSpeed(brakePosition); // Left wheel speed

psi1 = convertToPsi(analogRead(rightWheelPressure)); // Reading and converting pressure for the
right wheel
sendUARTData(psi1, "Right Wheel Pressure"); // Sending the right wheel pressure to UART with a
label

psi2 = convertToPsi(analogRead(leftWheelPressure)); // Reading and converting pressure for the left
wheel
sendUARTData(psi2, "Left Wheel Pressure"); // Sending the left wheel pressure to UART with a label

float brakeTempRightWheel = convertToTemperature(analogRead(brakeTempPinRightWheel)); //
Simulating the brake temperature sensor for the right wheel
sendUARTData(brakeTempRightWheel, "Right Wheel Brake Temperature"); // Sending the right wheel
brake temperature to UART with a label
// Displaying the right wheel brake temperature on the LCD
lcd.print("T1:");
lcd.print(brakeTempRightWheel);

float brakeTempLeftWheel = convertToTemperature(analogRead(brakeTempPinLeftWheel)); //
Simulating the brake temperature sensor for the left wheel
sendUARTData(brakeTempLeftWheel, "Left Wheel Brake Temperature"); // Sending the left wheel
brake temperature to UART with a label
// Displaying the left wheel brake temperature on the LCD
lcd.print("T2:");
lcd.print(brakeTempLeftWheel);

updateFeedback(ledPinRightWheel, fanPinRightWheel, brakeTempRightWheel, psi1); // Updating LED
and Fan feedback for the right wheel
updateFeedback(ledPinLeftWheel, fanPinLeftWheel, brakeTempLeftWheel, psi2); // Updating LED and
Fan feedback for the left wheel

// Introducing a delay to control the loop frequency

delay(200);
}

// Function to convert analog value to pressure in psi
float convertToPsi(int analogValue) {
float kpa = ((float)analogValue / (float)1023 + 0.095) / 0.009;
kpa *= 20; // Multiplying by 20 to adjust the value to actual pressure values of the plane's wheels
return 0.145038 * kpa;
}

// Function to convert analog value to temperature
float convertToTemperature(int analogValue) {
float temp = (analogValue / 1024.0) * 5000;
return temp / 10;
}

// Function to update LED and Fan feedback based on temperature and pressure
void updateFeedback(int ledPin, int fanPin, float brakeTemp, float psi) {
if (brakeTemp > 100 || psi < 175 || psi > 225) {
// Turning on the LED and the fan for warning and cooling
digitalWrite(ledPin, HIGH); // LED on
digitalWrite(fanPin, HIGH); // Fan on
} else {
// Turning off the LED and the fan
digitalWrite(ledPin, LOW); // LED off
digitalWrite(fanPin, LOW); // Fan off
}
}

// Function to adjust the speed of the right wheel based on the brake position
void adjustRightWheelSpeed(int brakePosition) {
int wheelSpeed = map(brakePosition, 0, 1, 255, 0); // Mapping the brake position to control the speed
of the right wheel
analogWrite(rightWheel, wheelSpeed); // Adjusting the speed of the right wheel
sendUARTData(wheelSpeed, "Right Wheel Speed"); // Sending the right wheel speed to UART with a
label
}

// Function to adjust the speed of the left wheel based on the brake position
void adjustLeftWheelSpeed(int brakePosition) {
int wheelSpeed = map(brakePosition, 0, 1, 255, 0); // Mapping the brake position to control the speed
of the left wheel
analogWrite(leftWheel, wheelSpeed); // Adjusting the speed of the left wheel
sendUARTData(wheelSpeed, "Left Wheel Speed"); // Sending the left wheel speed to UART with a
label
}

// Function to send data to UART
void sendUARTData(float data, String sensorType) {
// Sending data to UART with a label
mySerial.print(sensorType + ": ");
mySerial.println(data);
}
