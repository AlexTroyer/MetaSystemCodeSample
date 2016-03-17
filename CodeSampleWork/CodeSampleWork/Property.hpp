/*****************************************************************************
File:   Property.hpp
Author: Alex Troyer
  Holds information on a property of a class.
*****************************************************************************/
#pragma once

// Unreferenced function has been removed //
// Some of these functions will be generated but never used. We have to deal with it. //
#pragma warning(push)
#pragma warning(disable: 4505)

namespace Meta
{
  ///////////////////////////////////////////////////////////////
  // Property
  ///////////////////////////////////////////////////////////////

  // Get the data from this property.
  template<typename T>
  Any Property::Get(const T &object)
  {
    return Get(reinterpret_cast<const void *>(&object));
  }

  // Set data on this property.
  template<typename T, typename U>
  void Property::Set(T &object, const U &rhs)
  {
    Set(reinterpret_cast<void *>(&object), reinterpret_cast<const void *>(&rhs));
  }

  // Set the data on a static property.
  template<typename T>
  void Property::Set(const T &rhs)
  {
    Set(reinterpret_cast<const void *>(&rhs));
  }

  ///////////////////////////////////////////////////////////////
  // Property_T
  ///////////////////////////////////////////////////////////////

  // Constructor for Property.
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_T<Class, GetReturn, SetParameter>::Property_T(const std::string &name,
                                                         GetFn get, SetFn set)
    : Property(name, GET_META(Class), false)
    , m_Get(get)
    , m_Set(set)
  {
  }

  // Get the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  Any Property_T<Class, GetReturn, SetParameter>::Get(const void *object)
  {
    Any any(m_Get(*reinterpret_cast<const GET_TYPE(Class) *>(object)));

    return any;
  }

  // Set the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  void Property_T<Class, GetReturn, SetParameter>::Set(void *object, const void *rhs)
  {
    m_Set(*reinterpret_cast<GET_TYPE(Class) *>(object), 
          *reinterpret_cast<const GET_TYPE(SetParameter) *>(rhs));
  }

  // Serialize the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  void Property_T<Class, GetReturn, SetParameter>::Serialize(const void *object, Util::Serializer &stream)
  {
    Util::Write(stream, m_Get(*reinterpret_cast<const Class *>(object)));
  }

  // Deserialize the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  void Property_T<Class, GetReturn, SetParameter>::Deserialize(void *object, Util::Deserializer &stream)
  {
    GET_TYPE(SetParameter) readValue;
    Util::Read(stream, readValue);
    Set(object, &readValue);
  }

  // Compare two different objects and see if the properties are the same.
  template<typename Class, typename GetReturn, typename SetParameter>
  bool Property_T<Class, GetReturn, SetParameter>::Compare(const void *lhs, const void *rhs)
  {
    const Class *class1 = reinterpret_cast<const Class *>(lhs);
    const Class *class2 = reinterpret_cast<const Class *>(rhs);
  
    return m_Get(*class1) == m_Get(*class2);
  }

  // Assign one property from another.
  template<typename Class, typename GetReturn, typename SetParameter>
  void Property_T<Class, GetReturn, SetParameter>::Assignment(void *lhs, const void *rhs)
  {
    Class *class1 = reinterpret_cast<Class *>(lhs);
    const Class *class2 = reinterpret_cast<const Class *>(rhs);

    m_Set(*class1, m_Get(*class2));
  }

  ///////////////////////////////////////////////////////////////
  // Property_static_T
  ///////////////////////////////////////////////////////////////

  // Construct a static property
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_static_T<Class, GetReturn, SetParameter>::Property_static_T(const std::string &name, 
                                                                       GetFn get, 
                                                                       SetFn set)
    : Property(name, GET_META(Class), true)
    , m_Get(get)
    , m_Set(set)
  {
  }

  // Get the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  Any Property_static_T<Class, GetReturn, SetParameter>::Get()
  {
    Any any(m_Get());

    return any;
  }

  // Set the property.
  template<typename Class, typename GetReturn, typename SetParameter>
  void Property_static_T<Class, GetReturn, SetParameter>::Set(const void *rhs)
  {
    m_Set(*reinterpret_cast<const GET_TYPE(SetParameter) *>(rhs));
  }

  ///////////////////////////////////////////////////////////////
  // Property Creation
  ///////////////////////////////////////////////////////////////

  // Create a property with a get and set as a std::function
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_T<Class, GetReturn, SetParameter> *CreateProperty(const std::string &name,
                                                             typename Property_T<Class, GetReturn, SetParameter>::GetFn get,
                                                             typename Property_T<Class, GetReturn, SetParameter>::SetFn set)
  {
    return new Property_T<Class, GetReturn, SetParameter>(name, get, set);
  }

  // Create a property with only a get.
  template<typename Class, typename GetReturn>
  Property_T<Class, GetReturn, const GetReturn &> *CreateProperty(const std::string &name,
                                                             typename Property_T<Class, GetReturn, const GetReturn &>::GetFn get)
  {
    auto setFn = [](Class &, const GetReturn &)
    {
    };

    return new Property_T<Class, GetReturn, const GetReturn &>(name, get, setFn);
  }

  // Create a property from method pointers.
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_T<Class, GetReturn, SetParameter> *CreateProperty(const std::string &name,
                                                             GetReturn (Class::*get)() const,
                                                             void (Class::*set)(const SetParameter))
  {
    // Wrap the get and set methods into std::functions

    Property_T<Class, GetReturn, SetParameter>::GetFn getFn = 
      [get](const Class &c)
    {
      return (c.*get)();
    };
  
    Property_T<Class, GetReturn, SetParameter>::SetFn setFn = 
      [set](Class &c, const SetParameter rhs)
    {
      (c.*set)(rhs);
    };
  
    return new Property_T<Class, GetReturn, SetParameter>(name, getFn, setFn);
  }

  // Creates a property from method pointers with only a get method.
  template<typename Class, typename GetReturn>
  Property_T<Class, GetReturn, typename const GET_TYPE(GetReturn) &> *CreateProperty(const std::string &name,
                                                             GetReturn(Class::*get)() const)
  {
    // Create the get function.
    Property_T<Class, GetReturn, const GET_TYPE(GetReturn) &>::GetFn getFn =
      [get](const Class &c)
    {
      return (c.*get)();
    };

    // Create an empty set function.
    Property_T<Class, GetReturn, const GET_TYPE(GetReturn) &>::SetFn setFn =
      [](Class &, typename const GET_TYPE(GetReturn) &)
    {
    };

    return new Property_T<Class, GetReturn, const GET_TYPE(GetReturn) &>(name, getFn, setFn);
  }

  // This will generate a set function that does nothing if the member is const.
  template<typename Class, typename MemberType>
  std::function<void (Class &, const MemberType &)> 
    GetSetFunc(MemberType Class::*,
               typename std::enable_if<std::is_const<MemberType>::value>::type * = nullptr)
  {
    return [](Class &, const MemberType &){};
  }

  // This will generate a set function for a member.
  template<typename Class, typename MemberType>
  std::function<void(Class &, const MemberType &)>
    GetSetFunc(MemberType Class::*member,
               typename std::enable_if<!std::is_const<MemberType>::value>::type * = nullptr)
  {
    return [member](Class &lhs, const MemberType &rhs) 
    {
      (lhs.*member) = rhs;
    };
  }

  // Create a property from a member pointer.
  template<typename Class, typename MemberType>
  Property_T<Class, MemberType, const MemberType &> *CreateProperty(const std::string &name,
                                                                    MemberType Class::*member)
  {
    auto getFn = [member](const Class &c)
    {
      return (c.*member);
    };

    // If the member is const, we will get an empty set function.
    auto setFn = GetSetFunc<Class, MemberType>(member);

    return new Property_T<Class, MemberType, const MemberType &>(name, getFn, setFn);
  }

  // Create a static property using std::function.
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_static_T<Class, GetReturn, SetParameter> *CreateProperty(const std::string &name,
                                                                    typename Property_static_T<Class, GetReturn, SetParameter>::GetFn get,
                                                                    typename Property_static_T<Class, GetReturn, SetParameter>::SetFn set)
  {
    return new Property_static_T<Class, GetReturn, SetParameter>(name, get, set);
  }

  // Create a static property using std::function with only a get function.
  template<typename Class, typename GetReturn>
  Property_static_T<Class, GetReturn, typename const GET_TYPE(GetReturn) &> *CreateProperty(const std::string &name,
                                                                    typename Property_static_T<Class, GetReturn, typename const GET_TYPE(GetReturn) &>::GetFn get)
  {
    auto setFn = [](const GET_TYPE(GetReturn) &)
    {
    };

    return new Property_static_T<Class, GetReturn, typename const GET_TYPE(GetReturn) &>(name, get, set);
  }

  // Get a get and set from a static method (or a global function would work here too).
  template<typename Class, typename GetReturn, typename SetParameter>
  Property_static_T<Class, GetReturn, SetParameter> *CreateProperty(const std::string &name,
                                                                    GetReturn (*get)(),
                                                                    void (*set)(const SetParameter))
  {
    auto getFn = [get]()
    {
      return get();
    };

    auto setFn = [set](const SetParameter rhs)
    {
      set(rhs);
    };

    return new Property_static_T<Class, GetReturn, SetParameter>(name, getFn, setFn);
  }

  // Create a static property with only a get from a static method or global function.
  template<typename Class, typename GetReturn>
  Property_static_T<Class, GetReturn, typename const GET_TYPE(GetReturn) &> *CreateProperty(const std::string &name,
                                                                                            GetReturn(*get)())
  {
    auto getFn = [get]()
    {
      return get();
    };

    auto setFn = [](const GET_TYPE(GetReturn) &)
    {
    };

    return new Property_static_T<Class, GetReturn, const GET_TYPE(GetReturn) &>(name, getFn, setFn);
  }

  // If the member is const, return an empty set function.
  template<typename MemberType>
  std::function<void(const MemberType &rhs)> GetSetFuncStatic(MemberType *,
                                                              typename std::enable_if<std::is_const<MemberType>::value>::type * = nullptr)
  {
    return [](const MemberType &) {};
  }

  // If the member is not const, return a set function.
  template<typename MemberType>
  std::function<void(const MemberType &rhs)> GetSetFuncStatic(MemberType *member,
                                                              typename std::enable_if<!std::is_const<MemberType>::value>::type * = nullptr)
  {
    return [member](const MemberType &rhs) 
    {
      *member = rhs;
    };
  }

  // Create a static property from a member pointer.
  template<typename Class, typename MemberType>
  Property_static_T<Class, MemberType, const MemberType &> *CreateProperty(const std::string &name,
                                                                           MemberType *member)
  {
    auto getFn = [member]()
    {
      return *member;
    };

    // Get the set function for this member.  If it is a const member, the set function
    // will be empty.
    auto setFn = GetSetFuncStatic<MemberType>(member);

    return new Property_static_T<Class, MemberType, const MemberType &>(name, getFn, setFn);
  }
}

#pragma warning(pop)