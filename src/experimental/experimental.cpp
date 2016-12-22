#include "experimental.h"

#include <thread>

#include "core/messages.h"
#include "core/defs.h"

#include "opencv2/highgui/highgui.hpp"
#ifdef I3D_ENABLE_OPENCV_SFM
I3D_SUPPRESS_WARNINGS
#include "opencv2/core/eigen.hpp"
#include "opencv2/sfm/libmv_light/libmv_capi.h"
#include "libmv/simple_pipeline/reconstruction.h"
#include "libmv/simple_pipeline/camera_intrinsics.h"
#include "libmv/correspondence/matches.h"
#include "libmv/multiview/robust_fundamental.h"
I3D_DEFAULT_WARNINGS
#endif

namespace I3D
{

namespace EXPERIMENTAL
{

//// http://docs.opencv.org/3.1.0/dc/d2c/tutorial_real_time_pose.html

int RobustMatching::ratioTest(std::vector<std::vector<cv::DMatch> > &matches)
{
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
    //int max_track_number = 0;
    for (size_t i = 0; i < matches.size(); i++) {
      float distance0 = matches[i][0].distance;
      float distance1 = matches[i][1].distance;
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


void RobustMatching::robustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<std::vector<cv::DMatch>> *pMatches12, std::vector<std::vector<cv::DMatch>> *pMatches21)
{
  std::vector<std::vector<cv::DMatch> > matches12, matches21;

  mDescriptorMatcher->knnMatch(descriptor1, descriptor2, matches12, 2); // return 2 nearest neighbours
  if (pMatches21) *pMatches12 = matches12;

  mDescriptorMatcher->knnMatch(descriptor2, descriptor1, matches21, 2); // return 2 nearest neighbours
  if (pMatches21) *pMatches21 = matches21;
}

void RobustMatching::fastRobustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<std::vector<cv::DMatch>> *pMatches)
{
  std::vector<std::vector<cv::DMatch> > matches;
  mDescriptorMatcher->knnMatch(descriptor1, descriptor2, matches, 2);
  if (pMatches) *pMatches = matches;
}

















/* ----------------------------------------------------------------------------------------- */
#ifdef I3D_ENABLE_OPENCV_SFM

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
    /*int nft = */mFeature2D->detectKeyPoints(image, &keyPoint);
    mFeature2D->calcDescriptor(image, NULL, &descriptor);

    mKeyPoints[i] = keyPoint;
    mDescriptor[i] = descriptor;

    if (bSave) {
      char out[I3D_MAX_PATH];
      changeFileExtension(mImagesPaths[i].c_str(), "xml", out, I3D_MAX_PATH);

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
    changeFileExtension(mImagesPaths[i].c_str(), "xml", out, I3D_MAX_PATH);
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
    bool bfirst = true;
    int itrack = 0;
    // Se determinan los key points y los descriptores 
    getKeyPointAndDescriptor(images, true);
    // Con esto los tengo en mKeyPoints y mDescriptor
    size_t size = images.size();

    std::vector<std::vector<int> > idx_pass_points;
    std::vector<std::vector<cv::Vec2d> > pass_points;

    // matching cruzado
    //bool bRes2 = true;
    for (int i = 0; i < images.size(); ++i) {
      for (int j = 0; j < i; ++j) {
        std::vector<std::vector<cv::DMatch>> robus_matches;
        mRobustMatching->fastRobustMatch(mDescriptor[i], mDescriptor[j], &robus_matches);

        std::vector<std::vector<cv::DMatch>> symMatches;
        mRobustMatching->symmetryTest(robus_matches, &symMatches);

        std::vector<Eigen::MatrixXd> x;
        //libmv::vector<int> tracks/*, images*/;

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
        //double max_error = 0.1;
        // TODO(pmoulon) Make the Correspondence filter a parameter.
        //HomographyFromCorrespondences2PointRobust(x[0], x[1], 0.3, &H, &inliers);
        //HomographyFromCorrespondences4PointRobust(x[0], x[1], 0.3, &H, &inliers);
        //AffineFromCorrespondences2PointRobust(x[0], x[1], 1, &H, &inliers);
        libmv::FundamentalFromCorrespondences7PointRobust(x[0], x[1], 1.0, &H, &inliers);
        
        std::vector<cv::Point2f> pts1;
        std::vector<cv::Point2f> pts2;
        for (int igm = 0; igm < inliers.size(); igm++) {
          idx1 = symMatches[inliers[igm]][0].queryIdx;
          idx2 = symMatches[inliers[igm]][0].trainIdx;

          //Busqueda de si ya esta añadido ese punto
          bool addNew = true;
          if (bfirst == false) {
            for (int k = 0; k < idx_pass_points.size(); k++) {
              if (idx_pass_points[k][i] == idx1) {
                addNew = false;
                idx_pass_points[k][j] = idx2;
                cv::Point2f point2 = mKeyPoints[j][idx2].pt;
                if (pass_points[k][j] == cv::Vec2d(-1, -1)) {
                  tracks.Insert(j, k, point2.x, point2.y, 1);
                  pass_points[k][j] = cv::Vec2d(point2.x, point2.y);
                }
                break;
              } else if (idx_pass_points[k][j] == idx2) {
                addNew = false;
                idx_pass_points[k][i] = idx1;
                cv::Point2f point1 = mKeyPoints[i][idx1].pt;
                if (pass_points[k][i] == cv::Vec2d(-1, -1)) {
                  tracks.Insert(i, k, point1.x, point1.y, 1);
                  pass_points[k][i] = cv::Vec2d(point1.x, point1.y);
                }
                break;
              }
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
            tracks.Insert(i, itrack, point1.x, point1.y, 1);
            tracks.Insert(j, itrack, point2.x, point2.y, 1);
            itrack++;
          }
        }
        bfirst = false;
      }
    }

    libmv::Tracks sort_tracks;
    // ordenar tracks.
    for (int it = 0; it < images.size(); it++) {
      vector<Marker> marker = tracks.MarkersInImage(it);
      for (int im = 0; im < marker.size(); im++) {
        marker[im].track;
        sort_tracks.Insert(it, marker[im].track, marker[im].x, marker[im].y, marker[im].weight);
      }
    }


    //for (int i = 0; i < size; ++i) {
    //  cv::Mat_<double> frame(2, pass_points.size());
    //  for (int j = 0; j < pass_points.size(); ++j)
    //  {
    //    frame(0,j) = pass_points[j][i][0];
    //    frame(1,j) = pass_points[j][i][1];
    //  }
    //  points3d_estimated.push_back(cv::Mat(frame));
    //}

    //saveTracks("D:\\Desarrollo\\datos\\Videos\\Torres_Pasillo_illescas\\tracks.txt", pass_points);
    
    //int nPassPoint = static_cast<int>(pass_points.size());

    printInfo("Número de puntos de paso encontrados: %i", itrack);


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

    //reconstruction = cv::sfm::SFMLibmvEuclideanReconstruction::create(camera_instrinsic_options, reconstruction_options);

    //libmv_solveReconstruction(tracks,
    //                               libmv_camera_intrinsics_options,
    //                               libmv_reconstruction_options);

    // Perform reconstruction
    libmv_Reconstruction *rec = libmv_solveReconstruction(sort_tracks,
                                     &camera_instrinsic_options,
                                     &reconstruction_options);

    const size_t n_views = rec->reconstruction.AllCameras().size();
    //Rs.create(n_views, 1, CV_64F);
    //Ts.create(n_views, 1, CV_64F);
    Rs_est.resize(n_views);
    ts_est.resize(n_views);
    
    Matx33d R;
    Vec3d t;
    for(int i = 0; i < n_views; ++i)
    {
      eigen2cv(rec->reconstruction.AllCameras()[i].R, R);
      eigen2cv(rec->reconstruction.AllCameras()[i].t, t);
      //cv::Mat(R).copyTo(Rs.getMatRef(i));
      //cv::Mat(t).copyTo(Ts.getMatRef(i));
      Rs_est[i] = cv::Mat(R);
      ts_est[i] = cv::Mat(t);
    }

    int n_points = rec->reconstruction.AllPoints().size();

    //points3d.create(n_points, 1, CV_64F);

    cv::Vec3d point3d;
    for ( int i = 0; i < n_points; ++i ) {
      for ( int j = 0; j < 3; ++j )
        point3d[j] = rec->reconstruction.AllPoints()[i].X[j];
      //cv::Mat(point3d).copyTo(points3d.getMatRef(i));
      points3d_estimated.push_back(cv::Mat(point3d));
    }

    eigen2cv(rec->intrinsics->K(), K);
  }

}

#endif


/* ---------------------------------------------------------------------------------- */



//BresenhamLine::BresenhamLine()
//{}
//
//BresenhamLine::~BresenhamLine()
//{}

cv::Point &BresenhamLine::operator*()
{
  return mPos;
}

BresenhamLine &BresenhamLine::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, mStepX, mStepY);
  } else {
    _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, mStepY, mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator ++(int) 
{
  BresenhamLine it = *this;
  ++(*this);
  return it;
}

BresenhamLine &BresenhamLine::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, -mStepX, -mStepY);
  } else {
    _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, -mStepY, -mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator --(int) 
{
  BresenhamLine it = *this;
  --(*this);
  return it;
}

BresenhamLine &BresenhamLine::begin()
{
  mPos = mPt1;
  return *this;
}

BresenhamLine &BresenhamLine::end() 
{
  mPos = mPt2;
  return *this;
}

cv::Point BresenhamLine::position(int id)
{
  if (id == -1) {
    return mPos;
  } else {
    begin();
    for (int i = 0; i < id; i++) ++(*this);
    return mPos;
  }
}

/*
//http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void Line( const float x1, const float y1, const float x2, const float y2, const Color& color )
{
  // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
 
  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);
 
  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;
 
  const int maxX = (int)x2;
 
  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        SetPixel(y,x, color);
    }
    else
    {
        SetPixel(x,y, color);
    }
 
    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}
*/

void BresenhamLine::init()
{
  if (dy < 0) { 
    dy = -dy; 
    mStepY = -1; 
  } else {
    mStepY = 1;
  }

  if (dx < 0) {  
    dx = -dx;  
    mStepX = -1; 
  } else {
    mStepX = 1;
  }

  if (dx > dy) {
    mCount = dx + 1;
  } else {
    mCount = dy + 1;
  }

  if (dx > dy) {
    p = 2 * dy - dx;
    incE = 2 * dy;
    incNE = 2 * (dy - dx);
  } else {
    p = 2 * dx - dy;
    incE = 2 * dx;
    incNE = 2 * (dx - dy);
  }
}

void BresenhamLine::_next(int *max, int *min, int dMax, int dMin, int endMax, int stepMax, int stepMin) 
{
  if (*max < endMax) {
    *max += stepMax;
    if (p < 0) {
      p += incE;
    } else {
      *min += stepMin;
      p += incNE;
    }
  }
}

int BresenhamLine::size() const 
{
  return mCount;
}

std::vector<cv::Point> BresenhamLine::getPoints()
{
  std::vector<cv::Point> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

//DDA::DDA()
//{}
//
//DDA::~DDA()
//{}

cv::Point &DDA::operator*()
{
  return mPos;
}

DDA &DDA::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator ++(int) 
{
  DDA it = *this;
  ++(*this);
  return it;
}

DDA &DDA::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator --(int) 
{
  DDA it = *this;
  --(*this);
  return it;
}

DDA &DDA::begin()
{
  mPos = mPt1;
  return *this;
}

DDA &DDA::end() 
{
  mPos = mPt2;
  return *this;
}

cv::Point DDA::position(int id) 
{
  if (id == -1) {
    return mPos;
  } else {
    begin();
    for (int i = 0; i < id; i++) ++(*this);
    return mPos;
  }
}

void DDA::init()
{
  if (dx > dy) {
    m = (float)dy / (float)dx;
    b = mPt1.y - m * mPt1.x;
  } else {
    m = (float)dx / (float)dy;
    b = mPt1.x - m * mPt1.y;
  }

  if (dy < 0) {
    dy = -dy;
    mStepY = -1;
  } else {
    mStepY = 1;
  }

  if (dx < 0) {
    dx = -dx;
    mStepX = -1;
  } else {
    mStepX = 1;
  }
}

void DDA::_next(int *max, int *min, int dMin, int endMax, int step) 
{
  if (*max < endMax) {
    *max += step;
    if ( dMin != 0) 
      *min = I3D_ROUND_TO_INT(m * *max + b);
  }
}

int DDA::size() const 
{
  return mCount;
}

std::vector<cv::Point> DDA::getPoints()
{
  std::vector<cv::Point> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */


} // End namespace EXPERIMENTAL

} // End namespace I3D

