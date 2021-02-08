#include "tidop/graphic/layer.h"


namespace tl
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */


GLayer::GLayer() 
  : mName(""), 
    mEntities(0), 
    mSelectEntity(nullptr)
{
}

GLayer::GLayer(const GLayer &gLayer) 
  : mName(gLayer.mName), 
    mEntities(gLayer.mEntities), 
    mSelectEntity(gLayer.mSelectEntity)
{
}

GLayer::GLayer(GLayer &&gLayer) TL_NOEXCEPT
  : mName(std::move(gLayer.mName)), 
    mEntities(std::move(gLayer.mEntities)), 
    mSelectEntity(std::move(gLayer.mSelectEntity))
{
}

GLayer::GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities) 
  : mName(""), 
    mEntities(entities), 
    mSelectEntity(nullptr)
{
}

GLayer::iterator GLayer::begin() TL_NOEXCEPT
{
  return mEntities.begin();
}

GLayer::const_iterator GLayer::begin() const TL_NOEXCEPT
{
  return mEntities.cbegin();
}

GLayer::iterator GLayer::end() TL_NOEXCEPT
{
  return mEntities.end();
}

GLayer::const_iterator GLayer::end() const TL_NOEXCEPT
{
  return mEntities.cend();
}

void GLayer::push_back(const std::shared_ptr<GraphicEntity> &entity)
{
  mEntities.push_back(entity);
}

void GLayer::push_back(std::shared_ptr<GraphicEntity> &&entity) TL_NOEXCEPT
{
  mEntities.push_back(std::forward<std::shared_ptr<GraphicEntity>>(entity));
}

void GLayer::clear() TL_NOEXCEPT
{ 
  mEntities.clear();
}

bool GLayer::empty() const TL_NOEXCEPT
{
  return mEntities.empty();
}

void GLayer::resize(size_type count)
{
  mEntities.resize(count);
}

void GLayer::resize(size_type count, const std::shared_ptr<GraphicEntity> &value)
{
  mEntities.resize(count, value);
}

GLayer::size_type GLayer::size() const TL_NOEXCEPT
{ 
  return mEntities.size();
}

GLayer &GLayer::operator=(const GLayer& entity) 
{
  if (this != &entity) {
    this->mName = entity.mName;
    this->mEntities = entity.mEntities;
    this->mSelectEntity = entity.mSelectEntity;
  }
  return (*this);
}

GLayer& GLayer::operator=(GLayer&& entity) TL_NOEXCEPT
{
  if (this != &entity) {
    this->mName = std::move(entity.mName);
    this->mEntities.clear();
    this->mEntities = std::move(entity.mEntities);
    this->mSelectEntity = std::move(entity.mSelectEntity);
  }
  return (*this);
}

GLayer::iterator GLayer::erase(GLayer::const_iterator first, GLayer::const_iterator last)
{
  return mEntities.erase(first, last);
}

std::string GLayer::name() const
{
  return mName;
}

void GLayer::setName(const std::string & name)
{
  mName = name;
}

void GLayer::addDataField(const std::shared_ptr<TableField> &field)
{
  mTableFields.push_back(field);
}

std::vector<std::shared_ptr<TableField>> GLayer::tableFields() const
{
  return mTableFields;
}

} // Fin namespace graph

} // End namespace TL
