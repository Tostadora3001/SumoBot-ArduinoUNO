>[!NOTE]
>This README is mainly made with IA.

# AESBOT-Sumo

This project implements a Sumo robot using PlatformIO in VS Code. The robot is designed to compete in Sumo wrestling by pushing opponents out of the ring using various sensors for navigation and opponent detection.

## Hardware Components

- **Sensors:**
  - 3 VL53L0X laser distance sensors for precise distance measurement
  - 4 ultrasonic sensors for obstacle detection
  - 2 line sensors for detecting white/black boundaries (ring edges)

- **Actuators:**
  - 2 motors for robot movement and maneuvering

## Software Structure

The code is organized in the `src/` directory:

- `main.cpp`: Main entry point and program loop
- `Actuadores/`: Motor control and actuation code
- `Control/`: State machine logic for robot behavior
- `Pines/`: Pin definitions and hardware configuration
- `Sensores/`: Sensor reading and processing code

## State Machine
>[!NOTE]
>The States name are in Spanish.

The robot's behavior is controlled by a finite state machine implemented in `Control/Maquina_estados.cpp`. The states are:

- **INICIO**: Initial rest state. The robot waits for the start signal (PIN_INICIO high).
- **SENSORES**: Sensor reading state. All sensors are polled to update the sensor flags array.
- **ANALISIS**: Data analysis state. Purely logical state that evaluates sensor data and determines the next action based on priority:
  - Highest priority: Avoid falling (NO_CAER) if line sensors detect ring edges
  - Second priority: Attack (ATACAR) if opponent is detected close by
  - Third priority: Turn (GIRAR) to face detected opponent
  - Lowest priority: Search (BUSCAR) by spinning if no opponent detected
- **BUSCAR**: Search state. Robot spins to locate opponent.
- **ATACAR**: Attack state. Robot moves forward to push opponent out of ring.
- **GIRAR**: Turn state. Robot turns to face detected opponent.
- **NO_CAER**: Avoid fall state. Robot maneuvers to stay within ring boundaries.

The state machine prioritizes safety (avoiding ring edges) over aggression (attacking opponent).

## Sensor Functions

Sensor readings are handled in `Sensores/Sensores.cpp`. The `Ejecutar_Sensores()` function follows a priority-based reading order:

1. **Line Sensors (IR)**: Checked first for ring edge detection. If detected, stops further readings.
2. **Laser Sensors (VL53L0X)**: Fast, precise distance measurement (up to 70cm range). If opponent detected, also reads corresponding ultrasonic for confirmation.
3. **Ultrasonic Sensors (HC-SR04)**: Slower but reliable (up to 30cm range). Used when lasers don't detect anything.

Sensor data is stored in `Flags_Sensores[10]` array:
- [0-3]: Ultrasonic sensors (Front, Right, Left, Back)
- [4-6]: Laser sensors (Front, Right, Left) 
- [7-8]: IR line sensors (Front, Back)
- [9]: Start sensor/button

Functions:
- `VL53L0X_Medicion()`: Reads laser sensor distance in cm, returns 0 if timeout or out of range
- `HC_SR04_Medicion()`: Reads ultrasonic distance in cm using trigger/echo on same pin
- `IR_Medicion()`: Digital read for line sensor (inverted logic)

## Pin Definitions (Pines.h)

The `Pines/Pines.h` file defines all hardware pin assignments:

- **Start Button**: PIN_INICIO (A2) - Digital input for competition start
- **Ultrasonic Sensors**: HC_FRONTAL_PIN (11), HC_DERECHA_PIN (12), HC_IZQUIERDA_PIN (13), HC_TRASERO_PIN (A0) - Each uses single pin for both trigger and echo
- **Laser Sensors**: VL_SDA_PIN (A4), VL_SCL_PIN (A5) for I2C communication. XSHUT pins (7,8,10) for sensor shutdown control. Direction IDs (0x30,0x31,0x32) for sensor addressing
- **Line Sensors**: IR_FRONTAL_PIN (2), IR_TRASERO_PIN (4) - Digital inputs for ring edge detection
- **Motors**: MOTOR_IZQUIERDO_INT1/INT2 (3,5), MOTOR_DERECHO_INT1/INT2 (6,9) - PWM outputs for motor control

## Debug Prints (Log_INFO Function)

The `Log_INFO()` function in `Control/Maquina_estados.cpp` provides comprehensive debugging output via serial communication. It prints the following information at the end of each state machine cycle:

- **States**: Current state and next scheduled state (e.g., "INICIO", "SENSORES", "ANALISIS", etc.)
- **Sensors**: Raw values from all sensors:
  - Ultrasonic sensors (Front, Right, Left, Back) - distance in cm or 0 if not detected
  - Laser sensors (Front, Right, Left) - distance in cm or 0 if not detected  
  - IR line sensors (Front, Back) - 1 if edge detected, 0 otherwise
  - Start sensor/button - 1 if competition started, 0 otherwise
- **Actuators**: Flags indicating planned motor actions:
  - Turn left/right
  - Move forward/backward

This output helps monitor sensor readings, state transitions, and motor commands in real-time for debugging and tuning the robot's behavior.

## Expected Robot Behavior in Real Situation

In a Sumo competition, the AESBOT-Sumo robot follows this typical behavior sequence:

1. **Pre-Competition**: Robot remains stationary in INICIO state until start signal is received.

2. **Start of Match**: Upon start signal, enters search mode by spinning to locate the opponent using ultrasonic and laser sensors.

3. **Opponent Detection**: 
   - If opponent detected at medium range (ultrasonic), turns to face them
   - If opponent detected at close range (laser + ultrasonic), immediately attacks by moving forward
   - Uses laser sensors for precise targeting and ultrasonic for confirmation

4. **Combat Phase**: 
   - Prioritizes pushing opponent out while staying within ring boundaries
   - Continuously scans for opponent position changes
   - Adjusts direction based on sensor feedback

5. **Ring Safety**: 
   - Line sensors constantly monitor ring edges
   - If approaching edge, immediately reverses or turns away (highest priority)
   - Prevents falling out even during aggressive attacks

6. **Search Recovery**: If opponent lost from sensors, resumes spinning search pattern.

The robot balances aggression (attacking detected opponents) with caution (avoiding ring edges), using fast laser sensors for close combat and reliable ultrasonic sensors for general navigation. The state machine ensures quick reactions to changing combat situations while maintaining ring safety.

## Project Setup

This project is built using PlatformIO, a professional development environment for embedded systems.

### Prerequisites

- VS Code with PlatformIO extension installed
- Compatible microcontroller board (configured in `platformio.ini`)

### Building and Running

1. Open the project folder in VS Code
2. Ensure PlatformIO extension is active
3. Use PlatformIO's build and upload commands to compile and deploy the code

## Dependencies

- VL53L0X library (version 1.3.1) included in `lib/` directory

## Notes

- The `Facturas/` directory contains billing/invoice documents and is not part of the project code.

## Acknowledgments

Special thanks to AESS Estudiants ([https://aess-estudiants.vercel.app/](https://aess-estudiants.vercel.app/)) for organizing the AESSBOT tournaments and encouraging students to build and compete with robots. Their efforts in promoting robotics education and competition are greatly appreciated!
