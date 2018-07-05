/*!
 * Ejemplo de escritura de un fichero log:
 *
 * La clase Log permite generar un fichero log y escribir mensajes de información, advertencia o errores. 
 * Si la clase log esta suscrita al gestor de mensajes de la clase MessageManager los 
 * mensajes de la libreria se escriben automaticamente en el log
 */

#include <stdio.h>

/* Cabeceras tidopLib */
#include "core/console.h"
#include "core/messages.h"


using namespace TL;

int main(int argc, char** argv)
{

  const char *name = "Escritura de log";

  // Se definen los parámetros y opciones
  CmdParser cmdParser(name, "Ejemplo de escritura de un fichero log");
  cmdParser.addParameter("log", "Fichero log. Por defecto se toma el nombre de la aplicación con la extensión .log", true);

  // Parseo de los argumentos y comprobación de los mismos
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string log_file = cmdParser.getValue<std::string>("log");

  // Consola
  Console console;
  console.setTitle(name);                         // Titulo de la ventana de consola
  console.setLogLevel(MessageLevel::MSG_VERBOSE); // Se muestran todos los mensajes por consola
  
  // Se obtiene una instancia unica de la clase Log
  Log &log = Log::getInstance();

  /*
  Se establece el nivel de mensajes en el log. Los valores posibles son:
  
   - MSG_DEBUG:   Información extra para depuración.
   - MSG_ERROR:   Sólo errores.
   - MSG_WARNING: Warnings.
   - MSG_INFO:    Información.
   - MSG_VERBOSE: Todos los mensajes.
   
   Se pueden combinar varios niveles de log:

     log.setLogLevel(MessageLevel::MSG_WARNING | MessageLevel::MSG_ERROR);

  */
  log.setLogLevel(MessageLevel::MSG_VERBOSE); 
  
  // Si se ha pasado por comando el fichero log se establece. En caso contrario se escribe en la ruta del ejecutable con su mismo nombre
  if (log_file.empty() == false)
    log.setLogFile(log_file.c_str()); 

  // Escritura en el log:
  log.write("Prueba de escritura");

  // Se suscribe la clase log al gestor de mensajes
  MessageManager::getInstance().addListener(&log);

  // Se muestra un mensaje por consola y se escribe en el log
  msgInfo("prueba");

  // Se muestra una advertencia por consola y se escribe en el log
  msgWarning("Warning");

  // Se muestra un error por consola y se escribe en el log
  msgError("Error");

  return 0;
}
