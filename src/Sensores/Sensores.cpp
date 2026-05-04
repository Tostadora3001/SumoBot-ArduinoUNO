#include "Sensores.hh"
#include "../Pines/Pines.h"
#include <Arduino.h>
#include <Wire.h>

//Estos define definen los rangos maximos de detección de cada sensor para considerarlos como una detección válida.
#define RANGO_ULTRASONIDO 30
#define RANGO_LASER 70

int Flags_Sensores[10];

VL53L0X sensorIzq;
VL53L0X sensorFron;
VL53L0X sensorDer;

bool IR_Medicion(int pin);
int VL53L0X_Medicion(int DIR_SENSOR);
int HC_SR04_Medicion(int pin);

void Ejecutar_Sensores(){
    //Primero se ejecutan las lecturas de los sensores Infrarrojos dado que son los mas importantes, al evitar caidas. 
    //Si se detecta un borde, no es necesario realizar las lecturas de los demás sensores, ya que la prioridad máxima es evitar la caída.
    if(IR_Medicion(IR_FRONTAL_PIN)){
        Flags_Sensores[7] = 1;
        return; // Si se detecta un borde con el infrarrojo frontal, no es necesario medir el infrarrojo trasero, ya que la prioridad es evitar la caída.
    }
    else if(IR_Medicion(IR_TRASERO_PIN)){
        Flags_Sensores[8] = 1;
        return; // Si se detecta un borde con el infrarrojo trasero, no es necesario medir el infrarrojo frontal, ya que la prioridad es evitar la caída.
    }
    

    //Segundo se ejecutan las lecturas de los sensores laser dado que son mas rapidos. En caso de deteccion tambien se medira el ultrasonidos correspondiente, para mayor precison.
    int medicion_fron = VL53L0X_Medicion(DIR_SENSOR_FRONTAL);
    if(medicion_fron > 0 && medicion_fron < RANGO_LASER){
        Flags_Sensores[4] = medicion_fron;
        Flags_Sensores[0] = HC_SR04_Medicion(HC_FRONTAL_PIN);
        return; // Si se detecta algo con el láser frontal, no es necesario medir los sensores laterales, ya que la prioridad es atacar.
    }

    int medicion_der = VL53L0X_Medicion(DIR_SENSOR_DERECHA);
    if(medicion_der > 0 && medicion_fron < RANGO_LASER){
        Flags_Sensores[5] = medicion_der;
        Flags_Sensores[0] = HC_SR04_Medicion(HC_FRONTAL_PIN);
        return; // Si se detecta algo con el láser lateral, no es necesario medir el otro sensor lateral, ya que la prioridad es encarar al oponente.
    }

    int medicion_izq = VL53L0X_Medicion(DIR_SENSOR_IZQUIERDA);
    if(medicion_izq > 0 && medicion_fron < RANGO_LASER){
        Flags_Sensores[6] = medicion_izq;
        Flags_Sensores[0] = HC_SR04_Medicion(HC_FRONTAL_PIN);
        return; // Si se detecta algo con el láser lateral, no es necesario medir el otro sensor lateral, ya que la prioridad es encarar al oponente.
    }

    //Tercero, se ejecutan las lecturas de los sensores ultrasónicos. Dado que son los mas lentos, se ejecutan al final para evitar retrasos innecesarios en caso de detectar algo con los láseres o los infrarrojos.
    int medicion_ultra_fron = HC_SR04_Medicion(HC_FRONTAL_PIN);
    if(medicion_ultra_fron > 0 && medicion_ultra_fron < RANGO_ULTRASONIDO){
        Flags_Sensores[0] = medicion_ultra_fron;
        return; // Si se detecta algo con el ultrasonido frontal, no es necesario medir los sensores laterales, ya que la prioridad es atacar.
    }

    int medicion_ultra_der = HC_SR04_Medicion(HC_DERECHA_PIN);
    if(medicion_ultra_der > 0 && medicion_ultra_fron < RANGO_ULTRASONIDO){
        Flags_Sensores[1] = medicion_ultra_der;
        return; // Si se detecta algo con el ultrasonido lateral, no es necesario medir el otro sensor lateral, ya que la prioridad es encarar al oponente.
    }

    int medicion_ultra_izq = HC_SR04_Medicion(HC_IZQUIERDA_PIN);
    if(medicion_ultra_izq > 0 && medicion_ultra_fron < RANGO_ULTRASONIDO){
        Flags_Sensores[2] = medicion_ultra_izq;
        return; // Si se detecta algo con el ultrasonido lateral, no es necesario medir el otro sensor lateral, ya que la prioridad es encarar al oponente.
    }

    //En caso de no detectar nada con ningun sensor, se deja el vector de Flags_Sensores a 0, indicando que no se ha detectado nada.
    //Esta garantizado que antes de ejecutar esta funcion el vector Flags_Sensores se haya limpiado, por lo que no es necesario limpiarlo aquí.

    return;
}

//Esta funcion realiza la medición de distancia utilizando el sensor láser VL53L0X. 
//El pin DIR_SENSOR se utiliza para identificar a qué sensor se le está solicitando la medición. La función devuelve la distancia medida en centímetros.
//Si no se recibe una medición válida dentro del tiempo de espera, se devuelve 0.
int VL53L0X_Medicion(int DIR_SENSOR){
    int distancia_mm = 0;
    bool timeout = false;
    Serial.print("Medición VL53L0X - Sensor: ");
    switch (DIR_SENSOR) {
        case DIR_SENSOR_IZQUIERDA:
            Serial.println("Izquierda");
            break;
        case DIR_SENSOR_FRONTAL:
            Serial.println("Frontal");
            break;
        case DIR_SENSOR_DERECHA:
            Serial.println("Derecha");
            break;
        default:
            Serial.println("Desconocido");
            return 0; // ID no válido
    }

    // Seleccionamos a qué sensor pedirle el dato según el ID
    switch (DIR_SENSOR) {
        case DIR_SENSOR_IZQUIERDA:
        distancia_mm = sensorIzq.readRangeSingleMillimeters();
        timeout = sensorIzq.timeoutOccurred();
        break;
        
        case DIR_SENSOR_FRONTAL:
        distancia_mm = sensorFron.readRangeSingleMillimeters();
        timeout = sensorFron.timeoutOccurred();
        break;
        
        case DIR_SENSOR_DERECHA:
        distancia_mm = sensorDer.readRangeSingleMillimeters();
        timeout = sensorDer.timeoutOccurred();
        break;
        
        default:
        return 0; // ID no válido
    }

    // Filtro de errores o fuera de rango (> 8 metros)
    if (timeout || distancia_mm > 8000) {
        return 0;
    }

    // Devolvemos en centímetros
    return distancia_mm / 10;
}

//Esta funcion realiza la medición de distancia utilizando el sensor ultrasónico HC-SR04. 
//El pin especificado se utiliza tanto para enviar el pulso de disparo (Trigger) como para recibir el eco (Echo). 
//La función devuelve la distancia medida en centímetros. Si no se recibe un eco dentro del tiempo de espera, 
//se retorna un valor de 0 para indicar que no se detectó ningún objeto.
int HC_SR04_Medicion(int pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);

    pinMode(pin, INPUT);

    // Distancia máxima medible de 1,7 metros (340 m/s * 0.02 s / 2)
    long duracion = pulseIn(pin, HIGH, 10000); 

    if (duracion == 0) return 0;
    int distancia = duracion / 58; 
    if(distancia > RANGO_ULTRASONIDO) return 0; // Si la distancia supera el rango, se considera como no detectado
    
    return distancia;
}

bool IR_Medicion(int pin){
    return !digitalRead(pin);
}

void Detectar_INICIO(){
    //Esta función se encarga de detectar el inicio de la competición, ya sea mediante un botón físico o una señal que indique el inicio. 
    //Mientras no se detecte el inicio, el robot permanecerá en el estado de INICIO, sin ejecutar ninguna acción.
    if(digitalRead(PIN_INICIO) == HIGH){
        Flags_Sensores[9] = 1;
    }
    /*  Interruptor para la competición. Descomentar para uso final.
    else{
        Flags_Sensores[9] = 0;
    }
    */
}