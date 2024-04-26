#include <avr/io.h>  // AVR I/O definitions
#include <util/delay.h>  // Delay functions

#define F_CPU 16000000UL  // Define CPU frequency
#define BAUD 115200  // Define baud rate for UART communication
#define UBRR_VALUE F_CPU/16/BAUD-1  // Calculate UBRR value for UART

#define TEMP_SENSOR_PIN PC2  // Define pin for temperature sensor
#define FAN_PIN PB2  // Define pin for fan

#define LED1_PIN PB0  // Define pin for LED 1, GANTRY FORWARD TILT
#define LED2_PIN PB1  // Define pin for LED 2, GANTYR BACKWARD TILT
#define SWITCH1_PIN PC0  // Define pin for switch 1, GANTRY FORWARD TILT
#define SWITCH2_PIN PC1  // Define pin for switch 2, GANTRY BACKWARD TILT
#define EMERGENCY_STOP_PIN PD2  // Define pin for emergency stop switch, EMERGENCY

// H-BRIDGE SWITCHES FOR GANTRY MOTOR CONTROL
#define HBRIDGE_S1 PD4  // Define pin for H-bridge control, S1
#define HBRIDGE_S2 PD5  // Define pin for H-bridge control, S2
#define HBRIDGE_S3 PD6  // Define pin for H-bridge control, S3
#define HBRIDGE_S4 PD7  // Define pin for H-bridge control, S4

// Function to initialize ADC
void ADC_Init() {
	ADMUX |= (1 << REFS0);  // Setting ADC reference voltage to AVcc
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Enabling ADC and setting pre-scaler to 128
}

// Function to read analog value from specified ADC channel
uint16_t ADC_Read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07);  // Clearing previous channel selection and selecting new channel
	ADCSRA |= (1 << ADSC);  // Starting ADC conversion
	while (ADCSRA & (1 << ADSC));  // Waiting for conversion to complete
	return ADC;  // Returning ADC result
}

// Function to convert ADC value to temperature (in Celsius)
float ADC_to_Temperature(uint16_t adc_value) {
	float temperature = adc_value * 0.25;  // Scaled to match sensor real-time readings
	return temperature; //returning temperature
}

// Function to initialize UART communication
void UART_init() {
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);  // Setting baud rate
	UBRR0L = (uint8_t)UBRR_VALUE;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enabling receiver and transmitter
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Setting frame format: 8 data bits, 1 stop bit, no parity
}

// Function to send a character over UART
void UART_sendChar(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));  // Waiting for empty transmit buffer
	UDR0 = data;  // Putting data into buffer, sending the data
}

// Function to receive a character over UART
unsigned char UART_receiveChar() {
	while (!(UCSR0A & (1 << RXC0)));  // Waiting for data to be received
	return UDR0;  // Getting and returning received data from buffer
}

// Function to initialize I/O pins
void initializeIO() {
	DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN);  // Setting LED pins as output
	DDRC &= ~((1 << SWITCH1_PIN) | (1 << SWITCH2_PIN));  // Setting switch pins as input
	PORTC |= (1 << SWITCH1_PIN) | (1 << SWITCH2_PIN);  // Enabling internal pull-up resistors for switches
	DDRD &= ~(1 << EMERGENCY_STOP_PIN);  // Setting emergency stop pin as input
	PORTD |= (1 << EMERGENCY_STOP_PIN);  // Enabling internal pull-up resistor for emergency stop switch
	DDRD |= (1 << HBRIDGE_S1) | (1 << HBRIDGE_S2) | (1 << HBRIDGE_S3) | (1 << HBRIDGE_S4);  // Setting H-bridge control pins as output
}

// Function to control the motor based on direction
void controlMotor(uint8_t direction) {
	switch (direction) {
		case 1:
		PORTD |= (1 << HBRIDGE_S1) | (1 << HBRIDGE_S3);  // Forward direction
		PORTD &= ~((1 << HBRIDGE_S2) | (1 << HBRIDGE_S4));
		break;
		case 2:
		PORTD |= (1 << HBRIDGE_S2) | (1 << HBRIDGE_S4);  // Reverse direction
		PORTD &= ~((1 << HBRIDGE_S1) | (1 << HBRIDGE_S3));
		break;
		default:
		PORTD &= ~((1 << HBRIDGE_S1) | (1 << HBRIDGE_S2) | (1 << HBRIDGE_S3) | (1 << HBRIDGE_S4));  // Stop motor
		break;
	}
}

int main(void) {
	ADC_Init();  // Initialize ADC
	UART_init();  // Initialize UART
	DDRB |= (1<<FAN_PIN);  // Set fan pin as output
	initializeIO();  // Initialize I/O pins

	uint16_t tempValue;  // Variable to store ADC value
	float temperature;  // Variable to store temperature
	uint8_t switchState1, switchState2;  // Variables to store switch states
	uint8_t emergencyStopState;  // Variable to store emergency stop state

	while (1) {
		// Read analog value from temperature sensor connected to ADC channel 2 (PC2)
		uint16_t tempValue = ADC_Read(TEMP_SENSOR_PIN);
		// Convert ADC value to temperature
		float temperature = ADC_to_Temperature(tempValue);

		switchState1 = !(PINC & (1 << SWITCH1_PIN));  // Read switch 1 state
		switchState2 = !(PINC & (1 << SWITCH2_PIN));  // Read switch 2 state
		emergencyStopState = !(PIND & (1 << EMERGENCY_STOP_PIN));  // Read emergency stop state

		if (emergencyStopState) {
			UART_sendChar('E');  // Send emergency stop signal over UART
			// Turn off all LEDs
			PORTB &= ~((1 << LED1_PIN) | (1 << LED2_PIN));
			// Stop the motor
			controlMotor(0);
			// Wait until emergency stop is released
			while (emergencyStopState) {
				emergencyStopState = !(PIND & (1 << EMERGENCY_STOP_PIN)); //wait until the button is pressed again
			}
			} else {
			// Control motor based on switch states
			  if (switchState1 && !switchState2) {
				UART_sendChar('F');  // Send forward tilt signal over UART
				controlMotor(1);  // Forward direction
			  } else if (!switchState1 && switchState2) {
			    UART_sendChar('B');  // Send backward tilt signal over UART
			    controlMotor(2);  // Reverse direction
			  } else {
			    UART_sendChar('N');  // Send no tilt signal over UART
			    controlMotor(0);  // Stop motor
		                }

		       // Toggle LEDs based on switch states
		       PORTB = (switchState1 << LED1_PIN) | (switchState2 << LED2_PIN);

		       // Perform temperature-based fan control
		       if (temperature > 40) {
			    // If temperature exceeds threshold, turn on fan
			   PORTB |= (1 << FAN_PIN);  // Set fan pin high
			   } else {
			    // Otherwise, turn off fan
			   PORTB &= ~(1 << FAN_PIN);  // Set fan pin low
		       }
	         }

	         _delay_ms(100);  // Delay for stability
         }

         return 0;
}

// End of code

