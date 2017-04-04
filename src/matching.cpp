#include "matching.h"

#ifdef HAVE_OPENCV
#include "core/mathutils.h"
#include "core/messages.h"
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
      msgError(e.what());
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
      msgError(e.what());
    }
    if (_descriptor) mDescriptor.copyTo(*_descriptor);
  }
}

void Features2D::save( const char *fname ) const
{
  //... Habria que guardarlo como binario mejor o dar las dos opciones
  // Salva descriptores
  char ext[I3D_MAX_EXT];
  if (getFileExtension(fname, ext, I3D_MAX_EXT)) {
    return;
  }
  int flags = 0;
  if (strcmp(ext, ".xml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
  } else if (strcmp(ext, ".yml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
  } else if (strcmp(ext, ".bin") == 0) {
    
  } else {
    msgError("Extensión de archivo '%s' no valida", ext);
    return;
  }
  if (strcmp(ext, ".bin") == 0) {
    FILE* fp = std::fopen(fname, "wb");
    if (fp) {
      // Cabecera
      // - KeyPoints
      int32_t size = static_cast<int32_t>(mKeyPoints.size());
      // - Descriptor
      int32_t rows = mDescriptor.rows;
      int32_t cols = mDescriptor.cols;
      int32_t type = mDescriptor.type();
      std::fwrite("TIDOPLIB-Features2D-#01", sizeof("TIDOPLIB-Features2D-#01"), 1, fp);
      std::fwrite(&size, sizeof(int32_t), 1, fp);
      std::fwrite(&rows, sizeof(int32_t), 1, fp);
      std::fwrite(&cols, sizeof(int32_t), 1, fp);
      std::fwrite(&type, sizeof(int32_t), 1, fp);
      char extraHead[200]; // Reserva de espacio para futuros usos
      std::fwrite(&extraHead, sizeof(char), 200, fp);
      //Cuerpo
      for (auto &kp : mKeyPoints) {
        std::fwrite(&kp.pt.x, sizeof(float), 1, fp);
        std::fwrite(&kp.pt.y, sizeof(float), 1, fp);
        std::fwrite(&kp.size, sizeof(float), 1, fp);
        std::fwrite(&kp.angle, sizeof(float), 1, fp);
        std::fwrite(&kp.response, sizeof(float), 1, fp);
        std::fwrite(&kp.octave, sizeof(float), 1, fp);
        std::fwrite(&kp.class_id, sizeof(float), 1, fp);
      }
      std::fwrite(mDescriptor.data, sizeof(float), rows*cols, fp);
      std::fclose(fp);
    } else {
      msgError("No pudo escribir archivo %s", fname);
    } 
  } else {
    cv::FileStorage fs(fname, flags);
    if (fs.isOpened()) {
      if (!mKeyPoints.empty()) write(fs, "keypoints", mKeyPoints);
      if (!mDescriptor.empty()) write(fs, "descriptors", mDescriptor);
      fs.release();
    } else
      msgError("No pudo escribir archivo %s", fname);
  }
}

void Features2D::read( const char *fname )
{
  char ext[I3D_MAX_EXT];
  if (getFileExtension(fname, ext, I3D_MAX_EXT) == 0) {
    if (strcmp(ext, ".bin") == 0) {
      if (FILE* fp = std::fopen(fname, "rb")) {
        //cabecera
        char h[24];
        int size;
        int rows;
        int cols;
        int type;
        char extraHead[200];
        std::fread(h, sizeof(char), 24, fp);
        std::fread(&size, sizeof(int32_t), 1, fp);
        std::fread(&rows, sizeof(int32_t), 1, fp);
        std::fread(&cols, sizeof(int32_t), 1, fp);
        std::fread(&type, sizeof(int32_t), 1, fp);
        std::fread(&extraHead, sizeof(char), 200, fp);
        //Cuerpo
        mKeyPoints.resize(size);
        for (auto &kp : mKeyPoints) {
          std::fread(&kp.pt.x, sizeof(float), 1, fp);
          std::fread(&kp.pt.y, sizeof(float), 1, fp);
          std::fread(&kp.size, sizeof(float), 1, fp);
          std::fread(&kp.angle, sizeof(float), 1, fp);
          std::fread(&kp.response, sizeof(float), 1, fp);
          std::fread(&kp.octave, sizeof(float), 1, fp);
          std::fread(&kp.class_id, sizeof(float), 1, fp);
        }
        cv::Mat aux(rows, cols, type);
        std::fread(aux.data, sizeof(float), rows*cols, fp);
        aux.copyTo(mDescriptor);
        std::fclose(fp);
      } else
        msgError("No pudo leer archivo %s", fname);
    } else if (strcmp(ext, ".xml") == 0 || strcmp(ext, ".yml") == 0) {
      cv::FileStorage fs(fname, cv::FileStorage::READ);
      if (fs.isOpened()) {
        mKeyPoints.resize(0);
        mDescriptor.resize(0);
        fs["keypoints"] >> mKeyPoints;
        fs["descriptors"] >> mDescriptor;
        fs.release();
      } else msgError("No pudo leer archivo %s", fname);
    }
  } else msgError("Fichero no valido: %s", fname);
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
    msgError(e.what());
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
    
    for (size_t i = 0; i < match_aux.size(); i++) {
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
    while ( rmse > 0.5 ) {
      for (size_t i = 0, j = 0; i < pts1.size(); i++) {
        if (sqrt(err[i]) > 2*rmse) {
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

void Matching::save(const char *fname ) const
{
  char ext[I3D_MAX_EXT];
  if (getFileExtension(fname, ext, I3D_MAX_EXT)) {
    msgError("Fichero no valido: %s", fname);
    return;
  }
  if (strcmp(ext, ".bin") == 0) {
    FILE* fp = std::fopen(fname, "wb");
    if (fp) {
      // Cabecera
      int size = static_cast<int>(mMatches.size());
      std::fwrite("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"), 1, fp);
      std::fwrite(&size, sizeof(int32_t), 1, fp);
      char extraHead[100]; // Reserva de espacio para futuros usos
      std::fwrite(&extraHead, sizeof(char), 100, fp);
      //Cuerpo
      for (int i = 0; i < mMatches.size(); i++) {
        std::fwrite(&mMatches[i].queryIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&mMatches[i].trainIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&mMatches[i].imgIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&mMatches[i].distance, sizeof(float), 1, fp);
      }
      std::fclose(fp);
    } else {
      msgError("No pudo escribir archivo %s", fname);
    } 
  }
}

void Matching::load( const char *fname )
{
  char ext[I3D_MAX_EXT];
  if (getFileExtension(fname, ext, I3D_MAX_EXT) == 0) {
    if (strcmp(ext, ".bin") == 0) {
      if (FILE* fp = std::fopen(fname, "rb")) {
        //cabecera
        char h[22];
        int size;
        char extraHead[100];
        std::fread(h, sizeof(char), 22, fp);
        std::fread(&size, sizeof(int32_t), 1, fp);
        std::fread(&extraHead, sizeof(char), 100, fp);
        //Cuerpo
        mMatches.resize(size);
        for (auto &match : mMatches) {
          std::fread(&match.queryIdx, sizeof(int32_t), 1, fp);
          std::fread(&match.trainIdx, sizeof(int32_t), 1, fp);
          std::fread(&match.imgIdx, sizeof(int32_t), 1, fp);
          std::fread(&match.distance, sizeof(float), 1, fp);
        }
        std::fclose(fp);
      } else
        msgError("No pudo leer archivo %s", fname);
    }
  } else msgError("Fichero no valido: %s", fname);
}

} // End namespace I3D

#endif // HAVE_OPENCV