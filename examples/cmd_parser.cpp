/*!
 * Ejemplo de aplicación de consola:
 *
 * La clase CmdParser facilita la construcción de aplicaciones de consola. 
 * Esta clase permite definir los parámetros y opciones del programa. Con 
 * los parámetros añadidos se parsean los argumentos del comando introducido
 * comprobando si estos son correctos.
 *
 * La clase Console permite manipular la apariencia de la consola y mostrar
 * mensajes por pantalla.
 * 
 */

#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"


using namespace TL;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{

//  char name[TL_MAX_FNAME];
//  getFileName(getRunfile(), name, TL_MAX_FNAME);
  const char *name = "Consola";
  // Se definen los parámetros y opciones
  CmdParser cmdParser(name, "Ejemplo de aplicación de consola");
  cmdParser.addParameter("file", "Ejemplo de parámetro obligatorio. Ruta de un fichero.");
  cmdParser.addParameter("param2", "Parámetro opcional", true, "valor_opcional");
  cmdParser.addOption("opt", "Opción");
  cmdParser.addParameterOption("list_opt", "opt1,opt2,opt3", "Listado de opciones. Elegir una de las disponibles");
  
  // Parseo de los argumentos y comprobación de los mismos
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  // Recuperación de los valores de los argumentos
  // file es un fichero con lo cual se recomienda utilizar la clase Path para 
  // evitar problemas en el caso de que se introduzca una ruta relativa
  std::string file = cmdParser.getValue<Path>("file").toString();
  // Este parametro tiene un valor por defecto con lo cual puede ser omitido
  std::string param2 = cmdParser.getValue<std::string>("param2");
  // Se comprueba si existe la opción
  bool bOpt = cmdParser.hasOption("opt");
  // Devuelve la opción. Se puede utilizar un enum o un int
  options opt = cmdParser.getParameterOptionIndex<options>("list_opt");
  int opt_i = cmdParser.getParameterOptionIndex<int>("list_opt");

  // Consola
  Console console;
  console.setTitle(name);                         // Titulo de la ventana de consola
  //console.setConsoleUnicode();
  //console.setFontHeight(24);                      // Se establece el tamaño de fuente
  console.setLogLevel(MessageLevel::MSG_VERBOSE); // Se muestran todos los mensajes por consola
  
  // Se muestra un mensaje por consola
  msgInfo("prueba");
  msgWarning("Warning");

  return 0;
}
