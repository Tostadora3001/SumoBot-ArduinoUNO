#ifndef SENSORES_HH
#define SENSORES_HH

#include <VL53L0X.h>

/*
Este vector tiene una sucesion de integers que representan los valores medidos por cada sensor.
Cada posición del vector corresponde a un sensor específico, y el valor almacenado en esa posición representa la lectura actual de ese sensor. 
Estos valores se utilizan para tomar decisiones en la máquina de estados, permitiendo al robot reaccionar de manera adecuada a su entorno y a 
las condiciones que se presenten durante la ejecución del programa.

El orden de los sensores en el vector es el siguiente:
    - [0] : Sensor Ultrasonido Frontal
    - [1] : Sensor Ultrasonido Derecha
    - [2] : Sensor Ultrasonido Izquierda
    - [3] : Sensor Ultrasonido Trasero
    - [4] : Sensor Láser Frontal
    - [5] : Sensor Láser Derecha
    - [6] : Sensor Láser Izquierda
    - [7] : Sensor Infrarrojo Frontal
    - [8] : Sensor Infrarrojo Trasero
    - [9] : Sensor de Inicio (puede ser un botón o una señal que indique el inicio de la competición)

*/
extern int Flags_Sensores[10];

//Declaración de los objetos de los sensores láser VL53L0X
extern VL53L0X sensorIzq;
extern VL53L0X sensorFron;
extern VL53L0X sensorDer;

//Esta funcion ejecuta la lectura de todos los sensores y actualiza el vector de Flags_Sensores con los valores obtenidos de cada sensor.
void Ejecutar_Sensores();

void Detectar_INICIO();

#endif
