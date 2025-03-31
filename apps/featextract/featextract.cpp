/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include <tidop/core/app/app.h>
#include <tidop/core/app/message.h>
#include <tidop/core/base/path.h>
#include <tidop/core/base/chrono.h>
#include <tidop/rastertools/io/imgreader.h>
#include <tidop/featmatch/features/features.h>
#include <tidop/featmatch/features/agast.h>
#include <tidop/featmatch/features/kaze.h>
#include <tidop/featmatch/features/akaze.h>
#include <tidop/featmatch/features/brisk.h>
#include <tidop/featmatch/features/sift.h>
#include <tidop/featmatch/features/orb.h>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif
#include <tidop/featmatch/io/featwriter.h>

int main(int argc, char **argv)
{

    Console &console = App::console();
    console.setTitle("Feature Extractor");
    console.setMessageLevel(MessageLevel::all);
    Message::addMessageHandler(&console);

    auto arg_input = Argument::make<Path>("input", 'i', "Image");
    auto arg_output = Argument::make<Path>("output", 'o', "Output path");
    auto arg_format = Argument::make<std::string>("format", 'f', "Format", "XML");
    std::vector<std::string> options{"XML", "YML", "TXT", "BIN"};
    arg_format->setValidator(std::make_shared<ValuesValidator<std::string>>(options));
    auto arg_features_number = Argument::make<int>("features_number", 'n', "Features number", 5000);

    /* SIFT */

    auto cmd_sift = Command::create("sift", "SIFT", {
            arg_input,
            arg_output,
            arg_format,
            arg_features_number,
            Argument::make<int>("octave_layers", "Octave layers", 3),
            Argument::make<double>("contrast_threshold", "Contrast threshold", 0.04),
            Argument::make<double>("edge_threshold", "Edge threshold", 10.),
            Argument::make<double>("sigma", "Sigma", 1.6)
        });

    /* Akaze */

    auto arg_akaze_descriptor_type = Argument::make<std::string>("descriptor_type", "Akaze descriptor type", "MLDB");
    std::vector<std::string> descriptor_type_options{"KAZE", "KAZE_UPRIGHT", "MLDB", "MLDB_UPRIGHT"};
    arg_akaze_descriptor_type->setValidator(std::make_shared<ValuesValidator<std::string>>(descriptor_type_options));
    auto arg_akaze_diffusivity = Argument::make<std::string>("diffusivity", "Diffusivity", "DIFF_PM_G2");
    std::vector<std::string> diffusivity{"DIFF_PM_G1", "DIFF_PM_G2", "DIFF_WEICKERT", "DIFF_CHARBONNIER"};
    arg_akaze_diffusivity->setValidator(std::make_shared<ValuesValidator<std::string>>(diffusivity));
    auto arg_kaze_octaves = Argument::make<int>("octaves", "Octaves", 4);
    auto arg_kaze_octave_layers = Argument::make<int>("octave_layers", "Octave layers", 4);
    auto arg_kaze_threshold = Argument::make<float>("threshold", "Threshold", 0.001f);

    auto cmd_akaze = Command::create("akaze", "AKAZE", {
            arg_input,
            arg_output,
            arg_format,
            arg_akaze_descriptor_type,
            Argument::make<int>("descriptor_size", "Descriptor size", 0),
            Argument::make<int>("descriptor_channels", "Descriptor channels", 3),
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
            Argument::make<bool>("extended", "Extended descriptor", false),
            Argument::make<bool>("upright", "Upright descriptor", false),
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
            Argument::make<int>("threshold", "Threshold", 30),
            Argument::make<int>("octaves", "Octaves", 3),
            Argument::make<float>("pattern_scale", "Pattern scale", 1.0)
        });

    /* ORB */

    auto arg_orb_score_type = Argument::make<std::string>("score_type", "Score Type", "Harris");
    //std::vector<std::string> score_type{"Harris", "FAST"};
    //arg_orb_score_type->setValidator(std::make_shared<ValuesValidator<std::string>>(score_type));
    arg_orb_score_type->setValidator(ValuesValidator<std::string>::create({"Harris", "FAST"}));

    auto cmd_orb = Command::create("orb", "Oriented FAST and rotated BRIEF (ORB)", {
            arg_input,
            arg_output,
            arg_format,
            arg_features_number,
            Argument::make<double>("scale_factor", "Scale Factor", 1.2),
            Argument::make<int>("levels_number", "Levels Number", 8),
            Argument::make<int>("edge_threshold", "Edge threshold", 31),
            Argument::make<int>("first_level", "First Level", 0),
            Argument::make<int>("wta_k", "WTA K", 2),
            arg_orb_score_type,
            Argument::make<int>("patch_size", "Patch Size", 31),
            Argument::make<int>("fast_threshold", "Fast Threshold", 20)
        });


    CommandList cmd("featextract", "Feature detection and descriptor extraction");
    cmd.addCommand(cmd_sift);
    cmd.addCommand(cmd_akaze);
    cmd.addCommand(cmd_kaze);
    cmd.addCommand(cmd_brisk);
    cmd.addCommand(cmd_orb);

    Command::Status status = cmd.parse(argc, argv);
    if(status == Command::Status::parse_error) {
        return 1;
    } else if(status == Command::Status::show_help) {
        return 0;
    } else if(status == Command::Status::show_licence) {
        return 0;
    } else if(status == Command::Status::show_version) {
        return 0;
    }

    Path input_path = arg_input->value();
    Path output_path = arg_output->value();
    output_path.createDirectories();
    output_path.append(input_path.fileName());
    std::string format = arg_format->value();
    if (format == "XML")
        output_path.replaceExtension(".xml");
    else if (format == "YML")
        output_path.replaceExtension(".yml");
    else if (format == "TXT")
        output_path.replaceExtension(".txt");
    else if (format == "BIN")
        output_path.replaceExtension(".bin");
    else
        TL_THROW_EXCEPTION("Unknow format: {}", format);

    std::shared_ptr<FeatureDetector> feature_detector;
    std::shared_ptr<FeatureDescriptor> feature_descriptor;

    std::string featextract_name = cmd.commandName();

    if(featextract_name == "sift") {

        SiftProperties properties;
        properties.setFeaturesNumber(cmd_sift->value<int>("features_number"));
        properties.setOctaveLayers(cmd_sift->value<int>("octave_layers"));
        properties.setContrastThreshold(cmd_sift->value<double>("contrast_threshold"));
        properties.setEdgeThreshold(cmd_sift->value<double>("edge_threshold"));
        properties.setSigma(cmd_sift->value<double>("sigma"));
        auto sift = std::make_shared<SiftDetectorDescriptor>(properties);
        feature_detector = std::dynamic_pointer_cast<FeatureDetector>(sift);
        feature_descriptor = std::dynamic_pointer_cast<FeatureDescriptor>(sift);

    } else if(featextract_name == "akaze") {
        
        AkazeProperties properties;
        properties.setDescriptorType(cmd_akaze->value<std::string>("descriptor_type"));
        properties.setDescriptorSize(cmd_akaze->value<int>("descriptor_size"));
        properties.setDescriptorChannels(cmd_akaze->value<int>("descriptor_channels"));
        properties.setThreshold(cmd_akaze->value<double>("threshold"));
        properties.setOctaves(cmd_akaze->value<int>("octaves"));
        properties.setOctaveLayers(cmd_akaze->value<int>("octave_layers"));
        properties.setDiffusivity(cmd_akaze->value<std::string>("diffusivity"));
        auto akaze = std::make_shared<AkazeDetectorDescriptor>(properties);

        feature_detector = std::dynamic_pointer_cast<FeatureDetector>(akaze);
        feature_descriptor = std::dynamic_pointer_cast<FeatureDescriptor>(akaze);

    } else if(featextract_name == "kaze") {
        
        KazeProperties properties;
        properties.setExtendedDescriptor(cmd_kaze->value<bool>("extended"));
        properties.setUprightDescriptor(cmd_kaze->value<bool>("upright"));
        properties.setThreshold(cmd_kaze->value<float>("threshold"));
        properties.setOctaves(cmd_kaze->value<int>("octaves"));
        properties.setOctaveLayers(cmd_kaze->value<int>("octave_layers"));
        properties.setDiffusivity(cmd_kaze->value<std::string>("diffusivity"));
        auto kaze = std::make_shared<KazeDetectorDescriptor>(properties);

        feature_detector = std::dynamic_pointer_cast<FeatureDetector>(kaze);
        feature_descriptor = std::dynamic_pointer_cast<FeatureDescriptor>(kaze);

    } else if(featextract_name == "brisk") {

        BriskProperties properties;
        properties.setThreshold(cmd_brisk->value<int>("threshold"));
        properties.setOctaves(cmd_brisk->value<int>("octaves"));
        properties.setPatternScale(cmd_brisk->value<float>("patternScale"));
        auto brisk = std::make_shared<BriskDetectorDescriptor>(properties);

        feature_detector = std::dynamic_pointer_cast<FeatureDetector>(brisk);
        feature_descriptor = std::dynamic_pointer_cast<FeatureDescriptor>(brisk);

    } else if(featextract_name == "orb") {

        OrbProperties properties;
        properties.setFeaturesNumber(4000);
        properties.setScaleFactor(1.5);
        properties.setLevelsNumber(6);
        properties.setEdgeThreshold(11);
        properties.setWTA_K(2);
        properties.setScoreType("FAST");
        properties.setPatchSize(11);
        properties.setFastThreshold(10);

        auto orb = std::make_shared<OrbDetectorDescriptor>(properties);

        feature_detector = std::dynamic_pointer_cast<FeatureDetector>(orb);
        feature_descriptor = std::dynamic_pointer_cast<FeatureDescriptor>(orb);
    }


    try {

        if(!input_path.exists()) TL_THROW_EXCEPTION("Image doesn't exist: {}", input_path.fileName().toString());

        cv::Mat img = cv::imread(input_path.toString(), cv::IMREAD_IGNORE_ORIENTATION | cv::IMREAD_GRAYSCALE);
        if(img.empty()) TL_THROW_EXCEPTION("Could not load image: {}", input_path.toString());

        Message::info("Searching Keypoints for image {}", input_path.toString());

        tl::Chrono chrono;
        chrono.run();

        std::vector<cv::KeyPoint> key_points = feature_detector->detect(img);

        double time = chrono.stop();
        Message::info("{} Keypoints detected in image {} [Time: {} seconds]", key_points.size(), input_path.fileName().toString(), time);

        Message::info("Computing keypoints descriptors for image {}", input_path.fileName().toString());

        chrono.reset();
        chrono.run();

        cv::Mat descriptors = feature_descriptor->extract(img, key_points);

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
