# Temperature Control System

Welcome to the Temperature Control System project repository! This project combines noise filtering and PID control to implement temperature control in a house using a microcontroller. The set point temperature is 22°C, and the system controls a fan and a heater (modeled as an oven in Proteus) based on the temperature readings from an AD592 temperature sensor.

## Features

- **Temperature Control**: The system maintains a set point temperature of 22°C. If the temperature rises above this set point, the fan kicks in to cool the house. If the temperature falls below this set point, the heater turns on.

- **Noise Filtering**: The system uses noise filtering techniques to smooth out the temperature sensor readings and minimize the impact of noise on the control system.

- **PID Control**: The project utilizes PID control to manage the fan and heater based on the filtered temperature readings, ensuring that the temperature remains close to the set point.

- **Temperature Monitoring**: The system includes a serial output to monitor the temperature after filtering using the virtual terminal in Proteus.

## Getting Started

To get started with the project, follow the instructions below:

1. **Hardware**:
   - Microcontroller (e.g., Arduino Uno or equivalent).
   - AD592 temperature sensor.
   - Fan and heater (modeled as an oven in Proteus).
   - Proteus simulation software.
   - Virtual terminal for serial output.

2. **Software**:
   - Arduino IDE for coding and uploading sketches to the microcontroller.
   - Clone the repository to your local machine.

3. **Setup**:
   - In Proteus, set up the microcontroller, AD592 temperature sensor, fan, and heater (oven model).
   - Connect the fan and heater to appropriate output pins on the microcontroller.
   - Connect the AD592 temperature sensor to an input pin on the microcontroller.
   - Connect the virtual terminal to the microcontroller for serial output.

4. **Software Implementation**:
   - Load the provided sketch file (`temperature_control.ino`) onto your microcontroller using Arduino IDE.
   - Adjust PID control parameters as needed for optimal performance.
   - Test the system in Proteus to ensure everything is functioning as expected.

5. **Operation**:
   - Monitor the temperature using the virtual terminal in Proteus.
   - The system will automatically control the fan and heater to maintain the set point temperature.

## Safety Precautions

- Always follow safety protocols when working with electrical components and heating elements.
- Test the system in a controlled environment before deploying it in a real-world scenario.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Thank you for using the Temperature Control System! If you have any questions or need support, feel free to reach out to the project maintainers. Happy coding!
