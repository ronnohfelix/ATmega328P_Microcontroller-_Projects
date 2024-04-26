# Chemical Flow Control System

Welcome to the Chemical Flow Control System project repository! This project aims to design a system using an Atmega328P microcontroller (such as the one found on an Arduino Uno board) to control the flow of chemicals to three storage tanks. The project involves managing the operation of pumps to fill and empty the tanks, based on the inputs from a keypad and safety features such as an emergency button.

## Features

- **Tank Control**: Three storage tanks are controlled individually. When numbers 1, 2, or 3 followed by `*` are pressed on the keypad, the corresponding tank's fill pump is activated. When numbers 1, 2, or 3 followed by `#` are pressed, the empty pump is activated.
  
- **Level Sensors**: Each storage tank is equipped with a level sensor (potential meter) that generates 5V when the tank is full and 0V when empty.

- **Pump Control**: The fill and empty pumps for the tanks are controlled using 5V MOSFET transistors.

- **Safety Features**: An emergency button can be used to stop all pumps and sound an alarm. The button locks into position when pressed and needs to be reset manually.

- **Pump Restrictions**: Only one pump can be run at a time.

## Getting Started

To get started with the project, follow the instructions below:

1. **Hardware**:
   - Atmega328P or Arduino Uno board.
   - Keypad (0-9, `*`, and `#` keys).
   - Three storage tanks with level sensors.
   - Fill and empty pumps for each storage tank.
   - MOSFET transistors for controlling pumps.
   - Emergency button.
   - Other hardware components such as resistors, diodes, wires, and a buzzer for the alarm.

2. **Software**:
   - Arduino IDE for coding and uploading sketches to the Atmega328P board.
   - Clone the repository to your local machine.

3. **Wiring**:
   - Wire the keypad to the Atmega328P board.
   - Connect the pumps to the MOSFET transistors and the Atmega328P board.
   - Wire the level sensors to the Atmega328P board.
   - Connect the emergency button and buzzer as necessary.

4. **Software Implementation**:
   - Load the provided sketch file (`chemical_control.ino`) onto your Atmega328P board using Arduino IDE.
   - Adjust pin assignments and thresholds according to your specific setup in the sketch.
   - Make sure to test the system with no chemicals first to ensure everything is functioning as expected.

5. **Operation**:
   - Use the keypad to input commands for filling and emptying tanks.
   - The emergency button can be used to halt all pumps and sound the alarm in case of any emergency.

## Safety Precautions

- Handle all electrical and chemical components with care and follow safety protocols.
- Always wear appropriate safety gear when dealing with chemicals.
- Test the system without chemicals before actual use.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Thank you for using the Chemical Flow Control System! If you have any questions or need support, feel free to reach out to the project maintainers. Happy coding!
