/*!
 * Ejemplo de medici�n de tiempos con la clase Chrono:
 *
 * La clase Chrono permite la medici�n de tiempos en la aplicaci�n. 
 * 
 */

/* Cabeceras tidopLib */
#include "core/console.h"
#include "core/messages.h"

using namespace TL;

int main(int argc, char** argv)
{

  /* */
  Chrono chrono;
  chrono.run();  // Se inicia el cronometro
  //c�digo que se quiere medir
  chrono.stop(); // Se detiene el cronometro

  chrono.reset(); // Se reinicia el cronometro
  chrono.setMessage("Tiempo 2: "); // Se a�ade un mensaje para mostrar junto con el tiempo medido

  chrono.stop();

  /* Medici�n de tiempos en un bloque de c�digo */
  {
    Chrono chrono2("Medici�n de tiempos en un bloque");

    // Al salir del bloque y destruirse el objeto se llama al m�todo stop()
  }

}