#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Pines/Pines.h"
#include "Sensores/Sensores.hh"
#include "Control/Maquina_estados.hh"
#include "Actuadores/Actuadores.hh"

void setup() {
    Serial.begin(9600);
    Wire.begin();

    //Configración de los sensores láser VL53L0X. Dado que todos comparten la misma dirección I2C, 
    //se deben configurar uno por uno para asignarles direcciones distintas.
    pinMode(VL_IZQUIERDA_XSHUT, OUTPUT);
    pinMode(VL_FRONTAL_XSHUT, OUTPUT);
    pinMode(VL_DERECHA_XSHUT, OUTPUT);
    digitalWrite(VL_IZQUIERDA_XSHUT, LOW);
    digitalWrite(VL_FRONTAL_XSHUT, LOW);
    digitalWrite(VL_DERECHA_XSHUT, LOW);
    delay(10);

    //Encender y cambiar dirección I2C uno por uno (MÉTODO PRO)
    
    // Izquierda
    digitalWrite(VL_IZQUIERDA_XSHUT, HIGH);
    delay(10);
    sensorIzq.init();
    sensorIzq.setTimeout(200);
    sensorIzq.setAddress(DIR_SENSOR_IZQUIERDA); // Nueva dirección

    // Frontal
    digitalWrite(VL_FRONTAL_XSHUT, HIGH);
    delay(10);
    sensorFron.init();
    sensorFron.setTimeout(200);
    sensorFron.setAddress(DIR_SENSOR_FRONTAL); // Nueva dirección

    // Derecha
    digitalWrite(VL_DERECHA_XSHUT, HIGH);
    delay(10);
    sensorDer.init();
    sensorDer.setTimeout(200);
    sensorDer.setAddress(DIR_SENSOR_DERECHA); // Nueva dirección

    // Configuración de los pines de los sensores ultrasónicos HC-SR04

    //Configuración de los pines de los sensores infrarrojos IR TCRT5000
    pinMode(IR_FRONTAL_PIN, INPUT);
    pinMode(IR_TRASERO_PIN, INPUT);

    //Configuración de los pines de los motores
    pinMode(MOTOR_IZQUIERDO_INT1, OUTPUT);
    pinMode(MOTOR_IZQUIERDO_INT2, OUTPUT);
    pinMode(MOTOR_DERECHO_INT1, OUTPUT);
    pinMode(MOTOR_DERECHO_INT2, OUTPUT);

    //Configuración del pin de inicio
    pinMode(PIN_INICIO, INPUT);
}

void loop() {
    maquina_de_estados();
}
