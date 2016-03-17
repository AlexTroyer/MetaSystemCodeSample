/*****************************************************************************
File:   Meta.h
Author: Alex Troyer
  The heart of what makes Meta work.
*****************************************************************************/
#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include "Strip.h"
#include "ObjectInfo.h"
#include "Macros.h"

// Get meta information based off of type.
#define GET_META(type) Meta::DataStorage<GET_TYPE(type)>::GetData()
// Get meta information from the variable or expression you pass in
// (This calls "GET_META" with decltype around the argument passed in)
#define GET_META_VAR(var) GET_META(decltype(var))
// Get meta data by string name.
#define GET_META_NAME(name) Meta::NamedMetaStorage::GetMeta(name)

// Define a simple type, like an int, that doesn't need any methods or properties bound.
#define DEFINE_SIMPLE_TYPE_NAME(name, type) \
Meta::SimpleRegisterMetaData<type> Meta::SimpleRegister<type>::m_Register(name, sizeof(type), new Meta::ObjectInfo<type>())

// Define a simple type, like an int, that doesn't need any methods or properties bound.
#define DEFINE_SIMPLE_TYPE(type) \
DEFINE_SIMPLE_TYPE_NAME(#type, type)

// Gives private member access to meta registration. This would be for stuff like 
// members you don't want to make a getter or setter for
#define META_PRIVATE_ACCESS \
template<typename> \
friend struct RegisterMetaDataStruct

// Start the class registration with a different name than the type.
#define CLASS_START_NAME(name, type) \
struct \
{ \
  void ForceStaticLink() \
  { \
    Meta::DataStorage<type>::m_Data; \
    Meta::DataStorage<type>::m_RegisterMetaData; \
  } \
} static TOKENPASTE2(Register, __COUNTER__); \
__pragma(warning(push)) \
__pragma(warning(disable : 4100)) \
__pragma(warning(push))\
__pragma(warning(disable : 4189)) \
template<>\
struct Meta::RegisterMetaDataStruct<type>\
{ \
  static void Register() \
  { \
    typedef type T; \
    Meta::DataStorage<T>::SetData(name, sizeof(type), new Meta::ObjectInfo<T>()); \
    Meta::Data *data = GET_META(T);

// Start the class registration
#define CLASS_START(type) \
CLASS_START_NAME(#type, type)

// End the class registration.
#define CLASS_END \
__pragma(warning(pop))__pragma(warning(pop))}}


namespace Util
{
  class Serializer;
}

namespace Meta
{
  class Method;
  class MethodOverloads;
  class Property;
  class ObjectInfoBase;
  class DataInfo;

  class Data;

  typedef std::unordered_map<std::string, std::shared_ptr<MethodOverloads>> MethodMap;
  typedef std::unordered_map<std::string, std::shared_ptr<Property>> PropertyMap;
  typedef std::vector<std::shared_ptr<DataInfo>> OrderedVector;

  // Registers meta data when constructed.
  template<typename T>
  class RegisterMetaData
  {
  public:
    RegisterMetaData();
  };

  // This registers meta data for simple types.
  template<typename T>
  class SimpleRegisterMetaData
  {
  public:
    SimpleRegisterMetaData(const std::string &name, size_t size, ObjectInfoBase *objectInfo);  
  
  private:
    void BindData();
  };

  // This is the object that holds the object that will be constructed to do the 
  // simple meta register.
  template<typename T>
  class SimpleRegister
  {
  private:
    static SimpleRegisterMetaData<T> m_Register;
  };

  // Holds the meta data information.  You get the different type information
  // based off of the template parameter.
  template<typename T>
  class DataStorage
  {
  public:
    static Data *GetData();
    static void SetData(const std::string &name, size_t size, ObjectInfoBase *objectInfo);

    static Data *m_Data;
    static RegisterMetaData<T> m_RegisterMetaData;
  };

  // Is used to store all the meta data registered by string name.
  class NamedMetaStorage
  {
  public:
    template<typename>
    friend class DataStorage;

    static Data *GetMeta(const std::string &name);

  private:
    static void AddMetaData(const std::string &name, Data *meta);
    static std::unordered_map<std::string, Data *> *m_MetaMap;
  };

  // This holds all the meta information for a type.
  class Data
  {
  public:
    template<typename>
    friend class DataStorage;

    Data() = default;
    ~Data() = default;

    const OrderedVector &GetOrderedData() const;
    const MethodMap &GetMethodMap() const;
    const PropertyMap &GetPropertyMap() const;

    Method &AddMethod(Method *method);
    MethodOverloads *GetMethod(const std::string &name);

    Property &AddProperty(Property *prop);
    Property *GetProperty(const std::string &name);

    void AddParent(Data *parent);
    Data *GetParent() const;
    bool HasParent(Data *parent) const;

    const std::string &GetName() const;
    const char *GetNameCStr() const;
    size_t GetSize() const;

    ObjectInfoBase *GetObjectInfo() const;

  private:
    // This holds properties in order registered for serialization.
    OrderedVector m_OrderedVector;
    // Holds all the methods and their overloads.
    MethodMap m_MethodMap;
    // Holds all the properties.
    PropertyMap m_PropertyMap;
    std::string m_Name;
    size_t m_Size;
    std::shared_ptr<ObjectInfoBase> m_ObjectInfo;
    Data *m_Parent = nullptr;
  };
}

#include "Meta.hpp"