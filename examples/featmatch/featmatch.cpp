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

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
//#include <tidop/img/imgio.h>
#include <tidop/featmatch/features.h>

#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

struct akaze_properties
{
  std::string descriptorType;
  int descriptorSize;
  int descriptorChannels;
  double threshold;
  int octaves;
  int octaveLayers;
  std::string diffusivity;
};


int main(int argc, char** argv)
{

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  /// Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name.c_str());
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);


  std::string file;
  std::string output;
  bool bOpt, bOpt2;
  int val;
  double val_d = 0.5;
  size_t idx_detector_list = 0;
  std::vector<std::string> detector_list;
  detector_list.push_back("AGAST");
  detector_list.push_back("AKAZE");
  detector_list.push_back("BRISK");
  detector_list.push_back("FAST");
  detector_list.push_back("GFTT");
  detector_list.push_back("KAZE");
  detector_list.push_back("MSER");
  detector_list.push_back("ORB");
  detector_list.push_back("SIFT");
  detector_list.push_back("STAR");
  detector_list.push_back("SURF");

  size_t idx_descriptor_list = 0;
  std::vector<std::string> descriptor_list;
  descriptor_list.push_back("AKAZE");
  descriptor_list.push_back("BRIEF");
  descriptor_list.push_back("BRISK");
  descriptor_list.push_back("DAISY");
  descriptor_list.push_back("FREAK");
  descriptor_list.push_back("HOG");
  descriptor_list.push_back("KAZE");
  descriptor_list.push_back("LATCH");
  descriptor_list.push_back("LUCID");
  descriptor_list.push_back("ORB");
  descriptor_list.push_back("SIFT");
  descriptor_list.push_back("SURF");

  akaze_properties akaze;

  std::shared_ptr<Command> cmd(new Command("featextract","Feature detection and descriptor extraction", {
                                             std::make_shared<ArgumentList_<std::string, true>>("detector", "Detector: AGAST, AKAZE, BRISK, FAST, GFTT, KAZE, MSER, SIFT, ORB, STAR, SURF", detector_list, &idx_detector_list),
                                             std::make_shared<ArgumentList_<std::string, true>>("descriptor", "Descriptor: AKAZE, BRIEF, BRISK, DAISY, FREAK, HOG, KAZE, LATCH, LUCID, ORB, SIFT, SURF", descriptor_list, &idx_descriptor_list),
                                             std::make_shared<ArgumentStringOptional>("akaze.descriptor_type", "Akaze descriptor type", &akaze.descriptorType),
                                             std::make_shared<ArgumentIntegerOptional>("akaze.descriptor_size", "Akaze descriptor size", &akaze.descriptorSize),
                                             std::make_shared<ArgumentIntegerOptional>("akaze.descriptor_channels", "Akaze descriptor channels", &akaze.descriptorChannels),
                                             std::make_shared<ArgumentDoubleOptional>("akaze.threshold", "Akaze threshold", &akaze.threshold),
                                             std::make_shared<ArgumentIntegerOptional>("akaze.octaves", "Akaze octaves", &akaze.octaves),
                                             std::make_shared<ArgumentIntegerOptional>("akaze.octave_layers", "Akaze octave layers", &akaze.octaveLayers),
                                             std::make_shared<ArgumentStringOptional>("akaze.diffusivity", "Akaze diffusivity", &akaze.diffusivity)
                                           }));
  std::shared_ptr<Command> cmd2(new Command("featmatch","Feature matching", {
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

  return 0;
}
