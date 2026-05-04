#include "Actuadores.hh"
#include <Arduino.h>
#include "../Pines/Pines.h"

//Para mayor legibilidad se declaran
#define GIRAR_IZQUIERDA 0
#define GIRAR_DERECHA 1
#define MOVER_ADELANTE 2
#define MOVER_ATRAS 3

#define MAXIMA_POTENCIA 255
#define MINIMA_POTENCIA 0

int Flags_Actuadores[4];

void Motor_Izquierdo(int Potencia, bool adelante);
void Motor_Derecho(int Potencia, bool adelante);

void Ejecutar_Actuadores(){
    //Si el vector está vacío o hay varias acciones a ejecutar, no se ejecuta ninguna acción para evitar comportamientos erráticos.
    bool vector_valido = false;
    int accion = -1;
    for(int i = 0; i < 4; i++){
        if(Flags_Actuadores[i] > 0){
            if(!vector_valido){
                vector_valido = true;
                accion = i;
            }
            else vector_valido = false; // Si hay más de una acción a ejecutar, no se ejecuta ninguna para evitar comportamientos erráticos.
        }
    }

    // Si el vector está vacío, no se ejecuta ninguna acción y se para al robot.
    if(!vector_valido){
        Motor_Izquierdo(0, true);
        Motor_Derecho(0, true);
        return;
    }

    //Lógica de ejecución de cada acción
    switch (accion) {
        case GIRAR_IZQUIERDA:
            Motor_Izquierdo(MAXIMA_POTENCIA, false);
            Motor_Derecho(MAXIMA_POTENCIA, true);
            break;
        case GIRAR_DERECHA:
            Motor_Izquierdo(MAXIMA_POTENCIA, true);
            Motor_Derecho(MAXIMA_POTENCIA, false);
            break;
        case MOVER_ADELANTE:
            Motor_Izquierdo(MAXIMA_POTENCIA, true);
            Motor_Derecho(MAXIMA_POTENCIA, true);
            break;
        case MOVER_ATRAS:
            Motor_Izquierdo(MAXIMA_POTENCIA, false);
            Motor_Derecho(MAXIMA_POTENCIA, false);
            break;
    }

}

void Motor_Izquierdo(int Potencia, bool adelante){
    Potencia = constrain(Potencia, 0, 255);
    
    // Freno en seco si la potencia es 0 (IN1=1, IN2=1)
    if (Potencia == 0) {
        digitalWrite(MOTOR_IZQUIERDO_INT1, HIGH);
        digitalWrite(MOTOR_IZQUIERDO_INT2, HIGH);
        return; // Salimos de la función
    }
    
    if (adelante) {
        analogWrite(MOTOR_IZQUIERDO_INT1, Potencia); 
        digitalWrite(MOTOR_IZQUIERDO_INT2, LOW);     
    } else {
        digitalWrite(MOTOR_IZQUIERDO_INT1, LOW);     
        analogWrite(MOTOR_IZQUIERDO_INT2, Potencia); 
    }
}

void Motor_Derecho(int Potencia, bool adelante) {
    Potencia = constrain(Potencia, 0, 255);
    
    // Freno en seco si la potencia es 0 (IN1=1, IN2=1)
    if (Potencia == 0) {
        digitalWrite(MOTOR_DERECHO_INT1, HIGH);
        digitalWrite(MOTOR_DERECHO_INT2, HIGH);
        return; // Salimos de la función
    }
    
    if (adelante) {
        analogWrite(MOTOR_DERECHO_INT1, Potencia); 
        digitalWrite(MOTOR_DERECHO_INT2, LOW);     
    } else {
        digitalWrite(MOTOR_DERECHO_INT1, LOW);     
        analogWrite(MOTOR_DERECHO_INT2, Potencia); 
    }
}

