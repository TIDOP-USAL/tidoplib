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
#include "img_process/img_processing.h"

using namespace TL;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{
  const char *name = "Consola";

  std::string file;
  std::string param2;
  bool bOpt;
  int val = 10;

  Command cmd(name, "Ejemplo de aplicación de consola");
//  Option arg1({"opt", "o"}, "opción");
  cmd.push_back(std::make_shared<Argument_<int, true>>("int", 'i', "integer", &val));
  cmd.push_back(std::make_shared<Argument_<bool, false>>("bool", 'b', "boolean", &bOpt));
  ///Command::Status err = cmd.parse(argc, argv);
  std::array<char const*, 4> _argv{"" , "--int", "2", "-b"};
  cmd.parse(_argv.size(), _argv.data());

//  double val_d = 2.5;
//  std::shared_ptr<Argument> arg_double = std::make_shared<Argument_<double, false>>("double", 'd', "double", &val_d);
//  arg_double->fromString("2.3");
//  std::string type_name = arg_double->typeName();
//  //  cmd.push_back(Option({"opt", "o"}, "opción"));




//  std::shared_ptr<ImgProcessingList> imgprolist = std::make_shared<ImgProcessingList>();
//  imgprolist->add(std::make_shared<Normalize>(0., 255.));

//  // Se definen los parámetros y opciones
//  CmdParser cmdParser(name, "Ejemplo de aplicación de consola");
//  cmdParser.addParameter("file", "Ejemplo de parámetro obligatorio. Ruta de un fichero.");
//  cmdParser.addParameter("param2", "Parámetro opcional", true, "valor_opcional");
//  cmdParser.addOption("opt", "Opción");
//  cmdParser.addParameterOption("list_opt", "opt1,opt2,opt3", "Listado de opciones. Elegir una de las disponibles");
  
//  // Parseo de los argumentos y comprobación de los mismos
//  CmdParser::Status status = cmdParser.parse(argc, argv);
//  if (status == CmdParser::Status::PARSE_ERROR ) {
//    return 1;
//  } else if (status == CmdParser::Status::PARSE_HELP) {
//    return 0;
//  }

//  // Recuperación de los valores de los argumentos
//  std::string file = cmdParser.getValue<std::string>("file");

//  // Este parametro tiene un valor por defecto con lo cual puede ser omitido
//  std::string param2 = cmdParser.getValue<std::string>("param2");
//  // Se comprueba si existe la opción
//  bool bOpt = cmdParser.hasOption("opt");
//  // Devuelve la opción. Se puede utilizar un enum o un int
//  options opt = cmdParser.getParameterOptionIndex<options>("list_opt");
//  int opt_i = cmdParser.getParameterOptionIndex<int>("list_opt");

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
