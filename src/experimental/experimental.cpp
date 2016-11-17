#include "experimental.h"

#include "core/messages.h"

#include "opencv2/highgui/highgui.hpp"

namespace I3D
{

namespace EXPERIMENTAL
{

//// http://docs.opencv.org/3.1.0/dc/d2c/tutorial_real_time_pose.html

RobustMatcher::~RobustMatcher()
{
  // TODO Auto-generated destructor stub
}

void RobustMatcher::computeKeyPoints( const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints)
{
  detector_->detect(image, keypoints);
}

void RobustMatcher::computeDescriptors( const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors)
{
  extractor_->compute(image, keypoints, descriptors);
}

int RobustMatcher::ratioTest(std::vector<std::vector<cv::DMatch> > &matches)
{
  int removed = 0;
  // for all matches
  for ( std::vector<std::vector<cv::DMatch> >::iterator
        matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  {
    // if 2 NN has been identified
    if (matchIterator->size() > 1)
    {
      // check distance ratio
      if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio_)
      {
        matchIterator->clear(); // remove match
        removed++;
      }
    }
    else
    { // does not have 2 neighbours
      matchIterator->clear(); // remove match
      removed++;
    }
  }
  return removed;
}

void RobustMatcher::symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches1,
                     const std::vector<std::vector<cv::DMatch> >& matches2,
                     std::vector<cv::DMatch>& symMatches )
{

  // for all matches image 1 -> image 2
   for (std::vector<std::vector<cv::DMatch> >::const_iterator
       matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
   {

      // ignore deleted matches
      if (matchIterator1->empty() || matchIterator1->size() < 2)
         continue;

      // for all matches image 2 -> image 1
      for (std::vector<std::vector<cv::DMatch> >::const_iterator
          matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
      {
        // ignore deleted matches
        if (matchIterator2->empty() || matchIterator2->size() < 2)
           continue;

        // Match symmetry test
        if ((*matchIterator1)[0].queryIdx ==
            (*matchIterator2)[0].trainIdx &&
            (*matchIterator2)[0].queryIdx ==
            (*matchIterator1)[0].trainIdx)
        {
            // add symmetrical match
            symMatches.push_back(
              cv::DMatch((*matchIterator1)[0].queryIdx,
                         (*matchIterator1)[0].trainIdx,
                         (*matchIterator1)[0].distance));
            break; // next match in image 1 -> image 2
        }
      }
   }

}

void RobustMatcher::robustMatch( const cv::Mat& frame, std::vector<cv::DMatch>& good_matches,
              std::vector<cv::KeyPoint>& keypoints_frame, const cv::Mat& descriptors_model )
{

  // 1a. Detection of the ORB features
  this->computeKeyPoints(frame, keypoints_frame);

  // 1b. Extraction of the ORB descriptors
  cv::Mat descriptors_frame;
  this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

  // 2. Match the two image descriptors
  std::vector<std::vector<cv::DMatch> > matches12, matches21;

  // 2a. From image 1 to image 2
  matcher_->knnMatch(descriptors_frame, descriptors_model, matches12, 2); // return 2 nearest neighbours

  // 2b. From image 2 to image 1
  matcher_->knnMatch(descriptors_model, descriptors_frame, matches21, 2); // return 2 nearest neighbours

  // 3. Remove matches for which NN ratio is > than threshold
  // clean image 1 -> image 2 matches
  ratioTest(matches12);
  // clean image 2 -> image 1 matches
  ratioTest(matches21);

  // 4. Remove non-symmetrical matches
  symmetryTest(matches12, matches21, good_matches);

}

void RobustMatcher::fastRobustMatch( const cv::Mat& frame, std::vector<cv::DMatch>& good_matches,
                                 std::vector<cv::KeyPoint>& keypoints_frame,
                                 const cv::Mat& descriptors_model )
{
  good_matches.clear();

  // 1a. Detection of the ORB features
  this->computeKeyPoints(frame, keypoints_frame);

  // 1b. Extraction of the ORB descriptors
  cv::Mat descriptors_frame;
  this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

  // 2. Match the two image descriptors
  std::vector<std::vector<cv::DMatch> > matches;
  matcher_->knnMatch(descriptors_frame, descriptors_model, matches, 2);

  // 3. Remove matches for which NN ratio is > than threshold
  ratioTest(matches);

  // 4. Fill good matches container
  for ( std::vector<std::vector<cv::DMatch> >::iterator
         matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  {
    if (!matchIterator->empty()) good_matches.push_back((*matchIterator)[0]);
  }

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
      char out[_MAX_PATH];
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
  
void Reconstruction3D::multiImageMatching( std::vector<cv::Mat> &points2d) 
{

  if (mDescriptor.empty()) return;

  std::vector<std::vector<int> > idx_pass_points;
  int size = static_cast<int>(mDescriptor.size());
  std::vector<std::vector<cv::Vec2d> > pass_points;
  std::vector<std::vector<cv::Vec2d> > passPointsImage(size);
  for (int i = 0; i < size - 1; i++) {
    for (int j = i+1; j < size; j++) {

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
      ratioTest2(matches12);
      // clean image 2 -> image 1 matches
      ratioTest2(matches21);

      // 4. Remove non-symmetrical matches
      symmetryTest(matches12, matches21, goodMatches);


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
          if (idx_pass_points[k][i] == idx1 ) {
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
    for (int j = 0; j < nPassPoint; ++j)
    {
      frame(0,j) = pass_points[j][i][0];
      frame(1,j) = pass_points[j][i][1];
    }
    points2d.push_back(cv::Mat(frame));
  }
}

int Reconstruction3D::ratioTest2(std::vector<std::vector<cv::DMatch> > &matches, float ratio)
{
  int removed = 0;
  std::vector<cv::DMatch> goodMatches;
  std::vector<cv::Point2f> _pts1;
  std::vector<cv::Point2f> _pts2;
  for (size_t iMatch = 0; iMatch < matches.size(); ++iMatch) {
    if (matches[iMatch].size() > 1) {
      float distance0 = matches[iMatch][0].distance;
      float distance1 = matches[iMatch][1].distance;
      if (distance0 > ratio * distance1) {
        matches[iMatch].clear();
        removed++;
      }
    } else {
      matches[iMatch].clear();
      removed++;
    }
  }
  //... Casi dos veces mas lento
  /*
  int removed = 0;
  std::vector<cv::DMatch> goodMatches;
  std::vector<cv::Point2f> _pts1;
  std::vector<cv::Point2f> _pts2;
  for ( auto &matchIterator : matches) {
    if (matchIterator.size() > 1) {
      float distance0 = matchIterator[0].distance;
      float distance1 = matchIterator[1].distance;
      if (distance0 > ratio * distance1) {
        matchIterator.clear();
        removed++;
      }
    } else {
      matchIterator.clear();
      removed++;
    }
  }
  */
  //... 3 veces mas lento
  /*
  int removed = 0;
  std::vector<cv::DMatch> goodMatches;
  std::vector<cv::Point2f> _pts1;
  std::vector<cv::Point2f> _pts2;
  for ( std::vector<std::vector<cv::DMatch> >::iterator
        matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator) {
    if (matchIterator->size() > 1) {
      float distance0 = (*matchIterator)[0].distance;
      float distance1 = (*matchIterator)[1].distance ;
      if (distance0 > ratio * distance1) {
        matchIterator->clear();
        removed++;
      }
    } else {
      matchIterator->clear();
      removed++;
    }
  }
  */
  return removed;
}

int Reconstruction3D::ratioTest(std::vector<std::vector<cv::DMatch> > &matches, float ratio )
{
#ifdef _DEBUG
  double startTick, time;
  startTick = (double)cv::getTickCount(); // measure time
#endif
  int removed = 0;
  // for all matches
  for ( std::vector<std::vector<cv::DMatch> >::iterator
        matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  {
    // if 2 NN has been identified
    if (matchIterator->size() > 1)
    {
      // check distance ratio
      if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio)
      {
        matchIterator->clear(); // remove match
        removed++;
      }
    }
    else
    { // does not have 2 neighbours
      matchIterator->clear(); // remove match
      removed++;
    }
  }
#ifdef _DEBUG
  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
  printf("\nTime Reconstruction3D::ratioTest2 [s]: %.3f\n", time);
#endif
  return removed;
}


void Reconstruction3D::symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches1,
                     const std::vector<std::vector<cv::DMatch> >& matches2,
                     std::vector<cv::DMatch>& symMatches )
{

#ifdef _DEBUG
  double startTick, time;
  startTick = (double)cv::getTickCount(); // measure time
#endif
  // for all matches image 1 -> image 2
   for (std::vector<std::vector<cv::DMatch> >::const_iterator
       matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
   {

      // ignore deleted matches
      if (matchIterator1->empty() || matchIterator1->size() < 2)
         continue;

      // for all matches image 2 -> image 1
      for (std::vector<std::vector<cv::DMatch> >::const_iterator
          matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
      {
        // ignore deleted matches
        if (matchIterator2->empty() || matchIterator2->size() < 2)
           continue;

        // Match symmetry test
        if ((*matchIterator1)[0].queryIdx ==
            (*matchIterator2)[0].trainIdx &&
            (*matchIterator2)[0].queryIdx ==
            (*matchIterator1)[0].trainIdx)
        {
            // add symmetrical match
            symMatches.push_back(
              cv::DMatch((*matchIterator1)[0].queryIdx,
                         (*matchIterator1)[0].trainIdx,
                         (*matchIterator1)[0].distance));
            break; // next match in image 1 -> image 2
        }
      }
   }
#ifdef _DEBUG
  time = ((double)cv::getTickCount() - startTick) / cv::getTickFrequency();
  printf("\nTime Reconstruction3D::symmetryTest [s]: %.3f\n", time);
#endif
}

void Reconstruction3D::saveTracks(const std::string &filename, std::vector<std::vector<cv::Vec2d> > passPoints )
{
  std::ofstream hWrite(filename.c_str());


  if (!hWrite.is_open()) {
   consolePrintInfo("Unable to write file: %s", filename.c_str()) ;
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
