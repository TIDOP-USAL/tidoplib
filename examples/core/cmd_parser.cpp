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
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img_process/img_processing.h>

using namespace TL;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{
  const char *name = "Consola";
   
  // Consola
  Console console;
  console.setTitle(name);                           // Titulo de la ventana de consola
  //console.setConsoleUnicode();
  //console.setFontHeight(24);                      // Se establece el tamaño de fuente
  console.setLogLevel(MessageLevel::MSG_VERBOSE);   // Se muestran todos los mensajes por consola

  if (1){ /// Nueva gestión de argumentos por consola
    std::string file;
    bool bOpt;
    int val;
    double val_d = 0.5;

    Command cmd(name, "Ejemplo de aplicación de consola");
    cmd.push_back(std::make_shared<ArgumentStringRequired>("file", 'f', "Ejemplo de parámetro obligatorio. Ruta de un fichero.", &file));
    cmd.push_back(std::make_shared<ArgumentIntegerRequired>("int", 'i', "Valor entero obligatorio", &val));
    cmd.push_back(std::make_shared<ArgumentBooleanOptional>("bool", 'b', "boolean", &bOpt));
    cmd.push_back(std::make_shared<ArgumentDoubleOptional>("double", "Parámetro doble opcional. Si se omite se toma el valor por defecto", &val_d));

    // Parseo de los argumentos y comprobación de los mismos
    Command::Status status = cmd.parse(argc, argv);
    if (status == Command::Status::PARSE_ERROR ) {
      return 1;
    } else if (status == Command::Status::SHOW_HELP) {
      return 0;
    } else if (status == Command::Status::SHOW_LICENCE) {
      return 0;
    } else if (status == Command::Status::SHOW_VERSION) {
      return 0;
    }

    msgInfo("file: %s", file.c_str());
    msgInfo("int: %i", val);
    msgInfo("bool: %s", bOpt ? "true" : "false");
    msgInfo("double: %f", val_d);

  } else {
#ifdef TL_SHOW_DEPRECATED
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
    std::string file = cmdParser.getValue<std::string>("file");

    // Este parametro tiene un valor por defecto con lo cual puede ser omitido
    std::string param2 = cmdParser.getValue<std::string>("param2");
    // Se comprueba si existe la opción
    bool bOpt = cmdParser.hasOption("opt");
    // Devuelve la opción. Se puede utilizar un enum o un int
    options opt = cmdParser.getParameterOptionIndex<options>("list_opt");
    int opt_i = cmdParser.getParameterOptionIndex<int>("list_opt");
#endif
  }


  // Se muestra un mensaje por consola
  msgInfo("prueba");
  msgWarning("Warning");

  return 0;
}
