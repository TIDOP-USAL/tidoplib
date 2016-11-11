#include "matching.h"

#include "core\mathutils.h"
#include "core\messages.h"
#include "transform.h"

#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc.hpp"




namespace I3D
{

/* ---------------------------------------------------------------------------------- */

Features2D::~Features2D()
{
}

int Features2D::detectKeyPoints(const cv::Mat &img, std::vector<cv::KeyPoint> *_keyPoints, const cv::InputArray &mask)
{
  int iret = 0;
  mKeyPoints.clear();
  if (mFeatureDetector) {
    try{
      mFeatureDetector->detect(img, mKeyPoints, mask);
      if (_keyPoints) *_keyPoints = mKeyPoints;
      iret = static_cast<int>(mKeyPoints.size());
    } catch (cv::Exception &e) {
      printError(e.what());
    }
  } else if (_keyPoints) _keyPoints->clear();
  return iret;
}

void Features2D::calcDescriptor(const cv::Mat &img, std::vector<cv::KeyPoint> *_keyPoints, cv::Mat *_descriptor)
{
  if (mDescriptorExtractor) {
    mDescriptor.resize(0);
    try{
      mDescriptorExtractor->compute(img, !_keyPoints ? mKeyPoints : *_keyPoints, mDescriptor);
    } catch (cv::Exception &e) {
      printError(e.what());
    }
    if (_descriptor) mDescriptor.copyTo(*_descriptor);
  }
}

void Features2D::save( const char *fname ) const
{
  //... Habria que guardarlo como binario mejor o dar las dos opciones
  // Salva descriptores
  char ext[I3D_MAX_EXT];
  if (getFileExtension(fname, ext)) {
    return;
  }
  int flags = 0;
  if (strcmp(ext, ".xml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
  } else if (strcmp(ext, ".yml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
  } else {
    printError("Extensión de archivo '%s' no valida", ext);
    return;
  }
  cv::FileStorage fs(fname, flags);
  if (fs.isOpened()) {
    if (!mKeyPoints.empty()) write(fs, "keypoints", mKeyPoints);
    if (!mDescriptor.empty()) write(fs, "descriptors", mDescriptor);
    fs.release();
  } else
    printError("No pudo escribir archivo %s", fname);
}

void Features2D::read( const char *fname )
{
  cv::FileStorage fs(fname, cv::FileStorage::READ);
  if (fs.isOpened()) {
    mKeyPoints.resize(0);
    mDescriptor.resize(0);
    fs["keypoints"] >> mKeyPoints;
    fs["descriptors"] >> mDescriptor;
    fs.release();
  } else
    printError("No pudo leer archivo %s", fname);
}

/* ---------------------------------------------------------------------------------- */

int Matching::match(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector< cv::DMatch > *_matches)
{
  mMatches.clear();
  auto getAppropriateFormat = [](const cv::Mat &descIn) -> cv::Mat {
    cv::Mat descOut;
    descIn.copyTo(descOut);
    if (descOut.channels() != 1) cv::cvtColor(descOut, descOut, CV_BGR2GRAY);
    if (descOut.type() != CV_32F) descOut.convertTo(descOut, CV_32FC1);
    return std::move(descOut);
  };

  try {
    if (mDescriptorMatcher) mDescriptorMatcher->match(getAppropriateFormat( descriptor1), getAppropriateFormat( descriptor2 ), mMatches);
  } catch (cv::Exception &e) {
    printError(e.what());
  }
  
  if (_matches) *_matches = mMatches;
  return static_cast<int>(mMatches.size());
}

int Matching::match(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *_matches)
{
  return match(feat1.getDescriptors(), feat2.getDescriptors(), _matches);
}

void Matching::getGoodMatches(std::vector< cv::DMatch > *gm, double ratio) const
{
  if (!mMatches.empty() && gm) {
    gm->clear();
    std::vector<cv::DMatch> match_aux = mMatches;
    sortVector(&match_aux);
    double min_dist = match_aux.front().distance;
    
    for (int i = 0; i < match_aux.size(); i++) {
      if (match_aux[i].distance <= std::max(2 * min_dist, ratio)) {
        gm->push_back(match_aux[i]);
      }
    }
  }
}

void Matching::getGoodMatches(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *gm, double ratio) const
{
  getGoodMatches(feat1.getKeyPoints(), feat2.getKeyPoints(), gm, ratio);
}

void Matching::getGoodMatches(const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2, 
                              std::vector< cv::DMatch > *gm, double ratio) const
{
  if (!mMatches.empty() && gm) {
    // Para hacer un primer filtrado
    getGoodMatches(gm, ratio);
    if (gm && gm->empty()) *gm = mMatches;
    size_t nPoints = gm->size();
    std::vector<cv::Point2f> pts1(nPoints);
    std::vector<cv::Point2f> pts2(nPoints);
    for (size_t igm = 0; igm < nPoints; igm++) {
      pts1[igm] = keyPoints1[(*gm)[igm].queryIdx].pt;
      pts2[igm] = keyPoints2[(*gm)[igm].trainIdx].pt;
    }

    // Se calcula una transformación perspectiva y se van limpiando puntos
    //... Hay que revisar el valor de error medio cuadrático que se pone y
    //    la forma de ver la convergencia de la solución
    TrfPerspective<cv::Point2f> trfPerps;
    std::vector<double> err;
    double rmse = trfPerps.rootMeanSquareError(pts1, pts2, &err);
    while ( rmse > 10. ) {
      for (size_t i = 0, j = 0; i < pts1.size(); i++) {
        if (sqrt(err[i]) > rmse) {
          pts1.erase(pts1.begin() + j);
          pts2.erase(pts2.begin() + j);
          gm->erase(gm->begin() + j);
        } else j++;
      }
      rmse = trfPerps.rootMeanSquareError(pts1, pts2, &err);
    }
  }
}

void Matching::getGoodMatches(const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2, 
                              std::vector< cv::DMatch > *gm, double distance, double confidence) const
{
  if (!mMatches.empty() && gm) {
    gm->clear();
    size_t nPoints = mMatches.size();
    std::vector<cv::Point2f> pts1(nPoints);
    std::vector<cv::Point2f> pts2(nPoints);

    for (size_t igm = 0; igm < nPoints; igm++) {
      pts1[igm] = keyPoints1[(*gm)[igm].queryIdx].pt;
      pts2[igm] = keyPoints2[(*gm)[igm].trainIdx].pt;
    }

    std::vector<uchar> inliers(nPoints, 0);
    cv::Mat fundamental = cv::findFundamentalMat(cv::Mat(pts1), cv::Mat(pts2), inliers, CV_FM_RANSAC, distance, confidence);

    std::vector<uchar>::const_iterator itIn = inliers.begin();

    std::vector<cv::DMatch>::const_iterator itM = mMatches.begin();

    for (; itIn != inliers.end(); ++itIn, ++itM) {
      if (*itIn) { 
        gm->push_back(*itM);
      }
    }
  }
}

void Matching::getGoodMatches(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *gm, double distance, double confidence) const
{
  getGoodMatches(feat1.getKeyPoints(), feat2.getKeyPoints(), gm, distance, confidence);
}


  //int nft = feature1.detectKeyPoints(image1);
  //feature1.calcDescriptor(image1);
  //nft = feature2.detectKeyPoints(image2);
  //feature2.calcDescriptor(image2);
  //
  ////matcherA.add(std::vector<cv::Mat>(1, feature2.getDescriptors()));
  //std::vector <std::vector<cv::DMatch>> matchesA;
  //matcherA.knnMatch(feature1.getDescriptors(), feature2.getDescriptors(), matchesA, 2);

  //std::vector<cv::DMatch> goodMatchesA;
  //int max_track_number = 0;
  //for (size_t iMatch = 0; iMatch < matchesA.size(); ++iMatch) {
  //  float distance0 = matchesA[iMatch][0].distance;
  //  float distance1 = matchesA[iMatch][1].distance;
  //  if (distance0 < 0.8 * distance1) {
  //    goodMatchesA.push_back(matchesA[iMatch][0]);
  //  }
  //}

} // End namespace I3D
