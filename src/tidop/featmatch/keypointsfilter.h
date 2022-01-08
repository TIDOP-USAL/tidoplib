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

#ifndef TL_FEATMATCH_KEYPOINTSFILTER_H
#define TL_FEATMATCH_KEYPOINTSFILTER_H

#include <vector>

#include <opencv2/features2d.hpp>

#include <tidop/core/flags.h>

namespace tl
{


class TL_EXPORT KeyPointsFilter
{

public:

  enum class Type
  {
    n_best,
    size,
    remove_duplicated,
    mask
  };

public:

  KeyPointsFilter() {}
  virtual ~KeyPointsFilter() = default;

  /*!
   * \brief Recover the default values
   */
  virtual void reset() = 0;

  /*!
   * \brief type of keypoints filter
   * \return filter type
   */
  virtual Type type() const = 0;
  virtual std::string name() const = 0;

};
ALLOW_BITWISE_FLAG_OPERATIONS(KeyPointsFilter::Type)



class TL_EXPORT KeyPointsFilterBase
  : public KeyPointsFilter
{

public:

  KeyPointsFilterBase(Type type) : mFilterType(type) {}
  ~KeyPointsFilterBase() override = default;

  /*!
   * \brief type of keypoints filter
   * \return filter type
   */
  Type type() const override
  { 
    return mFilterType.flags(); 
  }

protected:

  tl::EnumFlags<Type> mFilterType;
};



/*----------------------------------------------------------------*/


class TL_EXPORT KeyPointsFilterProcess
{

public:

  KeyPointsFilterProcess() {}
  virtual ~KeyPointsFilterProcess() = default;

  /*!
   * \brief filter
   * \param[in] keypoints
   * \return Filtered Keypoints
   */
  virtual std::vector<cv::KeyPoint> filter(const std::vector<cv::KeyPoint> &keypoints) = 0;
};


/*----------------------------------------------------------------*/


class TL_EXPORT KeyPointsFilterNBestProperties
  : public KeyPointsFilterBase
{

public:

  KeyPointsFilterNBestProperties();
  ~KeyPointsFilterNBestProperties() override = default;

  /*!
   * \brief Number of points to retain
   * \return Number of points
   */
  virtual int nPoints() const;

  /*!
   * \brief Set the number of points to retain
   * \param[in] nPoints number of points to retain
   */
  virtual void setNPoints(int nPoints);

// KeyPointsFilter interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mPointsNumber;
};


/*----------------------------------------------------------------*/


class TL_EXPORT KeyPointsFilterNBest
  : public KeyPointsFilterNBestProperties,
    public KeyPointsFilterProcess
{

public:

  KeyPointsFilterNBest();
  explicit KeyPointsFilterNBest(int nPoints);
  ~KeyPointsFilterNBest() override = default;

// KeyPointsFilterNBestProperties interface

public:

  void setNPoints(int nPoints) override;

// KeyPointsFilter interface

public:

  void reset() override;

// KeyPointsFilterProcess interface

public:

  std::vector<cv::KeyPoint> filter(const std::vector<cv::KeyPoint> &keypoints) override;

};


/*----------------------------------------------------------------*/


class TL_EXPORT KeyPointsFilterBySizeProperties
  : public KeyPointsFilterBase
{

public:

  KeyPointsFilterBySizeProperties();
  ~KeyPointsFilterBySizeProperties() override = default;

  /*!
   * \brief Minimum size
   * \return Minimum size
   */
  virtual double minSize() const;

  /*!
   * \brief set minimum size
   * \param[in] minSize
   */
  virtual void setMinSize(double minSize);

  /*!
   * \brief maximum size
   * \return maximum size
   */
  virtual double maxSize() const;

  /*!
   * \brief Set maximum size
   * \param[in] maxSize Maximum size
   */
  virtual void setMaxSize(double maxSize);

// KeyPointsFilter interface

public:

  void reset() override;
  std::string name() const final;

private:

  double mMinSize;
  double mMaxSize;

};


/*----------------------------------------------------------------*/


class TL_EXPORT KeyPointsFilterBySize
  : public KeyPointsFilterBySizeProperties,
    public KeyPointsFilterProcess
{

public:

  KeyPointsFilterBySize();
  KeyPointsFilterBySize(double minSize, double maxSize);
  ~KeyPointsFilterBySize() override = default;

// KeyPointsFilterBySizeProperties interface

public:

  void setMinSize(double minSize) override;
  void setMaxSize(double maxSize) override;

// KeyPointsFilterProcess interface

public:

  std::vector<cv::KeyPoint> filter(const std::vector<cv::KeyPoint> &keypoints) override;

// KeyPointsFilter interface

public:

  void reset() override;

};



/*----------------------------------------------------------------*/

class TL_EXPORT KeyPointsFilterRemoveDuplicatedProperties
  : public KeyPointsFilterBase
{

public:

  KeyPointsFilterRemoveDuplicatedProperties();
  ~KeyPointsFilterRemoveDuplicatedProperties() override = default;

// KeyPointsFilter interface

public:

  void reset() override;
  std::string name() const final;

};


/*----------------------------------------------------------------*/



class TL_EXPORT KeyPointsFilterRemoveDuplicated
  : public KeyPointsFilterRemoveDuplicatedProperties,
    public KeyPointsFilterProcess
{

public:

  KeyPointsFilterRemoveDuplicated();
  ~KeyPointsFilterRemoveDuplicated() override = default;

// KeyPointsFilterProcess interface

public:

  std::vector<cv::KeyPoint> filter(const std::vector<cv::KeyPoint> &keypoints) override;

// KeyPointsFilter interface

public:

  void reset() override;
};


} // namespace tl

#endif // TL_FEATMATCH_KEYPOINTSFILTER_H
