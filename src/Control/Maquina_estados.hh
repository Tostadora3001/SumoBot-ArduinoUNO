//El comportamiento del robot se define en esta clase, que es la encargada de gestionar los estados del robot 
//y las transiciones entre ellos. Cada estado representa una acción o comportamiento específico del robot, 
//y las transiciones se basan en las condiciones que se cumplen durante la ejecución del programa.
#ifndef MAQUINA_ESTADOS_HH
#define MAQUINA_ESTADOS_HH

#define INICIO 0            //Estado Inicial de Reposo
#define SENSORES 1          //Estado de Lectura de Sensores, donde el robot analiza su entorno para tomar decisiones.
#define ANALISIS 2          //Estado de Análisis de Datos, donde el robot procesa la información obtenida de los sensores para determinar su próxima acción.
#define BUSCAR 3            //Estado de Búsqueda de Oponente, donde el robot gira sobre si mismo para localizar a su oponente en el campo de batalla.
#define ATACAR 4            //Estado de Ataque, donde el robot se desplaza hacia su oponente para intentar empujarlo fuera del ring.
#define GIRAR 5             //Estado de Giro, donde el robot realiza un giro para cambiar su dirección y encarar al oponente.
#define NO_CAER 6           //Estado de Evitar Caída, donde el robot detecta los bordes del ring y realiza maniobras para evitar caer fuera del área de combate.

//Esta funcion es la encargada de gestionar los estados del robot y las transiciones entre ellos.
void maquina_de_estados();

//Esta funcion printa en terminal toda la informacion relevante.
void Log_INFO();

#endif 