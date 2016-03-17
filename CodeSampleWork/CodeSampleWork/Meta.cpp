/*****************************************************************************
File:   Meta.cpp
Author: Alex Troyer
  The heart of what makes Meta work.
*****************************************************************************/
#include "Meta.h"
#include "ObjectInfo.h"
#include "Method.h"
#include "Property.h"
#include "Error.h"

namespace Meta
{
  // Find meta data by name.
  Data *NamedMetaStorage::GetMeta(const std::string &name)
  {
    auto it = m_MetaMap->find(name);

    if(it != m_MetaMap->end())
    {
      return it->second;
    }

    return nullptr;
  }

  // If this wasn't a pointer, we would have a fun issue with static initialization order.
  // Since m_MetaMap is set to nullptr before objects are constructed, we can then
  // say when we want to construct it (which is going to be when we first want to insert
  // into it).
  std::unordered_map<std::string, Data *> *NamedMetaStorage::m_MetaMap = nullptr;

  // Add meta data by name.
  void NamedMetaStorage::AddMetaData(const std::string &name, Data *meta)
  {
    // We want to make sure that the map is constructed before adding anything to it,
    // so construct it now if it doesn't exist.
    if(!m_MetaMap)
      m_MetaMap = new std::unordered_map<std::string, Data *>();

    m_MetaMap->insert({name, meta});
  }

  // Get the properties in the order they were registered in.
  const OrderedVector &Data::GetOrderedData() const
  {
    return m_OrderedVector;
  }

  // Get the map of all the methods registered.
  const MethodMap &Data::GetMethodMap() const
  {
    return m_MethodMap;
  }

  // Get a map of all the properties registered.
  const PropertyMap &Data::GetPropertyMap() const
  {
    return m_PropertyMap;
  }

  // Add a method to the meta data.
  Method &Data::AddMethod(Method *method)
  {
    auto it = m_MethodMap.find(method->GetName());

    std::shared_ptr<MethodOverloads> methodOverloads;

    // If the method doesn't exist yet, make an overload for it.
    if(it == m_MethodMap.end())
    {
      methodOverloads = std::make_shared<MethodOverloads>();
      m_MethodMap.insert({method->GetName(), methodOverloads});
      methodOverloads->m_Owner = this;
    }
    else
    {
      methodOverloads = it->second;
    }

    // Add the method to the overload.
    methodOverloads->AddMethod(method);

    return *method;
  }

  // Get the method by name.
  MethodOverloads *Data::GetMethod(const std::string &name)
  {
    auto it = m_MethodMap.find(name);

    if(it != m_MethodMap.end())
    {
      return it->second.get();
    }
    // Search the parent for the method
    else if(GetParent() != nullptr)
    {
      return GetParent()->GetMethod(name);
    }

    return nullptr;
  }

  // Add a property to the meta data.
  Property &Data::AddProperty(Property *prop)
  {
    // Check if the method already exists in the map.
    FATAL_ERROR_IF(m_PropertyMap.find(prop->GetName()) != m_PropertyMap.end(),
                   "Property already in map\nClass Name: " + GetName() +
                   "\nProperty Name: " + prop->GetName());

    auto propertyShared = std::shared_ptr<Property>(prop);

    // Add it to the property map and add it to the ordered vector so that we can
    // serialize the data in the same order we registered it in.
    m_PropertyMap.insert({prop->GetName(), propertyShared});
    m_OrderedVector.push_back(propertyShared);

    return *prop;
  }

  // Get the with the given name.
  Property *Data::GetProperty(const std::string &name)
  {
    auto it = m_PropertyMap.find(name);

    if(it != m_PropertyMap.end())
    {
      return it->second.get();
    }
    // Search the parent for the method
    else if(GetParent() != nullptr)
    {
      return GetParent()->GetProperty(name);
    }

    return nullptr;
  }

  // Add a parent to the meta data.
  void Data::AddParent(Data *parent)
  {
    FATAL_ERROR_IF(parent == this,
                   "Parenting Meta Data to itself!\nClass Name: " + GetName());
    m_Parent = parent;
  }

  // Get the parent of this meta data
  Data *Data::GetParent() const
  {
    return m_Parent;
  }

  // Check if this data has the given parent anywhere in the chain.
  bool Data::HasParent(Data *parent) const
  {
    if(GetParent() == nullptr)
    {
      return false;
    }
    else if(GetParent() == parent)
    {
      return true;
    }
    else
    {
      return GetParent()->HasParent(parent);
    }
  }

  // Get the name of the meta data.
  const std::string &Data::GetName() const
  {
    return m_Name;
  }

  // Get the name as a const char *
  const char *Data::GetNameCStr() const
  {
    return m_Name.c_str();
  }

  // Get the size of the meta data.
  size_t Data::GetSize() const
  {
    return m_Size;
  }

  // Get the object info for this meta data.
  ObjectInfoBase *Data::GetObjectInfo() const
  {
    return m_ObjectInfo.get();
  }  
}