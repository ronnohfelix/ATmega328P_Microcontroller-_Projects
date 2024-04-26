#include <Keypad.h> // Keypad library included to allow use of keys

const int buzzerPin = 13; // Definining the pin for the buzzer for emergency purposes

// Definining the pins for two pumps and the keypad
const int pump1 = 4; // Connected to pump1 via mosfet 1
const int pump2 = 5; // connected to pump2 via mosfet 2
const int numRows = 4; // Number of rows in the keypad
const int numCols = 3; // Number of columns in the keypad

// Defining pins for the level sensors of three tanks
const int tank1LevelPin = A1; // connected to potentiometer 1
const int tank2LevelPin = A2; // connected to potentiometer 2
const int tank3LevelPin = A3; // connected to potentiometer 3

// Defining pump control keys for each tank
const char tank1Key = '1'; //tank1
const char tank2Key = '2'; //tank2
const char tank3Key = '3'; //tank3

// Initial Variables to track the tank levels
int tank1Level = 0; //tank1
int tank2Level = 0; //tank2
int tank3Level = 0; //tank3

// Defining the keypad layout
char keys[numRows][numCols] = {

{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

// Defining the pinouts for the keypad rows and columns
byte rowPins[numRows] = {9, 8, 7, 6}; //row pins
byte colPins[numCols] = {12, 11, 10}; //column pins

// Creating a Keypad object with defined layout and pinouts
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, numRows, numCols);

// Setting up a function to initialize pin modes and start serial communication
void setup() {
pinMode(buzzerPin, OUTPUT);
pinMode(pump1, OUTPUT);
pinMode(pump2, OUTPUT);
pinMode(tank1LevelPin, INPUT);
pinMode(tank2LevelPin, INPUT);
pinMode(tank3LevelPin, INPUT);
Serial.begin(9600);
}

// Declaring a global variable to store the previous key pressed
char previousKey = '\0';

// Function to control pumps based on the pressed key
void controlPumps(char key) {

if (key == '*' && (previousKey == '1' || previousKey == '2' || previousKey == '3')) {
Serial.println("Turning Pump1 ON");
digitalWrite(pump1, HIGH); //pump1 is turned on because the keys pressed are either 1,2 or 3 follwed
by *
digitalWrite(pump2, LOW); //pump2 is off, since only one pump can run at a time
} else if (key == '#' && (previousKey == '1' || previousKey == '2' || previousKey == '3')) {
Serial.println("Turning Pump2 ON");
digitalWrite(pump2, HIGH); //pump2 is turned on because the keys pressed are either 1,2 or 3 follwed
by #
digitalWrite(pump1, LOW); //pump2 is off, since only one pump can run at a time
} else if (key == '9') {
Serial.println("Turning both pumps OFF");
digitalWrite(pump1, LOW); // emergency button to turn off both pumps, pump1 off
digitalWrite(pump2, LOW); // pump2 off
tone(buzzerPin, 1000, 500); // buzzer is turned on
delay(500); // buzzer delay time
noTone(buzzerPin); //buzzer goes off
}
previousKey = key; // Updating the previousKey variable
}

// Function to check and print the status of tank levels
void checkTankLevel(int tankLevel, const char* tankName) {
if (tankLevel >= 100) {//for a full tank
Serial.print("Tank ");
Serial.print(tankName);
Serial.println(" is Completely Full!");
delay(100);
} else if (tankLevel <= 0) { // for an empty tank

Serial.print("Tank ");
Serial.print(tankName);
Serial.println(" is Completely Empty!");
delay(100);
}
}

// Function to read tank level, update the variable, and check level status
void fillTank(int tankLevelPin, int& tankLevel, const char* tankName) {
int currentTankLevel = analogRead(tankLevelPin); //reads tank level
currentTankLevel = map(currentTankLevel, 0, 1023, 0, 100); // update the value in terms of percentage
tankLevel = currentTankLevel; //updates the level based on status
checkTankLevel(tankLevel, tankName); //checks the level
}

// Main loop function
void loop() {
char key = keypad.getKey(); // Reading a key from the keypad

// Reading analog values from tank level sensors and update tank levels
fillTank(tank1LevelPin, tank1Level, "1"); //for tank 1
fillTank(tank2LevelPin, tank2Level, "2"); // tank 2
fillTank(tank3LevelPin, tank3Level, "3"); // tank 3

// Printing the pressed key to the serial monitor
if (key) {
Serial.println(key);
controlPumps(key); // Controlling pumps based on the pressed key
}

// Checking if all tanks are full and stop Pump1
if (tank1Level >= 100 && tank2Level >= 100 && tank3Level >= 100) {
digitalWrite(pump1, LOW); //pump1 off since all the tanks are full
Serial.println("All Tanks are Completely Full! Pump1 OFF");
}
// Check if all tanks are empty and stop Pump2
else if (tank1Level == 0 && tank2Level == 0 && tank3Level == 0) {
digitalWrite(pump2, LOW); // pump2 off since the all tanks are empty
Serial.println("All Tanks are Completely Empty! Pump2 OFF");
}
}
