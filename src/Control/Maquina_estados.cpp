#include "Maquina_estados.hh"
#include <Arduino.h>

int Estado = INICIO;
int Proximo_estado;

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

//Para mayor legibilidad se declaran
#define HC_FRONTAL 0
#define HC_DERECHA 1
#define HC_IZQUIERDA 2
#define HC_TRASERO 3
#define VL_FRONTAL 4
#define VL_DERECHA 5
#define VL_IZQUIERDA 6
#define IR_FRONTAL 7
#define IR_TRASERO 8
#define SENSOR_INICIO 9

/*
El vector de actuadores es un arreglo de integers que representa el estado o la acción que deben ejecutar los actuadores del robot.
Cada posición representa una acción a efectuar (Girar, moverse hacia adelante, ...). Cada valor contiene si se hace o no.

El orden de los actuadores en el vector es el siguiente:
    - [0] : Girar a la izquierda
    - [1] : Girar a la derecha
    - [2] : Mover hacia adelante
    - [3] : Mover hacia atrás
*/
extern int Flags_Actuadores[4];

//Para mayor legibilidad se declaran
#define GIRAR_IZQUIERDA 0
#define GIRAR_DERECHA 1
#define MOVER_ADELANTE 2
#define MOVER_ATRAS 3

#define NO_DETECTADO 0
#define ESTABA_DERECHA 1
#define ESTABA_IZQUIERDA 2
#define ESTABA_FRENTE 3

//Funciones externas declaradas en otros archivos
void Ejecutar_Sensores();
void Ejecutar_Actuadores();
void Detectar_INICIO();

//Esta función es la encargada de imprimir información relevante sobre el estado actual del robot, las lecturas de los sensores y las acciones que se van a ejecutar.
void Log_INFO();

void maquina_de_estados(){
    //Se limpian los actuadores al inicio de cada ciclo de la máquina de estados para evitar que se ejecuten acciones no deseadas.
    for(int i = 0; i < 4; i++){
        Flags_Actuadores[i] = 0;
    }

    int Ultimo_Lugar_Conocido = NO_DETECTADO; // Variable para almacenar el último lugar donde se detectó al oponente, para usarlo en caso de perder la detección.

    //Este if controla el apagado. La señal de INICIO debe estar siempre HIGH durante la ejecución.
    if(Flags_Sensores[SENSOR_INICIO] == 0) Estado = INICIO;

    //Lógica del proximo estado a ejecutar
    switch (Estado) {
        case INICIO:
            if(Flags_Sensores[SENSOR_INICIO] > 0) Proximo_estado = SENSORES;
            else Proximo_estado = INICIO;

            break;
        case SENSORES:
            Proximo_estado = ANALISIS;

            break;
        case ANALISIS:
            //Se usan else if para evitar acciones simultaneas. En caso de cumplirse varias condiciones, se ejecutará la acción de mayor prioridad.

            //Casos de NO_CAER:
            //Maxima Proioridad: Evitar Caída
            if(Flags_Sensores[IR_FRONTAL] > 0){
                Proximo_estado = NO_CAER;
                Flags_Actuadores[MOVER_ATRAS] = 1;
            }
            else if(Flags_Sensores[IR_TRASERO] > 0){
                 Proximo_estado = NO_CAER;
                 Flags_Actuadores[MOVER_ADELANTE] = 1;
            }
            //Casos de ATAQUE:
            //Segunda Prioridad: Atacar. En caso de estar cerca del oponente, se avanza hacia él pero a una distancia menor para intentar empujarlo fuera del ring.
            else if(Flags_Sensores[HC_FRONTAL] > 0 && Flags_Sensores[VL_FRONTAL] > 0){
                Proximo_estado = ATACAR;
                Flags_Actuadores[MOVER_ADELANTE] = 1;
            }

            //Si el láser frontal detecta al oponente, se avanza hacia él.
            else if(Flags_Sensores[VL_FRONTAL] > 0){
                Proximo_estado = ATACAR;
                Flags_Actuadores[MOVER_ADELANTE] = 1;
            }

            //Casos de GIRO:
            //En caso de estar cerca del oponente pero sin detectarlo con el láser frontal, se gira para intentar localizarlo.
            //Lado Derecho
            else if(Flags_Sensores[HC_FRONTAL] > 0 && Flags_Sensores[VL_DERECHA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_DERECHA] = 1;
            }

            //Lado Izquierdo
            else if(Flags_Sensores[HC_FRONTAL] > 0 && Flags_Sensores[VL_IZQUIERDA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_IZQUIERDA] = 1;
            }

            //En caso de estar lejos del oponente detectandolo con un laser lateral, se gira para intentar encararlo.
            //Lado Derecho
            else if(Flags_Sensores[VL_DERECHA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_DERECHA] = 1;
            }

            //Lado Izquierdo
            else if(Flags_Sensores[VL_IZQUIERDA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_IZQUIERDA] = 1;
            }

            //En caso de detectar al oponente con un ultrasonido lateral pero sin detectarlo con los láseres, se gira para intentar encararlo.
            //Lado Derecho
            else if(Flags_Sensores[HC_DERECHA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_DERECHA] = 1;
            }

            //Lado Izquierdo
            else if(Flags_Sensores[HC_IZQUIERDA] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_IZQUIERDA] = 1;
            }

            //En caso de detectarlo detrás, se gira para intentar encararlo.
            else if(Flags_Sensores[HC_TRASERO] > 0){
                Proximo_estado = GIRAR;
                Flags_Actuadores[GIRAR_IZQUIERDA] = 1;
            }

            //Casos de BUSCAR:
            //Si los sensores no detectan nada, se gira para intentar localizar al oponente.
            else{
                Proximo_estado = BUSCAR;
                if(Ultimo_Lugar_Conocido == ESTABA_DERECHA) Flags_Actuadores[GIRAR_DERECHA] = 1;
                else Flags_Actuadores[GIRAR_IZQUIERDA] = 1;
            }
            break;
        case BUSCAR:
            Proximo_estado = SENSORES;

            break;
        case ATACAR:
            Proximo_estado = SENSORES;

            break;
        case GIRAR:
            Proximo_estado = SENSORES;

            break;
        case NO_CAER:
            Proximo_estado = SENSORES;

            break;
    }

    //Una vez procesada la lógica del próximo estado a ejecutar, se limpian los sensores para evitar que se ejecuten acciones no deseadas en el próximo ciclo de la máquina de estados.
    for(int i = 0; i < 9; i++){
        Flags_Sensores[i] = 0;
    }

    //Lógica de acciones a ejecutar en cada estado
    switch (Estado) {
        case INICIO:
            //Estado de Reposo, no se ejecuta ninguna acción
            Detectar_INICIO(); // Se ejecuta la función de detección de inicio para actualizar el vector de sensores con el estado del inicio de la competición. 
            break;
        case SENSORES:
            //Se activan las mediciones de todos los sensores para actualizar el vector de Flags_Sensores.
            Ejecutar_Sensores();
            
            break;
        case ANALISIS:
            //El estado de Analisis es puramente lógico y no realiza n inguna acción física, solo establecen el próximo estado y las acciones a ejecutar.
            break;
        case BUSCAR:
            //Se ejecuta una busqueda girando sobre si mismo para intentar localizar al oponente en el proximo estado de Sensores.
            Ejecutar_Actuadores();
            
            break;
        case ATACAR:
            //Se avanza hacia el oponente para intentar empujarlo fuera del ring.
            Ejecutar_Actuadores();

            break;
        case GIRAR:
            //Se gira para intentar encararlo.
            Ejecutar_Actuadores();

            break;
        case NO_CAER:
            //Se evita la caida realizando maniobras para mantenerse dentro del ring.
            Ejecutar_Actuadores();

            break;
    }

    //Se imprime información relevante para debugging al final de cada ciclo de la máquina de estados.
    Log_INFO();

    //Se actualiza el estado actual al proximo estado al final de cada ciclo de la máquina de estados.
    Estado = Proximo_estado;
}


//Funciones de Información y Debugging
const char* get_estado_nombre(int estado){
    switch (estado) {
        case INICIO:
            return "INICIO";
        case SENSORES:
            return "SENSORES";
        case ANALISIS:
            return "ANALISIS";
        case BUSCAR:
            return "BUSCAR";
        case ATACAR:
            return "ATACAR";
        case GIRAR:
            return "GIRAR";
        case NO_CAER:
            return "NO_CAER";
        default:
            return "ESTADO DESCONOCIDO";
    }
}

void Log_INFO(){
    Serial.println("=== LOG INFO ===");
    Serial.println("\n--- ESTADOS ---");
    Serial.print("Estado actual: ");
    Serial.println(get_estado_nombre(Estado));
    Serial.print("Proximo estado: ");
    Serial.println(get_estado_nombre(Proximo_estado));
    
    Serial.println("\n--- SENSORES ---");
    Serial.print("Ultrasonido Frontal: ");
    Serial.println(Flags_Sensores[0]);
    Serial.print("Ultrasonido Derecha: ");
    Serial.println(Flags_Sensores[1]);
    Serial.print("Ultrasonido Izquierda: ");
    Serial.println(Flags_Sensores[2]);
    Serial.print("Ultrasonido Trasero: ");
    Serial.println(Flags_Sensores[3]);
    Serial.print("Laser Frontal: ");
    Serial.println(Flags_Sensores[4]);
    Serial.print("Laser Derecha: ");
    Serial.println(Flags_Sensores[5]);
    Serial.print("Laser Izquierda: ");
    Serial.println(Flags_Sensores[6]);
    Serial.print("Infrarrojo Frontal: ");
    Serial.println(Flags_Sensores[7]);
    Serial.print("Infrarrojo Trasero: ");
    Serial.println(Flags_Sensores[8]);
    Serial.print("Sensor de Inicio: ");
    Serial.println(Flags_Sensores[9]);
    
    Serial.println("\n--- ACTUADORES ---");
    Serial.print("Girar izquierda: ");
    Serial.println(Flags_Actuadores[0]);
    Serial.print("Girar derecha: ");
    Serial.println(Flags_Actuadores[1]);
    Serial.print("Mover adelante: ");
    Serial.println(Flags_Actuadores[2]);
    Serial.print("Mover atras: ");
    Serial.println(Flags_Actuadores[3]);
}