/*****************************************************************************
File:   Property.h
Author: Alex Troyer
  Holds information on a property of a class.
*****************************************************************************/
#pragma once

#include "DataInfo.h"
#include "Any.h"
#include <type_traits>
#include <functional>
#include "Meta.h"
#include "Serializer.h"
#include "Deserializer.h"

namespace Util
{
  class Serializer;
  class Deserializer;
}

// Register a member to the class with a given name.
#define MEMBER_NAME(name, member) \
data->AddProperty(Meta::CreateProperty<T, decltype(T::member)>(name, &T::member))

// Register a member to the class.
#define MEMBER(member) \
MEMBER_NAME(#member, member)

// Macro helper to create a property with only a get.
#define PROPERTY_GET(name, get) \
data->AddProperty(Meta::CreateProperty<T>(name, &T::get))

// Macro helper to create a property with a get and set.
#define PROPERTY_GET_SET(name, get, set) \
data->AddProperty(Meta::CreateProperty<T>(name, &T::get, &T::set))

// This is an "overloaded" macro.
// The two overloads are:
// Property with get only
// PROPERTY("PropertyName", GetFunction);
// Property with set only.
// PROPERTY("PropertyName2", GetFunction, SetFunction); 
#define GET_PROPERTY_MACRO(_1,_2,_3,NAME,...) NAME
#define PROPERTY(...) EXPAND(GET_PROPERTY_MACRO(__VA_ARGS__, PROPERTY_GET_SET, PROPERTY_GET)(__VA_ARGS__))

namespace Meta
{
  // Property of a class.
  class Property : public DataInfo
  {
  public:
    Property(const std::string &name, Meta::Data *metaData, bool isStatic);

    virtual Any Get(const void *object);
    virtual Any Get();

    template<typename T>
    Any Get(const T &object);

    virtual void Set(void *object, const void *rhs);
    virtual void Set(const void *rhs);

    template<typename T, typename U>
    void Set(T &object, const U &rhs);

    template<typename T>
    void Set(const T &rhs);

    virtual bool Compare(const void *, const void *);
    virtual void Assignment(void *, const void *);
  };

  // Property of a class with a get and set, or just a get.
  template<typename Class, typename GetReturn, typename SetParameter>
  class Property_T : public Property
  {
  public:
    typedef std::function<GetReturn(const Class &)> GetFn;
    typedef std::function<void(Class &, const SetParameter)> SetFn;

    Property_T(const std::string &name, GetFn get, SetFn set);

    virtual Any Get(const void *object);
    virtual void Set(void *object, const void *rhs);

    virtual void Serialize(const void *object, Util::Serializer &stream);
    virtual void Deserialize(void *object, Util::Deserializer &stream);

    virtual bool Compare(const void *lhs, const void *rhs);
    virtual void Assignment(void *lhs, const void *rhs);

  private:
    GetFn m_Get = nullptr;
    SetFn m_Set = nullptr;
  };

  // Property of a class that is static with a get and set, or just a get.
  template<typename Class, typename GetReturn, typename SetParameter>
  class Property_static_T : public Property
  {
  public:
    typedef std::function<GetReturn()> GetFn;
    typedef std::function<void(const SetParameter)> SetFn;

    Property_static_T(const std::string &name, GetFn get, SetFn set);

    virtual Any Get();
    virtual void Set(const void *rhs); 

  private:
    GetFn m_Get = nullptr;
    SetFn m_Set = nullptr;
  }; 
}

#include "Property.hpp"