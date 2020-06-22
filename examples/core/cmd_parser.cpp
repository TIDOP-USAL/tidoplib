/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


/*!
 * Ejemplo de aplicación de consola:
 *
 * Las clases Console y Command facilitan la construcción de aplicaciones de consola.
 * La clase Command permite definir los parámetros y opciones del programa. Con
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
#include <tidop/imgprocess/imgprocess.h>

#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;

enum class options {
  opt1,
  opt2,
  opt3
};

int main(int argc, char** argv)
{
  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name.c_str());                           // Titulo de la ventana de consola
  console.setLogLevel(MessageLevel::msg_verbose);               // Se muestran todos los mensajes por consola
  MessageManager::instance().addListener(&console);          // Se añade un escuchador de los mensajes emitidos por la aplicación

  std::string file;
  bool bOpt, bOpt2;
  int val;
  double val_d = 0.5;
  size_t idx = 0;
  std::vector<std::string> options;
  options.push_back("OPT0");
  options.push_back("OPT1");
  options.push_back("OPT2");
  options.push_back("OPT3");
  options.push_back("OPT4");

  Command cmd(cmd_name, "Ejemplo de aplicación de consola");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("file", 'f', "Ejemplo de parámetro obligatorio. Ruta de un fichero.", &file));
  cmd.push_back(std::make_shared<ArgumentIntegerRequired>("int", 'i', "Valor entero", &val));
  cmd.push_back(std::make_shared<ArgumentBooleanOptional>("bool", 'b', "boolean", &bOpt));
  cmd.push_back(std::make_shared<ArgumentBooleanOptional>("opt", 'o', "boolean2", &bOpt2));
  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("double", "Parámetro doble. Si se omite se toma el valor por defecto", &val_d));
  cmd.push_back(std::make_shared<ArgumentList_<std::string, false>>("options", "lista de opciones", options, &idx));

  /// Definición de ejemplos de la aplicación
  cmd.addExample(std::string(cmd_name).append(" --file c:/path/file.txt --int 30 -b"));
  cmd.addExample(std::string(cmd_name).append(" -fc:/path/file.txt -i30 -b"));

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

  msgInfo("file: %s", file.c_str());
  msgInfo("int: %i", val);
  msgInfo("bool: %s", bOpt ? "true" : "false");
  msgInfo("double: %f", val_d);

  return 0;
}
