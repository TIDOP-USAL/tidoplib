/*!
 * Ejemplo de escritura de un fichero log:
 *
 * La clase Log permite generar un fichero log y escribir mensajes de información, advertencia o errores. 
 * Si la clase log esta suscrita al gestor de mensajes de la clase MessageManager los 
 * mensajes de la libreria se escriben automaticamente en el log
 */

#include <stdio.h>

/* Cabeceras tidopLib */
#include <tidop/core/log.h>
#include <tidop/core/console.h>
#include <tidop/core/messages.h>

#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;

int main(int argc, char** argv)
{

  const char *name = "Escritura de log";
  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  std::string log_file;

  // Se definen los parámetros y opciones
  Command cmd(cmd_name, "Ejemplo de escritura de un fichero log");
  cmd.push_back(std::make_shared<ArgumentStringOptional>("log", "Fichero log. Por defecto se toma el nombre de la aplicación con la extensión .log", &log_file));

  // Parseo de los argumentos y comprobación de los mismos
  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::parse_error ) {
    return 1;
  } else if (status == Command::Status::show_help) {
    return 0;
  } else if (status == Command::Status::show_licence) {
    return 0;
  } else if (status == Command::Status::show_version) {
    return 0;
  }

  // Consola
  Console &console = Console::getInstance();
  console.setTitle(name);                         // Titulo de la ventana de consola
  console.setLogLevel(MessageLevel::MSG_VERBOSE); // Se muestran todos los mensajes por consola
  MessageManager::getInstance().addListener(&console);

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
