#include "tidop/graphic/font.h"

#ifdef HAVE_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#endif // HAVE_OPENCV

namespace TL
{

namespace graph
{

/* ---------------------------------------------------------------------------------- */
/*                                     Clase Font                                    */
/* ---------------------------------------------------------------------------------- */

Font::Font() 
  : mName(),
    mSize(10),
    mStyle(Style::DEFAULT),
    mUnderline(false),
    mStrikethrough(false)
{
}

Font::Font(const Font &font) 
  : mName(font.mName),
    mSize(font.mSize),
    mStyle(font.mStyle),
    mUnderline(font.mUnderline),
    mStrikethrough(font.mStrikethrough)
{
}

Font::Font(Font &&font) TL_NOEXCEPT
  : mName(std::move(font.mName)),
    mSize(std::move(font.mSize)),
    mStyle(std::move(font.mStyle)),
    mUnderline(std::move(font.mUnderline)),
    mStrikethrough(std::move(font.mStrikethrough))
{
}

Font::Font(const std::string name, int size, Style style, bool underline, bool strikethrough)
  : mName(name),
    mSize(size),
    mStyle(style),
    mUnderline(underline),
    mStrikethrough(strikethrough)
{
}

std::string Font::name() const
{
  return mName;
}

int Font::size() const
{
  return mSize;
}


bool Font::isBold() const
{
  return mStyle.isActive(Style::BOLD);
}

bool Font::isItalic() const
{
  return mStyle.isActive(Style::ITALIC);
}

bool Font::isUnderline() const
{
  return mUnderline;
}

bool Font::isStrikethrough() const
{
  return mStrikethrough;
}

void Font::setName(const std::string &name)
{
  mName = name;
}

void Font::setSize(int size)
{
  mSize = size;
}

void Font::setStyle(Style style)
{
  mStyle = style;
}

void Font::setBold(bool active)
{
  mStyle.activeFlag(Style::BOLD, active);
}

void Font::setItalic(bool active)
{
  mStyle.activeFlag(Style::ITALIC, active);
}

void Font::setUnderline(bool active)
{
  mUnderline = active;
}

void Font::setStrikethrough(bool active)
{
  mStrikethrough = active;
}

Font &Font::operator = (const Font &font)
{
  if (this != &font) {
    this->mName = font.mName;
    this->mSize = font.mSize;
    this->mStyle = font.mStyle;
    this->mUnderline = font.mUnderline;
    this->mStrikethrough = font.mStrikethrough;
  }
  return *this;
}

Font &Font::operator = (Font &&font) TL_NOEXCEPT
{
  if (this != &font) {
    this->mName = std::move(font.mName);
    this->mSize = std::move(font.mSize);
    this->mStyle = std::move(font.mStyle);
    this->mUnderline = std::move(font.mUnderline);
    this->mStrikethrough = std::move(font.mStrikethrough);
  }
  return *this;
}

} // End namespace graph

} // End namespace TL
