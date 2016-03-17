/*****************************************************************************
File:   ObjectInfo.h
Author: Alex Troyer
  Stores information on an object like its constructors.
*****************************************************************************/
#pragma once

namespace Meta
{
  class ObjectInfoBase
  {
  public:
    virtual void *Construct() const = 0;
    virtual void PlacementConstruct(void *) const = 0;
    virtual void *Copy(const void *) const = 0;
    virtual void PlacementCopy(void *, const void *) const = 0;
    virtual void *MoveConstruct(void *) const = 0;
    virtual void PlacementMove(void *, void *) const = 0;
    virtual void Destroy(void *) const = 0;
    virtual void Destructor(void *) const = 0;

    virtual bool HasConstructor() const = 0;
    virtual bool HasCopyConstructor() const = 0;
    virtual bool HasMoveConstructor() const = 0;
    virtual bool HasDestructor() const = 0;
    virtual bool IsPolymorphic() const = 0;
  };
}

#include "ObjectInfo.hpp"