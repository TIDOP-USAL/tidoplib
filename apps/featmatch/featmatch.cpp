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

#include <tidop/core/app.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/path.h>
#include <tidop/core/chrono.h>
#include <tidop/img/imgreader.h>
#include <tidop/featmatch/features.h>
#include <tidop/featmatch/featio.h>
#include <tidop/featmatch/kaze.h>
#include <tidop/featmatch/akaze.h>
#include <tidop/featmatch/brisk.h>
#include <tidop/featmatch/sift.h>
#include <tidop/featmatch/orb.h>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

int main(int argc, char **argv)
{

    Console &console = App::console();
    console.setTitle("Feature Extractor");
    console.setMessageLevel(MessageLevel::all);
    Message::instance().addMessageHandler(&console);

    auto arg_input = makeArgument<std::string>("input", 'i', "Image");
    auto arg_output = makeArgument<std::string>("output", 'o', "feature file");
    auto arg_format = makeArgument<std::string>("format", 'f', "Format", "XML");
    std::vector<std::string> options{"XML", "YML", "TXT", "BIN"};
    arg_format->setValidator(std::make_shared<ValuesValidator<std::string>>(options));
    auto arg_features_number = makeArgument<int>("features_number", 'n', "Features number", 5000);

    /* SIFT */

    auto cmd_sift = Command::create("sift", "SIFT", {
            arg_input,
            arg_output,
            arg_format,
            arg_features_number,
            makeArgument<int>("octave_layers", "Octave layers", 3),
            makeArgument<double>("contrast_threshold", "Contrast threshold", 0.04),
            makeArgument<double>("edge_threshold", "Edge threshold", 10.),
            makeArgument<double>("sigma", "Sigma", 1.6)
        });

    /* Akaze */

    auto arg_akaze_descriptor_type = makeArgument<std::string>("descriptor_type", "Akaze descriptor type", "MLDB");
    std::vector<std::string> descriptor_type_options{"KAZE", "KAZE_UPRIGHT", "MLDB", "MLDB_UPRIGHT"};
    arg_akaze_descriptor_type->setValidator(std::make_shared<ValuesValidator<std::string>>(descriptor_type_options));
    auto arg_akaze_diffusivity = makeArgument<std::string>("diffusivity", "Diffusivity", "DIFF_PM_G2");
    std::vector<std::string> diffusivity{"DIFF_PM_G1", "DIFF_PM_G2", "DIFF_WEICKERT", "DIFF_CHARBONNIER"};
    arg_akaze_diffusivity->setValidator(std::make_shared<ValuesValidator<std::string>>(diffusivity));
    auto arg_kaze_octaves = makeArgument<int>("octaves", "Octaves", 4);
    auto arg_kaze_octave_layers = makeArgument<int>("octave_layers", "Octave layers", 4);
    auto arg_kaze_threshold = makeArgument<int>("threshold", "Threshold", 0.001);

    auto cmd_akaze = Command::create("akaze", "AKAZE", {
            arg_input,
            arg_output,
            arg_format,
            arg_akaze_descriptor_type,
            makeArgument<int>("descriptor_size", "Descriptor size", 0),
            makeArgument<int>("descriptor_channels", "Descriptor channels", 3),
            arg_kaze_threshold,
            arg_kaze_octaves,
            arg_kaze_octave_layers,
            arg_akaze_diffusivity
        });

    /* kaze */

    auto cmd_kaze = Command::create("kaze", "KAZE", {
            arg_input,
            arg_output,
            arg_format,
            makeArgument<bool>("extended", "Extended descriptor", false),
            makeArgument<bool>("upright", "Upright descriptor", false),
            arg_kaze_threshold,
            arg_kaze_octaves,
            arg_kaze_octave_layers,
            arg_akaze_diffusivity
        });

    /* BRISK */

    auto cmd_brisk = Command::create("brisk", "BRISK", {
            arg_input,
            arg_output,
            arg_format,
            arg_akaze_descriptor_type,
            makeArgument<int>("threshold", "Threshold", 30),
            makeArgument<int>("octaves", "Octaves", 3),
            makeArgument<double>("pattern_scale", "Pattern scale", 1.0)
        });

    /* ORB */

    auto arg_orb_score_type = std::make_shared<Argument_<std::string>>("score_type", "Score Type", "Harris");
    //std::vector<std::string> score_type{"Harris", "FAST"};
    //arg_orb_score_type->setValidator(std::make_shared<ValuesValidator<std::string>>(score_type));
    arg_orb_score_type->setValidator(ValuesValidator<std::string>::create({"Harris", "FAST"}));

    auto cmd_orb = Command::create("orb", "Oriented FAST and rotated BRIEF (ORB)", {
            arg_input,
            arg_output,
            arg_format,
            arg_features_number,
            makeArgument<double>("scale_factor", "Scale Factor", 1.2),
            makeArgument<int>("levels_number", "Levels Number", 0.04),
            makeArgument<int>("edge_threshold", "Edge threshold", 31),
            makeArgument<int>("first_level", "First Level", 0),
            makeArgument<int>("wta_k", "WTA K", 2),
            arg_orb_score_type,
            makeArgument<int>("patch_size", "Patch Size", 31),
            makeArgument<int>("fast_threshold", "Fast Threshold", 20)
        });


    CommandList cmd("featextract", "Feature detection and descriptor extraction");
    cmd.addCommand(cmd_sift);
    cmd.addCommand(cmd_akaze);
    cmd.addCommand(cmd_kaze);
    cmd.addCommand(cmd_brisk);
    cmd.addCommand(cmd_orb);

    // Parseo de los argumentos y comprobación de los mismos
    CommandList::Status status = cmd.parse(argc, argv);
    if(status == CommandList::Status::parse_error) {
        return 1;
    } else if(status == CommandList::Status::show_help) {
        return 0;
    } else if(status == CommandList::Status::show_licence) {
        return 0;
    } else if(status == CommandList::Status::show_version) {
        return 0;
    }

    Path input_path = arg_input->value();
    Path output_path = arg_output->value();
    std::string format = arg_format->value();

    std::shared_ptr<KeypointDetector> keypoint_detector;
    std::shared_ptr<DescriptorExtractor> descriptor_extractor;

    std::string featextract_name = cmd.commandName();

    if(featextract_name == "SIFT") {

        auto sift = std::make_shared<SiftDetectorDescriptor>(cmd_sift->value<int>("features_number"),
                                                             cmd_sift->value<int>("octave_layers"),
                                                             cmd_sift->value<double>("contrast_threshold"),
                                                             cmd_sift->value<double>("edge_threshold"),
                                                             cmd_sift->value<double>("sigma"));
        keypoint_detector = std::dynamic_pointer_cast<KeypointDetector>(sift);
        descriptor_extractor = std::dynamic_pointer_cast<DescriptorExtractor>(sift);

    } else if(featextract_name == "AKAZE") {
        
        auto akaze = std::make_shared<AkazeDetectorDescriptor>(cmd_akaze->value<std::string>("descriptor_type"),
                                                               cmd_akaze->value<int>("descriptor_size"),
                                                               cmd_akaze->value<int>("descriptor_channels"),
                                                               cmd_akaze->value<double>("threshold"),
                                                               cmd_akaze->value<int>("octaves"),
                                                               cmd_akaze->value<int>("octave_layers"),
                                                               cmd_akaze->value<std::string>("diffusivity"));

        keypoint_detector = std::dynamic_pointer_cast<KeypointDetector>(akaze);
        descriptor_extractor = std::dynamic_pointer_cast<DescriptorExtractor>(akaze);

    } else if(featextract_name == "KAZE") {
        
        auto kaze = std::make_shared<KazeDetectorDescriptor>(cmd_kaze->value<bool>("extended"),
                                                             cmd_kaze->value<bool>("upright"),
                                                             cmd_kaze->value<double>("threshold"),
                                                             cmd_kaze->value<int>("octaves"),
                                                             cmd_kaze->value<int>("octave_layers"),
                                                             cmd_kaze->value<std::string>("diffusivity"));

        keypoint_detector = std::dynamic_pointer_cast<KeypointDetector>(kaze);
        descriptor_extractor = std::dynamic_pointer_cast<DescriptorExtractor>(kaze);

    } else if(featextract_name == "BRISK") {

        auto brisk = std::make_shared<BriskDetectorDescriptor>(cmd_brisk->value<int>("threshold"),
                                                               cmd_brisk->value<int>("octaves"),
                                                               cmd_brisk->value<double>("patternScale"));

        keypoint_detector = std::dynamic_pointer_cast<KeypointDetector>(brisk);
        descriptor_extractor = std::dynamic_pointer_cast<DescriptorExtractor>(brisk);

    } else if(featextract_name == "ORB") {

        auto orb = std::make_shared<OrbDetectorDescriptor>(cmd_orb->value<int>("features_number"),
                                                           cmd_orb->value<double>("scale_factor"),
                                                           cmd_orb->value<int>("levels_number"),
                                                           cmd_orb->value<int>("edge_threshold"),
                                                           cmd_orb->value<int>("wta_k"),
                                                           cmd_orb->value<std::string>("score_type"),
                                                           cmd_orb->value<int>("patch_size"),
                                                           cmd_orb->value<int>("fast_threshold"));

        keypoint_detector = std::dynamic_pointer_cast<KeypointDetector>(orb);
        descriptor_extractor = std::dynamic_pointer_cast<DescriptorExtractor>(orb);
    }


    try {

        if(!input_path.exists()) TL_THROW_EXCEPTION("Image doesn't exist: {}", input_path.fileName().toString());

        cv::Mat img = cv::imread(input_path.toString(), cv::IMREAD_IGNORE_ORIENTATION | cv::IMREAD_GRAYSCALE);
        if(img.empty()) TL_THROW_EXCEPTION("Could not load image: {}", input_path.toString());

        Message::info("Searching Keypoints for image {}", input_path.toString());

        tl::Chrono chrono;
        chrono.run();

        std::vector<cv::KeyPoint> key_points = keypoint_detector->detect(img);

        uint64_t time = chrono.stop();
        Message::info("{} Keypoints detected in image {} [Time: {} seconds]", key_points.size(), input_path.fileName().toString(), time);

        Message::info("Computing keypoints descriptors for image {}", input_path.fileName().toString());

        chrono.reset();
        chrono.run();

        cv::Mat descriptors = descriptor_extractor->extract(img, key_points);

        time = chrono.stop();
        Message::info("Descriptors computed for image {} [Time: {} seconds]", input_path.fileName().toString(), time);

        std::unique_ptr<FeaturesWriter> writer = FeaturesWriterFactory::create(output_path);
        writer->setKeyPoints(key_points);
        writer->setDescriptors(descriptors);
        writer->write();

        Message::info("Write features at: {}", output_path.toString());

    } catch(const std::exception &e) {
        printException(e);

    } catch(...) {

        Message::error("Feature Extractor unknow exception");

    }

    return 0;
}




//std::shared_ptr<KeypointDetector> make_detector(const std::string &detector)
//{
//  std::shared_ptr<KeypointDetector> keypoint_detector;

  //if (detector == "AGAST") {
  //  keypoint_detector = std::make_shared<AgastDetector>(agast.threshold,
  //                                                      agast.nonmax_suppression,
  //                                                      agast.detector_type);
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
//  else {
//    throw std::runtime_error("Keypoint Detector not found");
//  }
//
//  return keypoint_detector;
//}

//std::shared_ptr<DescriptorExtractor> make_descriptor(const std::string &descriptor, std::shared_ptr<KeypointDetector> detector)
//{
//  std::shared_ptr<DescriptorExtractor> descriptor_extractor;
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
 /* else {
    throw std::runtime_error("Descriptor Extractor not found");
  }

  return descriptor_extractor;
}*/


