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
#include <tidop/core/path.h>
#include <tidop/core/chrono.h>
#include <tidop/img/imgreader.h>
#include <tidop/featmatch/features.h>
#include <tidop/featmatch/agast.h>
#include <tidop/featmatch/akaze.h>
#include <tidop/featmatch/featio.h>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

std::shared_ptr<KeypointDetector> make_detector(const std::string &detector);
std::shared_ptr<DescriptorExtractor> make_descriptor(const std::string &descriptor, std::shared_ptr<KeypointDetector> detector);

struct agast_properties
{
  int threshold = agast_default_value_threshold;
  bool nonmax_suppression = agast_default_value_nonmax_suppression;
  std::string detector_type = agast_default_value_detector_type;
} agast;

struct akaze_properties
{
  std::string descriptorType = akaze_default_value_descriptor_type;
  int descriptorSize = akaze_default_value_descriptor_size;
  int descriptorChannels = akaze_default_value_descriptor_channels;
  double threshold = akaze_default_value_threshold;
  int octaves = akaze_default_value_octaves;
  int octaveLayers = akaze_default_value_octave_layers;
  std::string diffusivity = akaze_default_value_diffusivity;
} akaze;


int main(int argc, char** argv)
{

  Path app_path(argv[0]);
  std::string cmd_name = app_path.baseName().toString();

  /// Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name.c_str());
  console.setMessageLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);


  Path input_path;
  Path output_path;
  bool bOpt = false;
  bool bOpt2 = false;
  int val = 0;
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

  //akaze_properties akaze;
  //agast_properties agast;

  Command cmd("cmd_name", "Feature detection and descriptor extraction", {
              CreateArgumentPathRequired("input", 'i', "Image", &input_path),
              CreateArgumentPathRequired("output", 'o', "feature file", &output_path),
              std::make_shared<ArgumentList_<std::string, true>>("detector", "Detector: AGAST, AKAZE, BRISK, FAST, GFTT, KAZE, MSER, SIFT, ORB, STAR, SURF", detector_list, &idx_detector_list),
              std::make_shared<ArgumentList_<std::string, true>>("descriptor", "Descriptor: AKAZE, BRIEF, BRISK, DAISY, FREAK, HOG, KAZE, LATCH, LUCID, ORB, SIFT, SURF", descriptor_list, &idx_descriptor_list),
              CreateArgumentIntegerOptional("agast.threshold", "Agast threshold", &agast.threshold),
              CreateArgumentBooleanOptional("agast.nonmax_suppression", "Agast non maximal suppresion", &agast.nonmax_suppression),
              CreateArgumentStringOptional("agast.detector_type", "Agast detector type", &agast.detector_type),
              CreateArgumentStringOptional("akaze.descriptor_type", "Akaze descriptor type", &akaze.descriptorType),
              CreateArgumentIntegerOptional("akaze.descriptor_size", "Akaze descriptor size", &akaze.descriptorSize),
              CreateArgumentIntegerOptional("akaze.descriptor_channels", "Akaze descriptor channels", &akaze.descriptorChannels),
              CreateArgumentDoubleOptional("akaze.threshold", "Akaze threshold", &akaze.threshold),
              CreateArgumentIntegerOptional("akaze.octaves", "Akaze octaves", &akaze.octaves),
              CreateArgumentIntegerOptional("akaze.octave_layers", "Akaze octave layers", &akaze.octaveLayers),
              CreateArgumentStringOptional("akaze.diffusivity", "Akaze diffusivity", &akaze.diffusivity)
    });

  /// Definición de ejemplos de la aplicación
  //TODO: falta por definir
  //cmd_list.addExample(std::string(cmd_name).append(" --file c:/path/file.txt --int 30 -b"));
  //cmd_list.addExample(std::string(cmd_name).append(" -fc:/path/file.txt -i30 -b"));

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

  std::string detector = detector_list[idx_detector_list];
  std::string descriptor = descriptor_list[idx_descriptor_list];

  std::shared_ptr<KeypointDetector> keypointDetector = make_detector(detector);
  std::shared_ptr<DescriptorExtractor> descriptorExtractor = make_descriptor(descriptor, keypointDetector);

  try {

    if (!input_path.exists()) TL_THROW_EXCEPTION("Image doesn't exist: %s", input_path.fileName().toString().c_str());

    cv::Mat img = cv::imread(input_path.toString(), cv::IMREAD_IGNORE_ORIENTATION | cv::IMREAD_GRAYSCALE);
    if (img.empty()) TL_THROW_EXCEPTION("Could not load image: %s", input_path.toString().c_str());

    msgInfo("Searching Keypoints for image %s", input_path.toString().c_str());

    tl::Chrono chrono;
    chrono.run();

    std::vector<cv::KeyPoint> key_points = keypointDetector->detect(img);

    uint64_t time = chrono.stop();
    msgInfo("%i Keypoints detected in image %s [Time: %f seconds]", key_points.size(), input_path.fileName().toString().c_str(), time);

    msgInfo("Computing keypoints descriptors for image %s", input_path.fileName().toString().c_str());

    chrono.reset();
    chrono.run();

    cv::Mat descriptors = descriptorExtractor->extract(img, key_points);

    time = chrono.stop();
    msgInfo("Descriptors computed for image %s [Time: %f seconds]", input_path.fileName().toString().c_str(), time);

    std::unique_ptr<FeaturesWriter> writer = FeaturesWriterFactory::createWriter(output_path);
    writer->setKeyPoints(key_points);
    writer->setDescriptors(descriptors);
    writer->write();

    msgInfo("Write features at: %s", output_path.toString().c_str());

  } catch (const std::exception &e) {

    //tl::MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    printException(e);

  } catch (...) {

    msgError("Feature Extractor unknow exception");

  }

  return 0;
}




std::shared_ptr<KeypointDetector> make_detector(const std::string &detector)
{
  std::shared_ptr<KeypointDetector> keypoint_detector;

  if (detector == "AGAST") {
    keypoint_detector = std::make_shared<AgastDetector>(agast.threshold,
                                                        agast.nonmax_suppression,
                                                        agast.detector_type);
  } else if (detector == "AKAZE") {
    keypoint_detector = std::make_shared<AkazeDetectorDescriptor>(akaze.descriptorType,
                                                                  akaze.descriptorSize,
                                                                  akaze.descriptorChannels,
                                                                  akaze.threshold,
                                                                  akaze.octaves,
                                                                  akaze.octaveLayers,
                                                                  akaze.diffusivity);
  }
//  else if (keypointDetector.compare("BRISK") == 0) {
//    keypoint_detector = std::make_shared<BriskDetectorDescriptor>(mBriskDetector->threshold(),
//      mBriskDetector->octaves(),
//      mBriskDetector->patternScale());
//  }
//  else if (keypointDetector.compare("FAST") == 0) {
//    keypoint_detector = std::make_shared<FastDetector>(mFastDetector->threshold(),
//      mFastDetector->nonmaxSuppression(),
//      mFastDetector->detectorType());
//  }
//  else if (keypointDetector.compare("GFTT") == 0) {
//    keypoint_detector = std::make_shared<GfttDetector>(mGfttDetector->maxFeatures(),
//      mGfttDetector->qualityLevel(),
//      mGfttDetector->minDistance(),
//      mGfttDetector->blockSize(),
//      mGfttDetector->harrisDetector(),
//      mGfttDetector->k());
//  }
//  else if (keypointDetector.compare("KAZE") == 0) {
//    keypoint_detector = std::make_shared<KazeDetectorDescriptor>(mKazeDetector->extendedDescriptor(),
//      mKazeDetector->uprightDescriptor(),
//      mKazeDetector->threshold(),
//      mKazeDetector->octaves(),
//      mKazeDetector->octaveLayers(),
//      mKazeDetector->diffusivity());
//  }
//  else if (keypointDetector.compare("MSD") == 0) {
//    keypoint_detector = std::make_shared<MsdDetector>(mMsdDetector->thresholdSaliency(),
//      mMsdDetector->pathRadius(),
//      mMsdDetector->knn(),
//      mMsdDetector->areaRadius(),
//      mMsdDetector->scaleFactor(),
//      mMsdDetector->NMSRadius(),
//      mMsdDetector->nScales(),
//      mMsdDetector->NMSScaleR(),
//      mMsdDetector->computeOrientations(),
//      mMsdDetector->affineMSD(),
//      mMsdDetector->tilts());
//  }
//  else if (keypointDetector.compare("MSER") == 0) {
//    keypoint_detector = std::make_shared<MserDetector>(mMserDetector->delta(),
//      mMserDetector->minArea(),
//      mMserDetector->maxArea(),
//      mMserDetector->maxVariation(),
//      mMserDetector->minDiversity(),
//      mMserDetector->maxEvolution(),
//      mMserDetector->areaThreshold(),
//      mMserDetector->minMargin(),
//      mMserDetector->edgeBlurSize());
//  }
//  else if (keypointDetector.compare("ORB") == 0) {
//#ifdef HAVE_CUDA
//    if (mSettingsModel->useCuda()) {
//      keypoint_detector = std::make_shared<OrbCudaDetectorDescriptor>(mOrbDetector->featuresNumber(),
//        mOrbDetector->scaleFactor(),
//        mOrbDetector->levelsNumber(),
//        mOrbDetector->edgeThreshold(),
//        mOrbDetector->wta_k(),
//        mOrbDetector->scoreType(),
//        mOrbDetector->patchSize(),
//        mOrbDetector->fastThreshold());
//    }
//    else {
//#endif // HAVE_CUDA
//      keypoint_detector = std::make_shared<OrbDetectorDescriptor>(mOrbDetector->featuresNumber(),
//        mOrbDetector->scaleFactor(),
//        mOrbDetector->levelsNumber(),
//        mOrbDetector->edgeThreshold(),
//        mOrbDetector->wta_k(),
//        mOrbDetector->scoreType(),
//        mOrbDetector->patchSize(),
//        mOrbDetector->fastThreshold());
//
//#ifdef HAVE_CUDA
//    }
//#endif // HAVE_CUDA
//  }
//#ifdef OPENCV_ENABLE_NONFREE
//  else if (keypointDetector.compare("SIFT") == 0) {
//    keypoint_detector = std::make_shared<SiftDetectorDescriptor>(mSiftDetector->featuresNumber(),
//      mSiftDetector->octaveLayers(),
//      mSiftDetector->contrastThreshold(),
//      mSiftDetector->edgeThreshold(),
//      mSiftDetector->sigma());
//  }
//#endif
//  else if (keypointDetector.compare("STAR") == 0) {
//    keypoint_detector = std::make_shared<StarDetector>(mStarDetector->maxSize(),
//      mStarDetector->responseThreshold(),
//      mStarDetector->lineThresholdProjected(),
//      mStarDetector->lineThresholdBinarized(),
//      mStarDetector->suppressNonmaxSize());
//  }
//#ifdef OPENCV_ENABLE_NONFREE
//  else if (keypointDetector.compare("SURF") == 0) {
//#ifdef HAVE_CUDA
//    if (mSettingsModel->useCuda()) {
//      keypoint_detector = std::make_shared<SurfCudaDetectorDescriptor>(mSurfDetector->hessianThreshold(),
//        mSurfDetector->octaves(),
//        mSurfDetector->octaveLayers(),
//        mSurfDetector->extendedDescriptor(),
//        mSurfDetector->upright());
//    }
//    else {
//#endif // HAVE_CUDA
//
//      keypoint_detector = std::make_shared<SurfDetectorDescriptor>(mSurfDetector->hessianThreshold(),
//        mSurfDetector->octaves(),
//        mSurfDetector->octaveLayers(),
//        mSurfDetector->extendedDescriptor(),
//        mSurfDetector->upright());
//#ifdef HAVE_CUDA
//    }
//#endif // HAVE_CUDA
//  }
//#endif
  else {
    throw std::runtime_error("Keypoint Detector not found");
  }

  return keypoint_detector;
}

std::shared_ptr<DescriptorExtractor> make_descriptor(const std::string &descriptor, std::shared_ptr<KeypointDetector> detector)
{
  std::shared_ptr<DescriptorExtractor> descriptor_extractor;

  if (descriptor == "AKAZE") {
    if (std::dynamic_pointer_cast<Feature>(detector)->name() == "AKAZE") {
      descriptor_extractor = std::dynamic_pointer_cast<AkazeDetectorDescriptor>(detector);
    } else {
      descriptor_extractor = std::make_shared<AkazeDetectorDescriptor>(akaze.descriptorType,
                                                                       akaze.descriptorSize,
                                                                       akaze.descriptorChannels,
                                                                       akaze.threshold,
                                                                       akaze.octaves,
                                                                       akaze.octaveLayers,
                                                                       akaze.diffusivity);
    }
  }
//#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
//  else if (descriptorExtractor.compare("BOOST") == 0) {
//    descriptor_extractor = std::make_shared<BoostDescriptor>(mBoostDescriptor->descriptorType(),
//      mBoostDescriptor->useOrientation(),
//      mBoostDescriptor->scaleFactor());
//  }
//#endif
//  else if (descriptorExtractor.compare("BRIEF") == 0) {
//    descriptor_extractor = std::make_shared<BriefDescriptor>(mBriefDescriptor->bytes(),
//      mBriefDescriptor->useOrientation());
//  }
//  else if (descriptorExtractor.compare("BRISK") == 0) {
//    descriptor_extractor = std::make_shared<BriskDetectorDescriptor>(mBriskDescriptor->threshold(),
//      mBriskDescriptor->octaves(),
//      mBriskDescriptor->patternScale());
//  }
//  else if (descriptorExtractor.compare("DAISY") == 0) {
//    descriptor_extractor = std::make_shared<DaisyDescriptor>(mDaisyDescriptor->radius(),
//      mDaisyDescriptor->qRadius(),
//      mDaisyDescriptor->qTheta(),
//      mDaisyDescriptor->qHist(),
//      mDaisyDescriptor->norm(),
//      mDaisyDescriptor->interpolation(),
//      mDaisyDescriptor->useOrientation());
//  }
//  else if (descriptorExtractor.compare("FREAK") == 0) {
//    descriptor_extractor = std::make_shared<FreakDescriptor>(mFreakDescriptor->orientationNormalized(),
//      mFreakDescriptor->scaleNormalized(),
//      mFreakDescriptor->patternScale(),
//      mFreakDescriptor->octaves());
//  }
//  else if (descriptorExtractor.compare("HOG") == 0) {
//    descriptor_extractor = std::make_shared<HogDescriptor>(mHogDescriptor->winSize(),
//      mHogDescriptor->blockSize(),
//      mHogDescriptor->blockStride(),
//      mHogDescriptor->cellSize(),
//      mHogDescriptor->nbins(),
//      mHogDescriptor->derivAperture());
//  }
//  else if (descriptorExtractor.compare("KAZE") == 0) {
//    if (keypointDetector.compare("KAZE") == 0) {
//      descriptor_extractor = std::make_shared<KazeDetectorDescriptor>(mKazeDetector->extendedDescriptor(),
//        mKazeDetector->uprightDescriptor(),
//        mKazeDetector->threshold(),
//        mKazeDetector->octaves(),
//        mKazeDetector->octaveLayers(),
//        mKazeDetector->diffusivity());
//    }
//    else {
//      descriptor_extractor = std::make_shared<KazeDetectorDescriptor>(mKazeDescriptor->extendedDescriptor(),
//        mKazeDescriptor->uprightDescriptor(),
//        mKazeDescriptor->threshold(),
//        mKazeDescriptor->octaves(),
//        mKazeDescriptor->octaveLayers(),
//        mKazeDescriptor->diffusivity());
//    }
//  }
//  else if (descriptorExtractor.compare("LATCH") == 0) {
//    descriptor_extractor = std::make_shared<LatchDescriptor>(mLatchDescriptor->bytes(),
//      mLatchDescriptor->rotationInvariance(),
//      mLatchDescriptor->halfSsdSize());
//  } /* else if (descriptorExtractor.compare("LUCID") == 0){
//    descriptor_extractor = std::make_shared<LucidDescriptor>(mLucidDescriptor->lucidKernel(),
//                                                            mLucidDescriptor->blurKernel());
//  }*/ else if (descriptorExtractor.compare("LSS") == 0) {
//    descriptor_extractor = std::make_shared<LssDescriptor>();
//  }
//  else if (descriptorExtractor.compare("ORB") == 0) {
//#ifdef HAVE_CUDA
//    if (mSettingsModel->useCuda()) {
//      if (keypointDetector.compare("ORB") == 0) {
//        descriptor_extractor = std::make_shared<OrbCudaDetectorDescriptor>(mOrbDetector->featuresNumber(),
//          mOrbDetector->scaleFactor(),
//          mOrbDetector->levelsNumber(),
//          mOrbDetector->edgeThreshold(),
//          mOrbDetector->wta_k(),
//          mOrbDetector->scoreType(),
//          mOrbDetector->patchSize(),
//          mOrbDetector->fastThreshold());
//      }
//      else {
//        descriptor_extractor = std::make_shared<OrbCudaDetectorDescriptor>(mOrbDescriptor->featuresNumber(),
//          mOrbDescriptor->scaleFactor(),
//          mOrbDescriptor->levelsNumber(),
//          mOrbDescriptor->edgeThreshold(),
//          mOrbDescriptor->wta_k(),
//          mOrbDescriptor->scoreType(),
//          mOrbDescriptor->patchSize(),
//          mOrbDescriptor->fastThreshold());
//      }
//    }
//    else {
//#endif // HAVE_CUDA
//      if (keypointDetector.compare("ORB") == 0) {
//        descriptor_extractor = std::make_shared<OrbDetectorDescriptor>(mOrbDetector->featuresNumber(),
//          mOrbDetector->scaleFactor(),
//          mOrbDetector->levelsNumber(),
//          mOrbDetector->edgeThreshold(),
//          mOrbDetector->wta_k(),
//          mOrbDetector->scoreType(),
//          mOrbDetector->patchSize(),
//          mOrbDetector->fastThreshold());
//      }
//      else {
//        descriptor_extractor = std::make_shared<OrbDetectorDescriptor>(mOrbDescriptor->featuresNumber(),
//          mOrbDescriptor->scaleFactor(),
//          mOrbDescriptor->levelsNumber(),
//          mOrbDescriptor->edgeThreshold(),
//          mOrbDescriptor->wta_k(),
//          mOrbDescriptor->scoreType(),
//          mOrbDescriptor->patchSize(),
//          mOrbDescriptor->fastThreshold());
//      }
//#ifdef HAVE_CUDA
//    }
//#endif // HAVE_CUDA
//  }
//#ifdef OPENCV_ENABLE_NONFREE
//  else if (descriptorExtractor.compare("SIFT") == 0) {
//    if (keypointDetector.compare("SIFT") == 0) {
//      descriptor_extractor = std::make_shared<SiftDetectorDescriptor>(mSiftDetector->featuresNumber(),
//        mSiftDetector->octaveLayers(),
//        mSiftDetector->contrastThreshold(),
//        mSiftDetector->edgeThreshold(),
//        mSiftDetector->sigma());
//    }
//    else {
//      descriptor_extractor = std::make_shared<SiftDetectorDescriptor>(mSiftDescriptor->featuresNumber(),
//        mSiftDescriptor->octaveLayers(),
//        mSiftDescriptor->contrastThreshold(),
//        mSiftDescriptor->edgeThreshold(),
//        mSiftDescriptor->sigma());
//    }
//  }
//  else if (descriptorExtractor.compare("SURF") == 0) {
//#ifdef HAVE_CUDA
//    if (mSettingsModel->useCuda()) {
//      if (keypointDetector.compare("SURF") == 0) {
//        descriptor_extractor = std::make_shared<SurfCudaDetectorDescriptor>(mSurfDetector->hessianThreshold(),
//          mSurfDetector->octaves(),
//          mSurfDetector->octaveLayers(),
//          mSurfDetector->extendedDescriptor(),
//          mSurfDetector->upright());
//      }
//      else {
//        descriptor_extractor = std::make_shared<SurfCudaDetectorDescriptor>(mSurfDescriptor->hessianThreshold(),
//          mSurfDescriptor->octaves(),
//          mSurfDescriptor->octaveLayers(),
//          mSurfDescriptor->extendedDescriptor(),
//          mSurfDescriptor->upright());
//      }
//    }
//    else {
//#endif // HAVE_CUDA
//      if (keypointDetector.compare("SURF") == 0) {
//        descriptor_extractor = std::make_shared<SurfDetectorDescriptor>(mSurfDetector->hessianThreshold(),
//          mSurfDetector->octaves(),
//          mSurfDetector->octaveLayers(),
//          mSurfDetector->extendedDescriptor(),
//          mSurfDetector->upright());
//      }
//      else {
//        descriptor_extractor = std::make_shared<SurfDetectorDescriptor>(mSurfDescriptor->hessianThreshold(),
//          mSurfDescriptor->octaves(),
//          mSurfDescriptor->octaveLayers(),
//          mSurfDescriptor->extendedDescriptor(),
//          mSurfDescriptor->upright());
//      }
//#ifdef HAVE_CUDA
//    }
//#endif // HAVE_CUDA
//  }
//#endif
//#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
//  else if (descriptorExtractor.compare("VGG") == 0) {
//    descriptor_extractor = std::make_shared<VggDescriptor>(mVggDescriptor->descriptorType(),
//      mVggDescriptor->scaleFactor(),
//      mVggDescriptor->sigma(),
//      mVggDescriptor->useNormalizeDescriptor(),
//      mVggDescriptor->useNormalizeImage(),
//      mVggDescriptor->useScaleOrientation());
//  }
//#endif
  else {
    throw std::runtime_error("Descriptor Extractor not found");
  }

  return descriptor_extractor;
}


