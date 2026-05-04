//Aqui se definen que pines usa que componente y estados usados en el programa.
#ifndef PINES_H
#define PINES_H

//Definición del Pin usado para el botón de inicio, que puede ser un botón físico o una señal que indique el inicio de la competición.
#define PIN_INICIO A2

//Ultrasonidos HC-SR04 
#define HC_FRONTAL_PIN 11
#define HC_DERECHA_PIN 12
#define HC_IZQUIERDA_PIN 13
#define HC_TRASERO_PIN A0

//Láseres VL53L0X 
#define VL_SDA_PIN A4
#define VL_SCL_PIN A5
#define VL_IZQUIERDA_XSHUT 7
#define VL_FRONTAL_XSHUT 8
#define VL_DERECHA_XSHUT 10

#define DIR_SENSOR_IZQUIERDA 0x30
#define DIR_SENSOR_FRONTAL 0x31
#define DIR_SENSOR_DERECHA 0x32

//Infrarojos IR TCRT5000
#define IR_FRONTAL_PIN 2
#define IR_TRASERO_PIN 4

//Motores
#define MOTOR_IZQUIERDO_INT1 3
#define MOTOR_IZQUIERDO_INT2 5
#define MOTOR_DERECHO_INT1 6
#define MOTOR_DERECHO_INT2 9

#endif