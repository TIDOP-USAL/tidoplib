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

#include <tidop/core/app.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/path.h>
#include <tidop/core/chrono.h>
#include <tidop/featmatch/bfmatch.h>
#include <tidop/featmatch/flann.h>
#include <tidop/featmatch/robustmatch.h>
#include <tidop/featmatch/matchio.h>
#include <tidop/featmatch/geomtest.h>
#include <tidop/featmatch/featio.h>

using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif

int main(int argc, char **argv)
{

    Console &console = App::console();
    console.setTitle("Feature Matching");
    console.setMessageLevel(MessageLevel::all);
    Message::instance().addMessageHandler(&console);

    auto arg_query = Argument::make<std::string>("query", 'q', "Features query");
    auto arg_train = Argument::make<std::string>("train", 't', "Features train");
    auto arg_output = Argument::make<std::string>("output", 'o', "Output path");
    auto arg_format = Argument::make<std::string>("format", 'f', "Format", "XML");
    std::vector<std::string> format_options{"XML", "YML", "BIN"};
    arg_format->setValidator(std::make_shared<ValuesValidator<std::string>>(format_options));
    auto arg_ratio = Argument::make<double>("ratio", "ratio", 0.8);
    auto arg_cross_match = Argument::make<bool>("cross_match", "Cross matching", true);
    auto arg_geometric_test = Argument::make<std::string>("geom_test", "Geometric test with Homography or Fundamental matrix", "F");
    std::vector<std::string> geom_test_options{"F", "H"};
    arg_geometric_test->setValidator(std::make_shared<ValuesValidator<std::string>>(geom_test_options));
    auto arg_ransac_distance = Argument::make<double>("RANSAC:distance", "RANSAC distance", 0.7);
    auto arg_ransac_confidence = Argument::make<double>("RANSAC:confidence", "RANSAC confidence", 0.999);
    auto arg_ransac_iterations = Argument::make<int>("RANSAC:iterations", "RANSAC iterations", 2000);


    /* Flann based matching */

    auto cmd_flann = Command::create("flann", "FLANN", {
            arg_query,
            arg_train,
            arg_output,
            arg_format,
            arg_ratio,
            arg_cross_match,
            arg_geometric_test,
            arg_ransac_distance,
            arg_ransac_confidence,
            arg_ransac_iterations
        });

    /* Brute-force matching */

    auto arg_norm = Argument::make<std::string>("norm", 'n', "Norm", "l2");
    std::vector<std::string> norm_options{"l1", "l2", "hamming", "hamming2"};
    arg_norm->setValidator(std::make_shared<ValuesValidator<std::string>>(norm_options));

    auto cmd_brute_force = Command::create("brute_force", "Brute-force matching", {
            arg_query,
            arg_train,
            arg_output,
            arg_format,
            arg_norm,
            arg_ratio,
            arg_cross_match,
            arg_geometric_test,
            arg_ransac_distance,
            arg_ransac_confidence,
            arg_ransac_iterations,
            Argument::make<bool>("cuda", "Enable cuda", false)
        });

    CommandList cmd("featmatch", "Feature matching");
    cmd.addCommand(cmd_flann);
    cmd.addCommand(cmd_brute_force);

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


    try {

        Path query_path = arg_query->value();
        Path train_path = arg_train->value();
        TL_ASSERT(query_path.exists(), "Query descriptors file not found {}", query_path.toString());
        TL_ASSERT(train_path.exists(), "Train descriptors file not found {}", train_path.toString());


        std::string matches_filename = query_path.baseName().toString();
        matches_filename.append("-");
        matches_filename.append(train_path.baseName().toString());
        std::string format = arg_format->value();
        if (format == "XML")
            matches_filename.append(".xml");
        else if (format == "YML")
            matches_filename.append(".yml");
        else if (format == "BIN")
            matches_filename.append(".bin");
        else
            TL_THROW_EXCEPTION("Unknow format: {}", format);
        Path output_path = arg_output->value();
        output_path.append(matches_filename);

        std::shared_ptr<DescriptorMatcher> descriptor_matcher;

        std::string command = cmd.commandName();

        if (command == "brute_force") {

            BruteForceMatcherImp::Norm norm = BruteForceMatcherProperties::Norm::l2;
            std::string norm_value = arg_norm->value();
            if (norm_value == "l1") {
                norm = BruteForceMatcherProperties::Norm::l1;
            } else if (norm_value == "l2") {
                norm = BruteForceMatcherProperties::Norm::l2;
            } else if (norm_value == "hamming") {
                norm = BruteForceMatcherProperties::Norm::hamming;
            } else if (norm_value == "hamming2") {
                norm = BruteForceMatcherProperties::Norm::hamming2;
            }

#ifdef HAVE_OPENCV_CUDAFEATURES2D
            if (cmd_brute_force->value<bool>("cuda") && norm != BruteForceMatcherProperties::Norm::hamming2) {
                descriptor_matcher = std::make_shared<BruteForceMatcherCuda>(norm);
            } else {
#endif // HAVE_OPENCV_CUDAFEATURES2D
                descriptor_matcher = std::make_shared<BruteForceMatcherImp>(norm);
#ifdef HAVE_OPENCV_CUDAFEATURES2D
            }
#endif // HAVE_OPENCV_CUDAFEATURES2D

        } else if (command == "flann") {

            FlannMatcherImp::Index index = FlannMatcherImp::Index::kdtree;
            descriptor_matcher = std::make_shared<FlannMatcherImp>(index);

        } else {
            TL_THROW_EXCEPTION("Unknow command: {}", command);
        }


        RobustMatchingImp robust_matching(descriptor_matcher);
        robust_matching.setRatio(arg_ratio->value());
        robust_matching.setCrossCheck(arg_cross_match->value());

        RANSACTestProperties ransac_properties;
        ransac_properties.distance = arg_ransac_distance->value();
        ransac_properties.confidence = arg_ransac_confidence->value();
        ransac_properties.iterations = arg_ransac_iterations->value();

        std::shared_ptr<tl::GeometricTest> geometric_test;
        std::string geometric_test_value = arg_geometric_test->value();
        if (geometric_test_value == "H") {

            geometric_test = tl::GeometricTestFactory::create(tl::GeometricTest::Type::homography_ransac, &ransac_properties);

        } else if (geometric_test_value == "F") {

            geometric_test = tl::GeometricTestFactory::create(tl::GeometricTest::Type::fundamental_ransac, &ransac_properties);

        }

        robust_matching.setGeometricTest(geometric_test);

        std::unique_ptr<FeaturesReader> features_reader = FeaturesReaderFactory::create(query_path);
        features_reader->read();
        std::vector<cv::KeyPoint> keypoints_query = features_reader->keyPoints();
        cv::Mat descriptors_query = features_reader->descriptors();
        
        features_reader = FeaturesReaderFactory::create(train_path);
        features_reader->read();
        std::vector<cv::KeyPoint> keypoints_train = features_reader->keyPoints();
        cv::Mat descriptors_train = features_reader->descriptors();

        std::vector<cv::DMatch> wrong_matches;
        std::vector<cv::DMatch> good_matches;

        robust_matching.compute(descriptors_query, descriptors_train, 
                                keypoints_query, keypoints_train, 
                                &good_matches, &wrong_matches);

        std::unique_ptr<MatchesWriter> writer = MatchesWriterFactory::create(output_path);
        writer->setGoodMatches(good_matches);
        writer->setWrongMatches(wrong_matches);
        writer->write();

        Message::success("Feature Matching finished");

    } catch(const std::exception &e) {
        printException(e);

    } catch(...) {

        Message::error("Feature matching unknow exception");

    }

    return 0;
}
