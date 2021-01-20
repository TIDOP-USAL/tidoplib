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

#include <stdio.h>
#include <memory>
#include <iomanip>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geospatial/crs.h>
#include <tidop/img/imgreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>
#include <tidop/experimental/datamodel.h>
#include <tidop/experimental/camera.h>
#include <tidop/experimental/photo.h>
#include <tidop/geospatial/footprint.h>
#include <tidop/geospatial/ortho.h>

#include <opencv2/stitching.hpp>

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string bundle_file;
  std::string image_list;
  std::string image_path;
  std::string crs;
  std::string mdt;
  std::string ortho_path;
  std::string footprint_file;

  Command cmd(cmd_name, "Huella de vuelo");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("bundle_file", 'b', "Fichero bundle", &bundle_file));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("image_list", 'i', "Listado de imagenes", &image_list));
  cmd.push_back(std::make_shared<ArgumentStringOptional>("image_path", 'p', "Ruta de imagenes", &image_path));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("crs", 'c', "Código EPSG", &crs));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("mdt", 'm', "Modelo digital del terreno", &mdt));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("ortho_path", 'o', "Ruta ortofotos", &ortho_path));
  cmd.push_back(std::make_shared<ArgumentStringOptional>("footprint_file", 'f', "Fichero Shapefile con la huella de vuelo", &footprint_file));

  cmd.addExample(cmd_name + " --bundle_file bundle.rd.out --image_list bundle.rd.out.list.txt --crs EPSG:25830 -- mdt mdt.tif");

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

  try {

    /// Carga de imagenes 

    if (!fs::exists(image_list)) throw std::exception("Image list not found");

    std::vector<std::string> images;

    std::ifstream ifs;
    ifs.open(image_list, std::ifstream::in);
    if (ifs.is_open()) {

      std::string line;

      while (std::getline(ifs, line)) {
        
        if (fs::exists(line)) {
          images.push_back(line);
        } else {
          std::string image = std::string(image_path).append("\\").append(line);
          
          if (fs::exists(image)) {
            images.push_back(image);
          } else {
            std::string err = "Image not found: ";
            err.append(image);
            throw std::exception(err.c_str());
          }

        }
        

      }

      ifs.close();
    }

    /// Fin carga de imagenes 

    std::vector<experimental::Photo> photos;
    

    /// Lectura de fichero bundle

    std::unique_ptr<ImageReader> imageReader;

    if (!fs::exists(bundle_file)) throw std::exception("Bundle file not found");

    ifs.open(bundle_file, std::ifstream::in);
    if (ifs.is_open()) {
    
      std::string line;
      std::getline(ifs, line); // Salto primera linea

      std::getline(ifs, line);

      std::stringstream ss(line);

      int camera_count;
      int feature_count;
      ss >> camera_count >> feature_count;

      TL_ASSERT(camera_count == images.size(), "ERROR");

      for (size_t i = 0; i < camera_count; i++) {
      //while (std::getline(ifs, line)) {

        imageReader = ImageReaderFactory::createReader(images[i]);
        imageReader->open();
        int width = 0;
        int height = 0;
        if (imageReader->isOpen()) {
          height = imageReader->rows();
          width = imageReader->cols();
        }


        std::getline(ifs, line);
        ss.str(line);
        ss.clear();

        double focal;
        double k1;
        double k2;
        ss >> focal >> k1 >> k2;

        TL_TODO("¿Necesito algo de Camera o sólo de Calibration?")
        experimental::Camera camera;
        //camera.setMake("SONY");
        //camera.setModel("ILCE-6000");
        //camera.setFocal(16);
        camera.setHeight(height);
        camera.setWidth(width);
        camera.setType("Radial");
        //camera.setSensorSize(23.5);
        std::shared_ptr<experimental::Calibration> calibration = experimental::CalibrationFactory::create(camera.type());
        calibration->setParameter(tl::experimental::Calibration::Parameters::focal, focal);
        calibration->setParameter(tl::experimental::Calibration::Parameters::cx, 3006.23);        
        calibration->setParameter(tl::experimental::Calibration::Parameters::cy, 2024.27);
        //calibration->setParameter(tl::experimental::Calibration::Parameters::cx, 3000);        
        //calibration->setParameter(tl::experimental::Calibration::Parameters::cy, 2000);
        calibration->setParameter(tl::experimental::Calibration::Parameters::k1, k1);
        calibration->setParameter(tl::experimental::Calibration::Parameters::k2, k2);
        camera.setCalibration(calibration);

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r00;
        double r01;
        double r02;
        ss >> r00 >> r01 >> r02;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r10;
        double r11;
        double r12;
        ss >> r10 >> r11 >> r12;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r20;
        double r21;
        double r22;
        ss >> r20 >> r21 >> r22;

        math::RotationMatrix<double> rotation_matrix;
        rotation_matrix.at(0, 0) = r00;
        rotation_matrix.at(0, 1) = r01;
        rotation_matrix.at(0, 2) = r02;
        rotation_matrix.at(1, 0) = r10;
        rotation_matrix.at(1, 1) = r11;
        rotation_matrix.at(1, 2) = r12;
        rotation_matrix.at(2, 0) = r20;
        rotation_matrix.at(2, 1) = r21;
        rotation_matrix.at(2, 2) = r22;

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double tx;
        double ty;
        double tz;
        ss >> tx >> ty >> tz;
        
        //Point3D position(tx, ty, tz);
        //Point3D position(-5.7208 + 272021.61, -17.8296 + 4338369.137, 0.166741 + 314.874);
        Point3D offset(272021.250, 4338368.076, 379.370);
        Point3D position;

        // Paso de la transformación de mundo a imagen a imagen mundo

        math::RotationMatrix<double> rotation_transpose = rotation_matrix.transpose();


        position.x = -(rotation_transpose.at(0, 0) * tx +
                       rotation_transpose.at(0, 1) * ty +
                       rotation_transpose.at(0, 2) * tz) + offset.x;
        position.y = -(rotation_transpose.at(1, 0) * tx +
                       rotation_transpose.at(1, 1) * ty +
                       rotation_transpose.at(1, 2) * tz) + offset.y;
        position.z = -(rotation_transpose.at(2, 0) * tx +
                       rotation_transpose.at(2, 1) * ty +
                       rotation_transpose.at(2, 2) * tz) + offset.z;


        experimental::Photo::Orientation orientation(position, rotation_matrix);
        experimental::Photo photo(images[i]);
        photo.setCamera(camera);
        photo.setOrientation(orientation);
        photos.push_back(photo);
      }

      ifs.close();
    }

    /// Fin lectura de fichero bundle
    
    Orthorectification ortho(mdt);
    ortho.run(photos, ortho_path, footprint_file);


    /// Fusión de ortos en un unico mosaico
    size_t num_images = photos.size();
    std::vector<cv::Point> corners(num_images);
    std::vector<cv::UMat> masks_warped(num_images);
    std::vector<cv::UMat> images_warped(num_images);
//    std::vector<cv::Size> sizes(num_images);
//    std::vector<cv::UMat> masks(num_images);
//
//    std::vector<cv::UMat> seam_est_imgs_;
//
//    // Prepare image masks
//    for (size_t i = 0; i < num_images; ++i) {
//        masks[i].create(seam_est_imgs_[i].size(), CV_8U);
//        masks[i].setTo(cv::Scalar::all(255));
//    }
//
//    // Warp images and their masks
//
//    Ptr<WarperCreator> warper_creator;
//#ifdef HAVE_OPENCV_CUDAWARPING
//    if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//    {
//        if (warp_type == "plane")
//            warper_creator = makePtr<cv::PlaneWarperGpu>();
//        else if (warp_type == "cylindrical")
//            warper_creator = makePtr<cv::CylindricalWarperGpu>();
//        else if (warp_type == "spherical")
//            warper_creator = makePtr<cv::SphericalWarperGpu>();
//    }
//    else
//#endif
//    {
//        if (warp_type == "plane")
//            warper_creator = makePtr<cv::PlaneWarper>();
//        else if (warp_type == "affine")
//            warper_creator = makePtr<cv::AffineWarper>();
//        else if (warp_type == "cylindrical")
//            warper_creator = makePtr<cv::CylindricalWarper>();
//        else if (warp_type == "spherical")
//            warper_creator = makePtr<cv::SphericalWarper>();
//        else if (warp_type == "fisheye")
//            warper_creator = makePtr<cv::FisheyeWarper>();
//        else if (warp_type == "stereographic")
//            warper_creator = makePtr<cv::StereographicWarper>();
//        else if (warp_type == "compressedPlaneA2B1")
//            warper_creator = makePtr<cv::CompressedRectilinearWarper>(2.0f, 1.0f);
//        else if (warp_type == "compressedPlaneA1.5B1")
//            warper_creator = makePtr<cv::CompressedRectilinearWarper>(1.5f, 1.0f);
//        else if (warp_type == "compressedPlanePortraitA2B1")
//            warper_creator = makePtr<cv::CompressedRectilinearPortraitWarper>(2.0f, 1.0f);
//        else if (warp_type == "compressedPlanePortraitA1.5B1")
//            warper_creator = makePtr<cv::CompressedRectilinearPortraitWarper>(1.5f, 1.0f);
//        else if (warp_type == "paniniA2B1")
//            warper_creator = makePtr<cv::PaniniWarper>(2.0f, 1.0f);
//        else if (warp_type == "paniniA1.5B1")
//            warper_creator = makePtr<cv::PaniniWarper>(1.5f, 1.0f);
//        else if (warp_type == "paniniPortraitA2B1")
//            warper_creator = makePtr<cv::PaniniPortraitWarper>(2.0f, 1.0f);
//        else if (warp_type == "paniniPortraitA1.5B1")
//            warper_creator = makePtr<cv::PaniniPortraitWarper>(1.5f, 1.0f);
//        else if (warp_type == "mercator")
//            warper_creator = makePtr<cv::MercatorWarper>();
//        else if (warp_type == "transverseMercator")
//            warper_creator = makePtr<cv::TransverseMercatorWarper>();
//    }
//
//    if (!warper_creator)
//    {
//        cout << "Can't create the following warper '" << warp_type << "'\n";
//        return 1;
//    }
//	
//    Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
//	
//    for (int i = 0; i < num_images; ++i)
//    {
//        Mat_<float> K;
//        cameras[i].K().convertTo(K, CV_32F);
//        float swa = (float)seam_work_aspect;
//        K(0,0) *= swa; K(0,2) *= swa;
//        K(1,1) *= swa; K(1,2) *= swa;
//
//        corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
//        sizes[i] = images_warped[i].size();
//
//        warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
//    }
//	
//    std::vector<cv::UMat> images_warped_f(photos.size());
//    for (size_t i = 0; i < photos.size(); ++i)
//        images_warped[i].convertTo(images_warped_f[i], CV_32F);
//	
    /// 1 - Compensación de exposición
	
	int expos_comp_type = cv::detail::ExposureCompensator::GAIN_BLOCKS;
	
///*    
//    if (string(argv[i + 1]) == "no")
//      expos_comp_type = ExposureCompensator::NO;
//    else if (string(argv[i + 1]) == "gain")
//      expos_comp_type = ExposureCompensator::GAIN;
//    else if (string(argv[i + 1]) == "gain_blocks")
//      expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
//    else if (string(argv[i + 1]) == "channels")
//      expos_comp_type = ExposureCompensator::CHANNELS;
//    else if (string(argv[i + 1]) == "channels_blocks")
//      expos_comp_type = ExposureCompensator::CHANNELS_BLOCKS;
//    else {
//      cout << "Bad exposure compensation method\n";
//      return -1;
//    } */
			
    cv::Ptr<cv::detail::ExposureCompensator> compensator = cv::detail::ExposureCompensator::createDefault(expos_comp_type);

//    if (dynamic_cast<GainCompensator*>(compensator.get())){
//        GainCompensator* gcompensator = dynamic_cast<GainCompensator*>(compensator.get());
//        gcompensator->setNrFeeds(expos_comp_nr_feeds);
//    } else if (dynamic_cast<ChannelsCompensator*>(compensator.get())) {
//        ChannelsCompensator* ccompensator = dynamic_cast<ChannelsCompensator*>(compensator.get());
//        ccompensator->setNrFeeds(expos_comp_nr_feeds);
//    } else if (dynamic_cast<BlocksCompensator*>(compensator.get())) {
//        BlocksCompensator* bcompensator = dynamic_cast<BlocksCompensator*>(compensator.get());
//        bcompensator->setNrFeeds(expos_comp_nr_feeds);
//        bcompensator->setNrGainsFilteringIterations(expos_comp_nr_filtering);
//        bcompensator->setBlockSize(expos_comp_block_size, expos_comp_block_size);
//    }
    
  	compensator->feed(corners, images_warped, masks_warped);

  
    for (size_t i = 0; i < num_images; ++i) {

    }

//	
//    /// 2 - Busqueda de costuras (seam finder)
//
//	    vector<UMat> images_warped_f(num_images);
//
//    cv::Ptr<cv::detail::SeamFinder> seam_finder;
//	
///* 
//    if (seam_find_type == "no")
//        seam_finder = makePtr<detail::NoSeamFinder>();
//    else if (seam_find_type == "voronoi")
//        seam_finder = makePtr<detail::VoronoiSeamFinder>();
//    else if (seam_find_type == "gc_color")
//    {
//#ifdef HAVE_OPENCV_CUDALEGACY
//        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
//        else
//#endif
//            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);
//    }
//    else if (seam_find_type == "gc_colorgrad")
//    {
//#ifdef HAVE_OPENCV_CUDALEGACY
//        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
//            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
//        else
//#endif
//            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
//    }
//    else if (seam_find_type == "dp_color")
//        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);
//    else if (seam_find_type == "dp_colorgrad")
//        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
//    if (!seam_finder)
//    {
//        cout << "Can't create the following seam finder '" << seam_find_type << "'\n";
//        return 1;
//    } */
//     
//	seam_finder = makePtr<cv::detail::DpSeamFinder>(cv::detail::DpSeamFinder::COLOR);
//	
//    seam_finder->find(images_warped_f, corners, masks_warped);


    /// 3 - mezcla (blender)



  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
