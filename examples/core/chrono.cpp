/*!
 * Ejemplo de medición de tiempos con la clase Chrono:
 *
 * La clase Chrono permite la medición de tiempos en la aplicación. 
 * 
 */

/* Cabeceras tidopLib */

#include <tidop/core/console.h>
#include <tidop/core/messages.h>

using namespace tl;

int main(int argc, char** argv)
{

  Console &console = Console::instance();
  console.setTitle("Chrono Example");
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  /* */
  Chrono chrono;
  chrono.run();  // Se inicia el cronometro
  // [código que se quiere medir]
  chrono.stop(); // Se detiene el cronometro

  chrono.reset(); // Se reinicia el cronometro
  chrono.setMessage("Tiempo 2: "); // Se añade un mensaje para mostrar junto con el tiempo medido

  chrono.run();  // Se inicia el cronometro

  chrono.stop();

  /* Medición de tiempos en un bloque de código */
  {
    Chrono chrono2("Medición de tiempos en un bloque");

    // Al salir del bloque y destruirse el objeto se llama al método stop()
  }

}
