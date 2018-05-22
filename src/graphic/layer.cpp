#include "graphic/layer.h"


namespace TL
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */


GLayer::GLayer() 
  : mName(""), 
    mEntities(0), 
    mSelectEntity(0)
{
}

GLayer::GLayer(const GLayer &gLayer) 
  : mName(gLayer.mName), 
    mEntities(gLayer.mEntities), 
    mSelectEntity(gLayer.mSelectEntity)
{
}

GLayer::GLayer(GLayer &&gLayer) 
  : mName(std::move(gLayer.mName)), 
    mEntities(std::move(gLayer.mEntities)), 
    mSelectEntity(std::move(gLayer.mSelectEntity))
{
}

GLayer::GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities) 
  : mName(""), 
    mEntities(entities), 
    mSelectEntity(0)
{
}

GLayer::iterator GLayer::begin() 
{
  return mEntities.begin();
}

GLayer::const_iterator GLayer::begin() const 
{
  return mEntities.cbegin();
}

GLayer::iterator GLayer::end()
{
  return mEntities.end();
}

GLayer::const_iterator GLayer::end() const 
{
  return mEntities.cend();
}

void GLayer::add(const std::shared_ptr<GraphicEntity> &entity)
{
  mEntities.push_back(entity);
}

void GLayer::push_back(const std::shared_ptr<GraphicEntity> &entity)
{
  mEntities.push_back(entity);
}

void GLayer::push_back(std::shared_ptr<GraphicEntity> &&entity)
{
  mEntities.push_back(std::forward<std::shared_ptr<GraphicEntity>>(entity));
}

void GLayer::clear() 
{ 
  mEntities.clear();
}

bool GLayer::empty() const
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

GLayer::size_type GLayer::size() const
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

GLayer& GLayer::operator=(GLayer&& entity) 
{
  if (this != &entity) {
    this->mName = std::move(entity.mName);
    this->mEntities.clear();
    this->mEntities = std::move(entity.mEntities);
    this->mSelectEntity = std::move(entity.mSelectEntity);
  }
  return (*this);
}

std::string GLayer::getName() const
{
	return mName;
}

void GLayer::setName(const std::string & name)
{
  mName = name;
}


} // Fin namespace graph

} // End namespace TL
