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

  /// Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name.c_str());                           // Titulo de la ventana de consola
  console.setLogLevel(MessageLevel::msg_verbose);               // Se muestran todos los mensajes por consola
  MessageManager::instance().addListener(&console);          // Se añade un escuchador de los mensajes emitidos por la aplicación

  std::string file;
  std::string output;
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

  std::shared_ptr<Command> cmd(new Command("cmd1","comando 1", {
                                             std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file)
                                           }));
  std::shared_ptr<Command> cmd2(new Command("cmd2","comando 2", {
                                              std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file),
                                              std::make_shared<ArgumentStringRequired>("output", 'o', "Fichero de salida", &output)
                                            }));

  CommandList cmd_list(cmd_name, "Ejemplo de listado de comandos");
  cmd_list.push_back(cmd);
  cmd_list.push_back(cmd2);

  /// Definición de ejemplos de la aplicación
  //TODO: falta por definir
  //cmd_list.addExample(std::string(cmd_name).append(" --file c:/path/file.txt --int 30 -b"));
  //cmd_list.addExample(std::string(cmd_name).append(" -fc:/path/file.txt -i30 -b"));

  // Parseo de los argumentos y comprobación de los mismos
  CommandList::Status status = cmd_list.parse(argc, argv);
  if (status == CommandList::Status::parse_error ) {
    return 1;
  } else if (status == CommandList::Status::show_help) {
    return 0;
  } else if (status == CommandList::Status::show_licence) {
    return 0;
  } else if (status == CommandList::Status::show_version) {
    return 0;
  }

  msgInfo("file: %s", file.c_str());
//  msgInfo("int: %i", val);
//  msgInfo("bool: %s", bOpt ? "true" : "false");
//  msgInfo("double: %f", val_d);

  return 0;
}
