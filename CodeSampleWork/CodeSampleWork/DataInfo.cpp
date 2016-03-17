/*****************************************************************************
File:   DataInfo.cpp
Author: Alex Troyer
  Base class for the different parts of a class (like properties and methods).
*****************************************************************************/
#include "DataInfo.h"

namespace Meta
{
  // Constructor which sets the name, meta data, and if it is static.
  DataInfo::DataInfo(const std::string &name, Meta::Data *metaData, bool isStatic)
    : m_Name(name)
    , m_MetaData(metaData)
    , m_IsStatic(isStatic)
  {
  }

  // Get the meta data associated with this.
  Data *DataInfo::GetMeta() const
  {
    return m_MetaData;
  }

  // Enables serialization on this if supported.
  DataInfo &DataInfo::EnableSerialization()
  {
    m_Serialize = true;
    return *this;
  }

  // Whether or not it is static.
  bool DataInfo::IsStatic() const
  {
    return m_IsStatic;
  }

  // Whether or not we can serialize this object.
  bool DataInfo::IsSerializable() const
  {
    return m_Serialize;
  }

  // Default comparison which returns false.
  bool DataInfo::Compare(const void *, const void *)
  {
    return false;
  }

  // Get the name of this object.
  const std::string &DataInfo::GetName() const
  {
    return m_Name;
  }

  // Default Serialize which does nothing.
  void DataInfo::Serialize(const void *, Util::Serializer &)
  {
  }

  // Default Deserialize which does nothing.
  void DataInfo::Deserialize(void *, Util::Deserializer &)
  {
  }
}