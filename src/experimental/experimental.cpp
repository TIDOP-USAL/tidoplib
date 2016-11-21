#include "experimental.h"

#include "core/messages.h"

#include "opencv2/highgui/highgui.hpp"
//#include "opencv2\sfm\libmv_light\libmv_capi.h"
#include "libmv\simple_pipeline\reconstruction.h"
#include "libmv\simple_pipeline\tracks.h"
#include "libmv\simple_pipeline\camera_intrinsics.h"
#include "libmv\correspondence\matches.h"
#include "libmv\multiview\robust_fundamental.h"

//Extraido de libmv_capi.h

struct libmv_Reconstruction {
  libmv::EuclideanReconstruction reconstruction;
  /* Used for per-track average error calculation after reconstruction */
  libmv::Tracks tracks;
  libmv::CameraIntrinsics *intrinsics;
  double error;
  bool is_valid;
};

namespace I3D
{

namespace EXPERIMENTAL
{

//// http://docs.opencv.org/3.1.0/dc/d2c/tutorial_real_time_pose.html

int RobustMatching::ratioTest(std::vector<std::vector<cv::DMatch> > &matches)
{
#ifdef _DEBUG
  double startTick, time;
  startTick = (double)cv::getTickCount(); // measure time
#endif
  int removed = 0;
  // for all matches
  for (std::vector<std::vector<cv::DMatch> >::iterator
       matchIterator = matches.begin(); matchIterator != matches.end(); ++matchIterator) {
    // if 2 NN has been identified
    if (matchIterator->size() > 1) {
      // check distance ratio
      if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > mRatio) {
        matchIterator->clear(); // remove match
        removed++;
      }
    } else { // does not have 2 neighbours
      matchIterator->clear(); // remove match
      removed++;
    }
  }
  return removed;
}

void RobustMatching::symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
                                  const std::vector<std::vector<cv::DMatch> >& matches2,
                                  std::vector<cv::DMatch>& symMatches)
{
#ifdef _DEBUG
  double startTick, time;
  startTick = (double)cv::getTickCount(); // measure time
#endif
  // for all matches image 1 -> image 2
  for (std::vector<std::vector<cv::DMatch> >::const_iterator
       matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1) {

    // ignore deleted matches
    if (matchIterator1->empty() || matchIterator1->size() < 2)
      continue;

    // for all matches image 2 -> image 1
    for (std::vector<std::vector<cv::DMatch> >::const_iterator
         matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2) {
      // ignore deleted matches
      if (matchIterator2->empty() || matchIterator2->size() < 2)
        continue;

      // Match symmetry test
      if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx && (*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx) {
        // add symmetrical match
        // ????
        symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx, (*matchIterator1)[0].trainIdx, (*matchIterator1)[0].distance));
        // No es lo mismo??:
        // symMatches.push_back(matchIterator1[0]);
        break; // next match in image 1 -> image 2
      }
    }
  }
#ifdef _DEBUG
  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
  printf("\nTime Reconstruction3D::symmetryTest [s]: %.3f\n", time);
#endif
}

void RobustMatching::symmetryTest(const std::vector<std::vector<cv::DMatch> > &matches, std::vector<std::vector<cv::DMatch>> *symMatches)
{
  if (symMatches && !matches.empty()) {
    // From putative matches get matches that fit the "Ratio" heuristic.
    int max_track_number = 0;
    for (size_t i = 0; i < matches.size(); i++) {
      float distance0 = matches[i][0].distance;
      float distance1 = matches[i][1].distance;
      // Add the match only if we have a symmetric result.
      if (distance0 < /*fRatio*/0.8f * distance1) {
        //... Creo que es lo mismo
        symMatches->push_back(matches[i]);
        //matches.Insert(0, max_track_number, &m_ViewData[images[i]].features[i]);
        //matches.Insert(1, max_track_number, &m_ViewData[images[j]].features[matchesA[i][0].trainIdx]);
        //++max_track_number;
      }
    }
  }
}


void RobustMatching::robustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<cv::DMatch>& goodMatches, std::vector<std::vector<cv::DMatch>> *pMatches12, std::vector<std::vector<cv::DMatch>> *pMatches21)
{

  // 2. Match the two image descriptors
  std::vector<std::vector<cv::DMatch> > matches12, matches21;

  // 2a. From image 1 to image 2
  mDescriptorMatcher->knnMatch(descriptor1, descriptor2, matches12, 2); // return 2 nearest neighbours
  if (pMatches21) *pMatches12 = matches12;

  // 2b. From image 2 to image 1
  mDescriptorMatcher->knnMatch(descriptor2, descriptor1, matches21, 2); // return 2 nearest neighbours
  if (pMatches21) *pMatches21 = matches21;

  //// 3. Remove matches for which NN ratio is > than threshold
  //// clean image 1 -> image 2 matches
  //ratioTest(matches12);
  //// clean image 2 -> image 1 matches
  //ratioTest(matches21);

  //// 4. Remove non-symmetrical matches
  //symmetryTest(matches12, matches21, goodMatches);

}

void RobustMatching::fastRobustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<std::vector<cv::DMatch>> *pMatches)
{
  //good_matches.clear();

  //// 1a. Detection of the ORB features
  //this->computeKeyPoints(frame, keypoints_frame);

  //// 1b. Extraction of the ORB descriptors
  //cv::Mat descriptors_frame;
  //this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

  // 2. Match the two image descriptors
  std::vector<std::vector<cv::DMatch> > matches;
  mDescriptorMatcher->knnMatch(descriptor1, descriptor2, matches, 2);
  if (pMatches) *pMatches = matches;
  //// 3. Remove matches for which NN ratio is > than threshold
  //ratioTest(matches);

  //// 4. Fill good matches container
  //for ( std::vector<std::vector<cv::DMatch> >::iterator
  //       matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  //{
  //  if (!matchIterator->empty()) good_matches.push_back((*matchIterator)[0]);
  //}

}

















/* ----------------------------------------------------------------------------------------- */

void Reconstruction3D::getKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths, bool bSave)
{
  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);
  for (int i = 0; i < size; i++) {
    cv::Mat image = cv::imread(mImagesPaths[i].c_str(), cv::IMREAD_GRAYSCALE);

    std::vector<cv::KeyPoint> keyPoint;
    cv::Mat descriptor;
    // Se detectan los key points y los descriptores
    int nft = mFeature2D->detectKeyPoints(image, &keyPoint);
    mFeature2D->calcDescriptor(image, NULL, &descriptor);

    mKeyPoints[i] = keyPoint;
    mDescriptor[i] = descriptor;

    if (bSave) {
      char out[I3D_MAX_PATH];
      changeFileExtension(mImagesPaths[i].c_str(), "xml", out);

      // Salvamos key points y los descriptores
      mFeature2D->save(out);
    }
  }
}

void Reconstruction3D::loadKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths)
{
  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);

  char out[I3D_MAX_PATH];

  for (int i = 0; i < size; i++) {
    changeFileExtension(mImagesPaths[i].c_str(), "xml", out);
    mFeature2D->read(out);
    mKeyPoints[i] = mFeature2D->getKeyPoints();
    mDescriptor[i] = mFeature2D->getDescriptors();
  }
}

//void Reconstruction3D::multiImageMatching(std::vector<std::string> &images_paths) 
//{
//
//}

void Reconstruction3D::multiImageMatching(std::vector<cv::Mat> &points2d)
{

  if (mDescriptor.empty()) return;

  std::vector<std::vector<int> > idx_pass_points;
  int size = static_cast<int>(mDescriptor.size());
  std::vector<std::vector<cv::Vec2d> > pass_points;
  std::vector<std::vector<cv::Vec2d> > passPointsImage(size);
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {

      //#ifdef _DEBUG
      //      cv::Mat image1 = cv::imread(mImagesPaths[i].c_str());
      //      cv::Mat image2 = cv::imread(mImagesPaths[j].c_str());
      //#endif
      //
      //mMatcher.match(mDescriptor[i], mDescriptor[j]);

      std::vector<cv::DMatch> goodMatches;
      ////mMatcher.getGoodMatches(&good_matches, 0.5);
      //mMatcher.getGoodMatches(mKeyPoints[i], mKeyPoints[j], &good_matches, 0.05);



      //// 1a. Detection of the ORB features
      //this->computeKeyPoints(frame, keypoints_frame);

      // 1b. Extraction of the ORB descriptors
      //cv::Mat descriptors_frame;
      //this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

      // 2. Match the two image descriptors
      std::vector<std::vector<cv::DMatch> > matches12, matches21;

      // 2a. From image 1 to image 2
      mMatcherFB.knnMatch(mDescriptor[i], mDescriptor[j], matches12, 2);
      //matcher_->knnMatch(descriptors_frame, descriptors_model, matches12, 2); // return 2 nearest neighbours

      // 2b. From image 2 to image 1
      //matcher_->knnMatch(descriptors_model, descriptors_frame, matches21, 2); // return 2 nearest neighbours
      mMatcherFB.knnMatch(mDescriptor[j], mDescriptor[i], matches21, 2);

      // 3. Remove matches for which NN ratio is > than threshold
      // clean image 1 -> image 2 matches
      //ratioTest2(matches12);
      //// clean image 2 -> image 1 matches
      //ratioTest2(matches21);

      // 4. Remove non-symmetrical matches
      //symmetryTest(matches12, matches21, goodMatches);


      //#ifdef _DEBUG
      //      // drawing the results
      //      cv::Mat img_matches, img1, img2;
      //      cv::drawMatches(image1, mKeyPoints[i], image2, mKeyPoints[j], good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
      //#endif
      //      char name1[I3D_MAX_PATH];
      //      char name2[I3D_MAX_PATH];
      //      char buf[I3D_MAX_PATH];
      //
      //      getFileName(mImagesPaths[i].c_str(), name1);
      //      getFileName(mImagesPaths[j].c_str(), name2);
      //
      //      //sprintf_s(buf, 500, "%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);
      //      //progress_bar.init(0, static_cast<double>(good_matches.size()), buf);
      //      printInfo("%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);
      //
      std::vector<cv::Point2f> pts1;
      std::vector<cv::Point2f> pts2;
      int idx1, idx2;
      for (size_t igm = 0; igm < goodMatches.size(); igm++) {
        idx1 = goodMatches[igm].queryIdx;
        idx2 = goodMatches[igm].trainIdx;

        //Busqueda de si ya esta añadido ese punto
        bool addNew = true;
        for (int k = 0; k < idx_pass_points.size(); k++) {
          if (idx_pass_points[k][i] == idx1) {
            addNew = false;
            idx_pass_points[k][j] = idx2;
            cv::Point2f point2 = mKeyPoints[j][idx2].pt;
            pass_points[k][j] = cv::Vec2d(point2.x, point2.y);
            break;
          } else if (idx_pass_points[k][j] == idx2) {
            addNew = false;
            idx_pass_points[k][i] = idx1;
            cv::Point2f point1 = mKeyPoints[i][idx1].pt;
            pass_points[k][i] = cv::Vec2d(point1.x, point1.y);
            break;
          }
        }
        if (addNew) {
          std::vector<int> v(size, -1);
          v[i] = idx1;
          v[j] = idx2;
          idx_pass_points.push_back(v);

          std::vector<cv::Vec2d> v_points(size, cv::Vec2d(-1, -1));
          cv::Point2f point1 = mKeyPoints[i][idx1].pt;
          cv::Point2f point2 = mKeyPoints[j][idx2].pt;
          v_points[i] = cv::Vec2d(point1.x, point1.y);
          v_points[j] = cv::Vec2d(point2.x, point2.y);
          pass_points.push_back(v_points);
        }
        //progress_bar();
      }
    }
  }

  saveTracks("D:\\Desarrollo\\datos\\Videos\\Torres_Pasillo_illescas\\tracks.txt", pass_points);
  int nPassPoint = static_cast<int>(pass_points.size());

  printInfo("Número de puntos de paso encontrados: %i", nPassPoint);

  // Formato necesario para sfm
  for (int i = 0; i < size; ++i) {
    cv::Mat_<double> frame(2, nPassPoint);
    for (int j = 0; j < nPassPoint; ++j) {
      frame(0, j) = pass_points[j][i][0];
      frame(1, j) = pass_points[j][i][1];
    }
    points2d.push_back(cv::Mat(frame));
  }
}


//int Reconstruction3D::ratioTest2(std::vector<std::vector<cv::DMatch> > &matches, float ratio)
//{
//  int removed = 0;
//  std::vector<cv::DMatch> goodMatches;
//  std::vector<cv::Point2f> _pts1;
//  std::vector<cv::Point2f> _pts2;
//  for (size_t iMatch = 0; iMatch < matches.size(); ++iMatch) {
//    if (matches[iMatch].size() > 1) {
//      float distance0 = matches[iMatch][0].distance;
//      float distance1 = matches[iMatch][1].distance;
//      if (distance0 > ratio * distance1) {
//        matches[iMatch].clear();
//        removed++;
//      }
//    } else {
//      matches[iMatch].clear();
//      removed++;
//    }
//  }
//  //... Casi dos veces mas lento
//  /*
//  int removed = 0;
//  std::vector<cv::DMatch> goodMatches;
//  std::vector<cv::Point2f> _pts1;
//  std::vector<cv::Point2f> _pts2;
//  for ( auto &matchIterator : matches) {
//  if (matchIterator.size() > 1) {
//  float distance0 = matchIterator[0].distance;
//  float distance1 = matchIterator[1].distance;
//  if (distance0 > ratio * distance1) {
//  matchIterator.clear();
//  removed++;
//  }
//  } else {
//  matchIterator.clear();
//  removed++;
//  }
//  }
//  */
//  //... 3 veces mas lento
//  /*
//  int removed = 0;
//  std::vector<cv::DMatch> goodMatches;
//  std::vector<cv::Point2f> _pts1;
//  std::vector<cv::Point2f> _pts2;
//  for ( std::vector<std::vector<cv::DMatch> >::iterator
//  matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator) {
//  if (matchIterator->size() > 1) {
//  float distance0 = (*matchIterator)[0].distance;
//  float distance1 = (*matchIterator)[1].distance ;
//  if (distance0 > ratio * distance1) {
//  matchIterator->clear();
//  removed++;
//  }
//  } else {
//  matchIterator->clear();
//  removed++;
//  }
//  }
//  */
//  return removed;
//}
//
//int Reconstruction3D::ratioTest(std::vector<std::vector<cv::DMatch> > &matches, float ratio)
//{
//#ifdef _DEBUG
//  double startTick, time;
//  startTick = (double)cv::getTickCount(); // measure time
//#endif
//  int removed = 0;
//  // for all matches
//  for (std::vector<std::vector<cv::DMatch> >::iterator
//       matchIterator = matches.begin(); matchIterator != matches.end(); ++matchIterator) {
//    // if 2 NN has been identified
//    if (matchIterator->size() > 1) {
//      // check distance ratio
//      if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio) {
//        matchIterator->clear(); // remove match
//        removed++;
//      }
//    } else { // does not have 2 neighbours
//      matchIterator->clear(); // remove match
//      removed++;
//    }
//  }
//#ifdef _DEBUG
//  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
//  printf("\nTime Reconstruction3D::ratioTest2 [s]: %.3f\n", time);
//#endif
//  return removed;
//}


//void Reconstruction3D::symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches1,
//                     const std::vector<std::vector<cv::DMatch> >& matches2,
//                     std::vector<cv::DMatch>& symMatches )
//{
//
//#ifdef _DEBUG
//  double startTick, time;
//  startTick = (double)cv::getTickCount(); // measure time
//#endif
//  // for all matches image 1 -> image 2
//   for (std::vector<std::vector<cv::DMatch> >::const_iterator
//       matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
//   {
//
//      // ignore deleted matches
//      if (matchIterator1->empty() || matchIterator1->size() < 2)
//         continue;
//
//      // for all matches image 2 -> image 1
//      for (std::vector<std::vector<cv::DMatch> >::const_iterator
//          matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
//      {
//        // ignore deleted matches
//        if (matchIterator2->empty() || matchIterator2->size() < 2)
//           continue;
//
//        // Match symmetry test
//        if ((*matchIterator1)[0].queryIdx ==
//            (*matchIterator2)[0].trainIdx &&
//            (*matchIterator2)[0].queryIdx ==
//            (*matchIterator1)[0].trainIdx)
//        {
//            // add symmetrical match
//            symMatches.push_back(
//              cv::DMatch((*matchIterator1)[0].queryIdx,
//                         (*matchIterator1)[0].trainIdx,
//                         (*matchIterator1)[0].distance));
//            break; // next match in image 1 -> image 2
//        }
//      }
//   }
//#ifdef _DEBUG
//  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
//  printf("\nTime Reconstruction3D::symmetryTest [s]: %.3f\n", time);
//#endif
//}

void Reconstruction3D::saveTracks(const std::string &filename, std::vector<std::vector<cv::Vec2d> > passPoints)
{
  std::ofstream hWrite(filename.c_str());


  if (!hWrite.is_open()) {
    consolePrintInfo("Unable to write file: %s", filename.c_str());
  } else {

    int nPassPoint = static_cast<int>(passPoints.size());
    int nCameras = static_cast<int>(passPoints[0].size());
    for (int j = 0; j < nPassPoint; ++j) {
      for (int i = 0; i < nCameras; ++i) {
        hWrite << passPoints[j][i][0] << " " << passPoints[j][i][1] << " ";
      }
      hWrite << "\n";
    }
    hWrite.close();
  }
}

void Reconstruction3D::init(bool refinement)
{
  bRefinement = refinement;
}

void Reconstruction3D::reconstruct(std::vector<std::string> &images, std::vector<cv::Mat> &Rs_est, std::vector<cv::Mat> &ts_est, cv::Matx33d &K, std::vector<cv::Mat> &points3d_estimated)
{
  if (images.size() >= 2) {

    // Se determinan los key points y los descriptores 
    getKeyPointAndDescriptor(images, true);
    // Con esto los tengo en mKeyPoints y mDescriptor
    size_t size = images.size();

    // Initial reconstruction
    const int keyframe1 = 1, keyframe2 = 2;
    const int select_keyframes = 1; // enable automatic keyframes selection
    const int verbosity_level = -1; // mute libmv logs

    // Refinement parameters
    const int refine_intrinsics = (!bRefinement) ? 0 :
      cv::sfm::SFM_REFINE_FOCAL_LENGTH | cv::sfm::SFM_REFINE_PRINCIPAL_POINT | cv::sfm::SFM_REFINE_RADIAL_DISTORTION_K1 | cv::sfm::SFM_REFINE_RADIAL_DISTORTION_K2;

    // Camera data
    cv::Matx33d Ka = K; // K.getMat();
    const double focal_length = Ka(0, 0);
    const double principal_x = Ka(0, 2), principal_y = Ka(1, 2), k1 = 0, k2 = 0, k3 = 0;

    // Set reconstruction options
    cv::sfm::libmv_ReconstructionOptions reconstruction_options(keyframe1, keyframe2, refine_intrinsics, select_keyframes, verbosity_level);

    cv::sfm::libmv_CameraIntrinsicsOptions camera_instrinsic_options =
      cv::sfm::libmv_CameraIntrinsicsOptions(cv::sfm::SFM_DISTORTION_MODEL_POLYNOMIAL,
      focal_length, principal_x, principal_y,
      k1, k2, k3);

    //-- Instantiate reconstruction pipeline
    reconstruction = cv::sfm::SFMLibmvEuclideanReconstruction::create(camera_instrinsic_options, reconstruction_options);

    //para hacer el matching
    //reconstruction->run(images);
    //{
    //libmv_reconstruction_ =
    //*libmv_solveReconstructionImpl(images,
    //                               &libmv_camera_intrinsics_options_,
    //                               &libmv_reconstruction_options_);
    libmv_Reconstruction libmv_reconstruction_;
    //{
      //cv::Ptr<cv::Feature2D> edetector = cv::ORB::create(10000);
      //cv::Ptr<cv::Feature2D> edescriber = cv::xfeatures2d::DAISY::create();

      //std::cout << "Initialize nViewMatcher ... ";
      // Esta clase deberia ser sencilla de implementar en caso de que necesite limpiar esta
      //libmv::correspondence::nRobustViewMatching nViewMatcher(edetector, edescriber);

      //mRobustMatching = std::make_unique<RobustMatching>(edetector, edescriber, cv::DescriptorMatcher::create("FlannBased"));

      //std::cout << "OK" << std::endl << "Performing Cross Matching ... ";
      //nViewMatcher.computeCrossMatch(images);
      //{
        //if (/*nViewMatcher.m_pDetector*/edetector == NULL || /*nViewMatcher.m_pDescriber*/edescriber == NULL)  {
        //  //LOG(FATAL) << "Invalid Detector or Describer.";
        //  //return false;
        //}

        // detección de features
        //nViewMatcher.m_vec_InputNames = images;
        //bool bRes = true;
        //for (int i=0; i < images.size(); ++i) {
        //bRes &= computeData(images[i]);
        //{
        //  cv::Mat im_cv = cv::imread(images[i], 0);
        //  if (im_cv.empty()) {
        //    //LOG(FATAL) << "Failed loading image: " << filename;
        //    //return false;
        //  } else {
        //    libmv::vector<libmv::Feature *> features;
        //    std::vector<cv::KeyPoint> features_cv;
        //    edetector->detect( im_cv, features_cv );
        //    features.resize(features_cv.size());
        //    for(size_t i=0; i<features_cv.size(); ++i)
        //      features[i] = new libmv::PointFeature(features_cv[i]);

        //    cv::Mat descriptors;
        //    edescriber->compute(im_cv, features_cv, descriptors);

        //    // Copy data.
        //    nViewMatcher.m_ViewData.insert( std::make_pair(images[i],FeatureSet()) );
        //    FeatureSet & KeypointData = nViewMatcher.m_ViewData[images[i]];
        //    KeypointData.features.resize(descriptors.rows);
        //    for(int i = 0;i < descriptors.rows; ++i)
        //    {
        //      KeypointFeature & feat = KeypointData.features[i];
        //      descriptors.row(i).copyTo(feat.descriptor);
        //      *(PointFeature*)(&feat) = *(PointFeature*)features[i];
        //    }

        //    DeleteElements(&features);

        //    return true;
        //  }
        //}
        //}

    std::vector<std::vector<int> > idx_pass_points;
    std::vector<std::vector<cv::Vec2d> > pass_points;
    // matching cruzado
    bool bRes2 = true;
    for (int i = 0; i < images.size(); ++i) {
      for (int j = 0; j < i; ++j) {
        std::vector<std::vector<cv::DMatch>> robus_matches;
        mRobustMatching->fastRobustMatch(mDescriptor[i], mDescriptor[j], &robus_matches);

        //if (nViewMatcher.m_ViewData.find(images[i]) != nViewMatcher.m_ViewData.end() &&
        //  m_ViewData.find(images[j]) != m_ViewData.end())
        //{
        //bRes2 &= nViewMatcher.MatchData( images[i], images[j]);
        //{
        // Check input data
        //if ( std::find(m_vec_InputNames.begin(), m_vec_InputNames.end(), images[i])
        //        == m_vec_InputNames.end() ||
        //       std::find(m_vec_InputNames.begin(), m_vec_InputNames.end(), images[j])
        //        == m_vec_InputNames.end())
        //{
        //  //LOG(INFO) << "[nViewMatching::MatchData] "
        //  //          << "Could not identify one of the input name.";
        //  //return false;
        //}
        //if (m_ViewData.find(images[i]) == m_ViewData.end() ||
        //    m_ViewData.find(images[j]) == m_ViewData.end())
        //{
        //  //LOG(INFO) << "[nViewMatching::MatchData] "
        //  //          << "Could not identify data for one of the input name.";
        //  //return false;
        //}

        //// Computed data exist for the given name
        //int iDataA = find(m_vec_InputNames.begin(), m_vec_InputNames.end(), images[i])
        //              - m_vec_InputNames.begin();
        //int iDataB = find(m_vec_InputNames.begin(), m_vec_InputNames.end(), images[j])
        //              - m_vec_InputNames.begin();

        //Matches matches;
        //TODO(pmoulon) make FindCandidatesMatches a parameter.
        //FindCandidateMatches_Ratio(m_ViewData[ images[i]],
        //                     m_ViewData[images[j]],
        //                     &matches);
        //{
        //  if (m_ViewData[ images[i]].features.empty() || m_ViewData[images[j]].features.empty())
        //    return;

        //  cv::FlannBasedMatcher matcherA;

        //  // Paste the necessary data in contiguous arrays.
        //  cv::Mat arrayA = FeatureSet::FeatureSetDescriptorsToContiguousArray(m_ViewData[images[i]]);
        //  cv::Mat arrayB = FeatureSet::FeatureSetDescriptorsToContiguousArray(m_ViewData[images[j]]);

        //  matcherA.add(std::vector<cv::Mat>(1, arrayB));
        //  std::vector < std::vector<cv::DMatch> > matchesA;
        //  matcherA.knnMatch(arrayA, matchesA, 2);
        std::vector<std::vector<cv::DMatch>> symMatches;
        mRobustMatching->symmetryTest(robus_matches, &symMatches);
        //libmv::Matches matches;
        //for (size_t im = 0; im < symMatches.size(); im++) {
        //  matches.Insert(0, im, &m_ViewData[images[i]].features[i]);
        //  matches.Insert(1, im, &m_ViewData[images[j]].features[matchesA[i][0].trainIdx]);

        //}
        //  // From putative matches get matches that fit the "Ratio" heuristic.
        //  int max_track_number = 0;
        //  for (size_t i = 0; i < matchesA.size(); ++i)
        //  {
        //    float distance0 = matchesA[i][0].distance;
        //    float distance1 = matchesA[i][1].distance;
        //    // Add the match only if we have a symmetric result.
        //    if (distance0 < /*fRatio*/0.8f * distance1)
        //    {
        //      {
        //        matches.Insert(0, max_track_number, &m_ViewData[images[i]].features[i]);
        //        matches.Insert(1, max_track_number, &m_ViewData[images[j]].features[matchesA[i][0].trainIdx]);
        //        ++max_track_number;
        //      }
        //    }
        //  }
        //}

        //Matches *consistent_matches;
        //bool bret;
        //bool nRobustViewMatching::computeConstrainMatches(const Matches & matchIn,
        //           int dataAindex,
        //           int dataBindex,
        //           Matches * matchesOut)
        //{
        //if (consistent_matches == NULL) {
        //  //LOG(INFO) << "[nViewMatching::computeConstrainMatches]"
        //  //          << " Could not export constrained matches.";
        //  //return false;
        //}
        std::vector<Eigen::MatrixXd> x;
        libmv::vector<int> tracks/*, images*/;
        //images.push_back(0);
        //images.push_back(1);

        //PointMatchMatrices(matches, images, &tracks, &x);
        //{
        ///////////////////////////////////////////////////////////////////
        // No se que coño está haciendo aqui... tracks es una lista de 0 a n de matches
        // Images es un vector que contiene 1 y 2 ...
        //TracksInAllImages(matches, images, &tracks);
        ///////////////////////////////////////////////////////////////////
        //x es una matriz que contiene las coordenadas
        x.resize(2/*images.size()*/);

        //for (int i = 0; i < 2; ++i) {
        x[0].resize(2, symMatches.size()/*tracks.size()*/);
        x[1].resize(2, symMatches.size()/*tracks.size()*/);

        int idx1, idx2;
        for (int k = 0; k < symMatches.size()/*tracks.size()*/; ++k) {
          //const PointFeature *f = static_cast<const PointFeature *>(matches.Get(images[i], tracks[j]));
          //x[i](0, j) = f->x();
          //x[i](1, j) = f->y();
          //.... Ver si es lo mismo
          idx1 = symMatches[k][0].queryIdx;
          idx2 = symMatches[k][0].trainIdx;
          cv::Point2f point1 = mKeyPoints[i][idx1].pt;
          cv::Point2f point2 = mKeyPoints[j][idx2].pt;
          x[0](0, k) = point1.x;
          x[0](1, k) = point1.y;
          x[1](0, k) = point2.x;
          x[1](1, k) = point2.y;
        }
        libmv::vector<int> inliers;
        Eigen::Matrix<double, 3, 3> H;
        double max_error = 0.1;
        // TODO(pmoulon) Make the Correspondence filter a parameter.
        //HomographyFromCorrespondences2PointRobust(x[0], x[1], 0.3, &H, &inliers);
        //HomographyFromCorrespondences4PointRobust(x[0], x[1], 0.3, &H, &inliers);
        //AffineFromCorrespondences2PointRobust(x[0], x[1], 1, &H, &inliers);
        libmv::FundamentalFromCorrespondences7PointRobust(x[0], x[1], 1.0, &H, &inliers);

        std::vector<cv::Point2f> pts1;
        std::vector<cv::Point2f> pts2;
        for (size_t igm = 0; igm < inliers.size(); igm++) {
          idx1 = symMatches[inliers[igm]][0].queryIdx;
          idx2 = symMatches[inliers[igm]][0].trainIdx;

          //Busqueda de si ya esta añadido ese punto
          bool addNew = true;
          for (int k = 0; k < idx_pass_points.size(); k++) {
            if (idx_pass_points[k][i] == idx1) {
              addNew = false;
              idx_pass_points[k][j] = idx2;
              cv::Point2f point2 = mKeyPoints[j][idx2].pt;
              if ( pass_points[k][j] == cv::Vec2d(-1, -1))
                pass_points[k][j] = cv::Vec2d(point2.x, point2.y);
              break;
            } else if (idx_pass_points[k][j] == idx2) {
              addNew = false;
              idx_pass_points[k][i] = idx1;
              cv::Point2f point1 = mKeyPoints[i][idx1].pt;
              if (pass_points[k][i] == cv::Vec2d(-1, -1))
                pass_points[k][i] = cv::Vec2d(point1.x, point1.y);
              break;
            }
          }
          if (addNew) {
            std::vector<int> v(size, -1);
            v[i] = idx1;
            v[j] = idx2;
            idx_pass_points.push_back(v);

            std::vector<cv::Vec2d> v_points(size, cv::Vec2d(-1, -1));
            cv::Point2f point1 = mKeyPoints[i][idx1].pt;
            cv::Point2f point2 = mKeyPoints[j][idx2].pt;
            v_points[i] = cv::Vec2d(point1.x, point1.y);
            v_points[j] = cv::Vec2d(point2.x, point2.y);
            pass_points.push_back(v_points);
          }
              //progress_bar();
        }
        //TODO(pmoulon) insert an optimization phase.
        // Rerun Robust correspondance on the inliers.
        // it will allow to compute a better model and filter ugly fitting.

        //-- Assert that the output of the model is consistent :
        // As much as the minimal points are inliers.
        //if (inliers.size() > 7 * 2) { //2* [nbPoints required by the estimator]
        //  // If tracks table is empty initialize it
        //  if (m_featureToTrackTable.size() == 0)  {
        //    // Build new correspondence graph containing only inliers.
        //    for (int l = 0; l < inliers.size(); ++l)  {
        //      const int k = inliers[l];
        //      m_featureToTrackTable[matches.Get(0, tracks[k])] = l;
        //      m_featureToTrackTable[matches.Get(1, tracks[k])] = l;
        //      m_tracks.Insert(iDataA, l,
        //          matches.Get(iDataB, tracks[k]));
        //      m_tracks.Insert(iDataB, l,
        //          matches.Get(iDataA, tracks[k]));
        //    }
        //  }
        //  else  {
        //    // Else update the tracks
        //    for (int l = 0; l < inliers.size(); ++l)  {
        //      const int k = inliers[l];
        //      std::map<const Feature*, int>::const_iterator iter =
        //        m_featureToTrackTable.find(matches.Get(1, tracks[k]));

        //      if (iter!=m_featureToTrackTable.end())  {
        //        // Add a feature to the existing track
        //        const int trackIndex = iter->second;
        //        m_featureToTrackTable[matches.Get(0, tracks[k])] = trackIndex;
        //        m_tracks.Insert(iDataA, trackIndex,
        //          matches.Get(0, tracks[k]));
        //      }
        //      else  {
        //        // It's a new track
        //        const int trackIndex = m_tracks.NumTracks();
        //        m_featureToTrackTable[matches.Get(0, tracks[k])] = trackIndex;
        //        m_featureToTrackTable[matches.Get(1, tracks[k])] = trackIndex;
        //        m_tracks.Insert(iDataA, trackIndex,
        //            matches.Get(0, tracks[k]));
        //        m_tracks.Insert(iDataB, trackIndex,
        //            matches.Get(1, tracks[k]));
        //      }
        //    }
        //  }


        // Export common feature between the two view
        //if (consistent_matches) {
        //  //Matches & consistent_matches = *matchesOut;
        //  // Build new correspondence graph containing only inliers.
        //  for (int l = 0; l < inliers.size(); ++l) {
        //    int k = inliers[l];
        //    for (int i = 0; i < 2; ++i) {
        //      consistent_matches->Insert(images[i], tracks[k],
        //          matches.Get(images[i], tracks[k]));
        //    }
        //  }
        //}
      }
          //bret = true;
    }

    for (int i = 0; i < size; ++i) {
      cv::Mat_<double> frame(2, pass_points.size());
      for (int j = 0; j < pass_points.size(); ++j)
      {
        frame(0,j) = pass_points[j][i][0];
        frame(1,j) = pass_points[j][i][1];
      }
      points3d_estimated.push_back(cv::Mat(frame));
    }

    saveTracks("D:\\Desarrollo\\datos\\Videos\\Torres_Pasillo_illescas\\tracks.txt", pass_points);
    
    int nPassPoint = static_cast<int>(pass_points.size());

    printInfo("Número de puntos de paso encontrados: %i", nPassPoint);

        //if (bret /*computeConstrainMatches(matches,iDataA,iDataB,&consistent_matches)*/)
        //{
        //  matches = *consistent_matches;
        //}
        //if (matches.NumTracks() > 0)
        //{
        //  m_sharedData.insert(
        //    make_pair(
        //      make_pair(m_vec_InputNames[iDataA],m_vec_InputNames[iDataB]),
        //      matches)
        //    );
        //}

        //return true;

      // Building tracks
  //libmv::Tracks tracks;
  //libmv::Matches matches = nViewMatcher.getMatches();

  //std::set<Matches::ImageID>::const_iterator iter_image =
  //matches.get_images().begin();

  //bool is_first_time = true;

    //for (; iter_image != matches.get_images().end(); ++iter_image) {
    //  // Exports points
    //  Matches::Features<PointFeature> pfeatures =
    //    matches.InImage<PointFeature>(*iter_image);

    //  while (pfeatures) {

    //    double x = pfeatures.feature()->x(),
    //      y = pfeatures.feature()->y();

    //    // valid marker
    //    if (x > 0 && y > 0) {
    //      tracks.Insert(*iter_image, pfeatures.track(), x, y);

    //      if (is_first_time)
    //        is_first_time = false;
    //    }

    //    // lost track
    //    else if (x < 0 && y < 0) {
    //      is_first_time = true;
    //    }

    //    pfeatures.operator++();
    //  }
    //}
  }

  //return bRes2;
}

//std::cout << "OK" << std::endl;


//parser_2D_tracks( matches, tracks );
//{

// Perform reconstruction
//return libmv_solveReconstruction(tracks,
//                           libmv_camera_intrinsics_options,
//                           libmv_reconstruction_options);
//libmv_reconstruction_ = cv::sfm::libmv_solveReconstruction(tracks,
//                           libmv_camera_intrinsics_options,
//                           libmv_reconstruction_options);





/*
Metric

Kilometre1
Metre1000
Decimetre10000
Centimetre100000
Millimetre1000000
Micrometre1000000000
Nanometre1E12
Angstrom1E13

British/American

League 0,21
Mile 0,62 
Furlong 4,97
Chain 49,71
Rod 198,84
Yard 1093,61
Foot 3280,84
Link 4970,97
Hand 9842,52
Inch 39370,08
Line 393700,79
Mil 39370078,74
Thou 39370078,74

Nautical

Sea mile 0,54
Fathom 546,81

*/







} // End namespace EXPERIMENTAL

} // End namespace I3D


  ///*!
  // * \brief keyPoints
  // */
  //std::vector<std::vector<cv::KeyPoint>> mKeyPoints;

  ///*!
  // * \brief Descriptores
  // */
  //std::vector<cv::Mat> mDescriptor;
