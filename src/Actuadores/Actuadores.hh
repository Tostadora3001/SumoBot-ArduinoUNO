#ifndef ACTUADORES_HH
#define ACTUADORES_HH

/*
El vector de actuadores es un arreglo de integers que representa el estado o la acción que deben ejecutar los actuadores del robot.
Cada posición representa una acción a efectuar (Girar, moverse hacia adelante, ...). Cada valor contiene la cantidad (distancia o grados)
a efectuar por la acción.

El orden de los actuadores en el vector es el siguiente:
    - [0] : Girar a la izquierda
    - [1] : Girar a la derecha
    - [2] : Mover hacia adelante
    - [3] : Mover hacia atrás
*/
extern int Flags_Actuadores[4];

//Esta funcion ejecuta las acciones correspondientes a cada actuador según los valores establecidos en el vector de Flags_Actuadores.
void Ejecutar_Actuadores();

#endif