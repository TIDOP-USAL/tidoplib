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
 * Ejemplo de ejecución de un proceso:
 * 
 */

#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/core/process.h>

#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;


class ProcessExample
  : public Process
{
public:
  ProcessExample();
  ~ProcessExample() override;

private:


  // Heredado vía Process
  virtual void execute(Progress *progressBar = nullptr) override
  {


    return;
  }

};

ProcessExample::ProcessExample()
  : Process()
{
}

ProcessExample::~ProcessExample()
{
}

int main(int argc, char** argv)
{
  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name.c_str());
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string file;
  bool bOpt;

  Command cmd(cmd_name, "Ejemplo de aplicación de consola");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("file", 'f', "Ejemplo de parámetro obligatorio. Ruta de un fichero.", &file));
  cmd.push_back(std::make_shared<ArgumentBooleanOptional>("opt", 'o', "boolean2", &bOpt));

  /// Definición de ejemplos de la aplicación
  cmd.addExample(std::string(cmd_name).append(" --file c:/path/file.txt -o"));
  cmd.addExample(std::string(cmd_name).append(" -fc:/path/file.txt -o"));

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

  ProcessExample process;
  process.run();


  return 0;
}
