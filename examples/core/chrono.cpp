/*!
 * Ejemplo de medición de tiempos con la clase Chrono:
 *
 * La clase Chrono permite la medición de tiempos en la aplicación. 
 * 
 */

/* Cabeceras tidopLib */

#include <tidop/core/console.h>
#include <tidop/core/messages.h>

using namespace TL;

int main(int argc, char** argv)
{

  /* */
  Chrono chrono;
  chrono.run();  // Se inicia el cronometro
  //código que se quiere medir
  chrono.stop(); // Se detiene el cronometro

  chrono.reset(); // Se reinicia el cronometro
  chrono.setMessage("Tiempo 2: "); // Se añade un mensaje para mostrar junto con el tiempo medido

  chrono.stop();

  /* Medición de tiempos en un bloque de código */
  {
    Chrono chrono2("Medición de tiempos en un bloque");

    // Al salir del bloque y destruirse el objeto se llama al método stop()
  }

}
