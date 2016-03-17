/*****************************************************************************
File:   DataInfo.h
Author: Alex Troyer
  Base class for the different parts of a class (like properties and methods).
*****************************************************************************/
#pragma once

#include <string>

namespace Util
{
  class Serializer;
  class Deserializer;
}

namespace Meta
{
  class Data;

  class DataInfo
  {
  public:
    DataInfo(const std::string &name, Meta::Data *metaData, bool isStatic);

    virtual ~DataInfo() = default;

    Data *GetMeta() const;
    DataInfo &EnableSerialization();
    bool IsStatic() const;
    bool IsSerializable() const;
  
    virtual bool Compare(const void *, const void *);
  
    const std::string &GetName() const;
    virtual void Serialize(const void *object, Util::Serializer &stream);
    virtual void Deserialize(void *object, Util::Deserializer &stream);
  
  private:
    Data *m_MetaData = nullptr;
    std::string m_Name;

    bool m_IsStatic = false;
    bool m_Serialize = false;
  };
}