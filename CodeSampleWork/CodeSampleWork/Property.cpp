#include "Property.h"
#include "Meta.h"

namespace Meta
{
  Property::Property(const std::string &name, Meta::Data *metaData, bool isStatic)
    : DataInfo(name, metaData, isStatic)
  {

  }

  Any Property::Get(const void *)
  {
    return Any();
  }

  Any Property::Get()
  {
    return Any();
  }

  void Property::Set(void *, const void *)
  {
  }

  void Property::Set(const void *)
  {
  }

  bool Property::Compare(const void *, const void *)
  {
    return false;
  }

  void Property::Assignment(void *, const void *)
  {
  }
}