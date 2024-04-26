#include <PID_v1.h> // PID Library imported

// Declaring variables for setpoint, temperature, and PID output
double Setpoint; // Desired temperature
double temp; // Temperature sensor reading
double Output; // PID output

// Declaring PID parameters
double Kp = 1.0;
double Ki = 3.0;
double Kd = 0.2;

// Creating PID instance with specified parameters
PID myPID(&temp, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Defining pin numbers for fan and heater control
const int fanPin = 7; // digital pin for the fan
const int heaterPin = 5; // digital pin for the heater

// Setup function, runs once when the Arduino is powered on or reset
void setup()
{
// Begins serial communication at 9600 baud rate
Serial.begin(9600);

// Setting the desired temperature
Setpoint = 22;

// Turning on PID controller in AUTOMATIC mode

myPID.SetMode(AUTOMATIC);

// Adjusting PID controller's tuning parameters
myPID.SetTunings(Kp, Ki, Kd);

// Setting fan and heater pins as outputs
pinMode(fanPin, OUTPUT);
pinMode(heaterPin, OUTPUT);
}

// Loop function, to run repeatedly as long as the Arduino is powered on
void loop()
{
// Reading the analogue value from the temperature sensor
int tempsensor = analogRead(A0);

// Performing temperature conversion based on experimentally determined values
temp = map(tempsensor, 508, 1023, -25, 105); // values were adjusted based on sensor readings

// Performing PID calculation
myPID.Compute();

// Controlling the heater and fan based on the PID output
if (temp > Setpoint) {
// Temperature is above setpoint, turning on the fan
digitalWrite(fanPin, Output); //fan on
digitalWrite(heaterPin, LOW); // Turning off the heater
Serial.println("The Fan is ON ");
} else {

// Temperature is below or at setpoint, turning on the heater
digitalWrite(heaterPin, Output); //heater on
digitalWrite(fanPin, LOW); // Turning off the fan
Serial.println("The Heater is ON");
}

// Sending temperature and setpoint data to serial for plotting
Serial.print("The Temperature is: ");
Serial.println(temp);
Serial.print("The Setpoint is: ");
Serial.println(Setpoint);
}
