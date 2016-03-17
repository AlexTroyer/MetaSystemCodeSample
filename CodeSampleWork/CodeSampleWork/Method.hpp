/*****************************************************************************
File:   Method.hpp
Author: Alex Troyer
  Stores method information for a class.
*****************************************************************************/
#pragma once

namespace Meta
{
  ///////////////////////////////////////////////////////////////
  // Method
  ///////////////////////////////////////////////////////////////

  // Helper to call a non-const method without having to do casting and manual creation of
  // the Any types.
  template<typename T, typename... Args>
  Any Method::Call(T &object, Args... args) const
  {
    return Call(reinterpret_cast<void *>(&object), {Any::AnyRef(args)...});
  }

  // Helper to call a const method without having to do casting and manual creation of
  // the Any types.
  template<typename T, typename... Args>
  Any Method::Call(const T &object, Args... args) const
  {
    return Call(reinterpret_cast<const void *>(&object), {Any::AnyRef(args)...});
  }

  // Helper to call a static method without having to do casting and manual creation of
  // the Any types.
  template<typename ...Args>
  Any Method::CallStatic(Args ...args) const
  {
    return CallStatic({Any::AnyRef(args)...});
  }
  
  // Helper structs in order to expand a list of numbers that expands from 0 to N.
  namespace VectorUnpack
  {
    template<size_t ...N>
    struct indicies
    {
      typedef indicies<N..., sizeof...(N)> next;
    };

    template<typename size_t N>
    struct make_indicies
    {
      typedef typename make_indicies<N - 1>::type::next type;
    };

    template<>
    struct make_indicies<0>
    {
      typedef indicies<> type;
    };
  }

  // Get the meta data information of all the types in the variadic template.
  template<typename ...Args>
  void CaptureArgumentMeta(std::vector<Meta::Data *> &argumentMeta)
  {
    argumentMeta.assign({GET_META(Args)...});
  }

  ///////////////////////////////////////////////////////////////
  // MethodOverloads
  ///////////////////////////////////////////////////////////////

  // Helper to call a non-const method without having to do casting and manual creation of
  // the Any types.
  template<typename T, typename ...Args>
  Any MethodOverloads::Call(T &object, Args... args) const
  {
    return Call(reinterpret_cast<void *>(&object), {Any::AnyRef(args)...});
  }

  // Helper to call a const method without having to do casting and manual creation of
  // the Any types.
  template<typename T, typename ...Args>
  Any MethodOverloads::Call(const T &object, Args... args) const
  {
    return Call(reinterpret_cast<const void *>(&object), {Any::AnyRef(args)...});
  }

  // Helper to call a static method without having to do casting and manual creation of
  // the Any types.
  template<typename ...Args>
  Any MethodOverloads::CallStatic(Args... args) const
  {
    return CallStatic({Any::AnyRef(args)...});
  }

  ///////////////////////////////////////////////////////////////
  // Method_T
  ///////////////////////////////////////////////////////////////

  // Helper to call a non-const method.  This helps unpack all the Anys in the passed in
  // vector.
  template<typename Class, typename Return, typename ...Args, size_t ...N>
  Any CallHelper(Class &object, std::function<Return(Class &, Args...)> func, 
                 const std::vector<Any> &args, VectorUnpack::indicies<N...>)
  {
    return func(object, args[N]...);
  }

  // Constructor for a method.
  template<typename Class, typename Return, typename ...Args>
  Method_T<Class, Return, Args...>::Method_T(const std::string &name, 
                                             std::function<Return(Class &, Args...)> func)
    : Method(name, GET_META(Return), sizeof...(Args), false, false)
    , m_Function(func)
  {
    CaptureArgumentMeta<Args...>(m_ArgumentMeta);
  }

  // Call function for a method.
  template<typename Class, typename Return, typename ...Args>
  Any Method_T<Class, Return, Args...>::Call(void *object, const std::vector<Any> &args) const
  {
    // Call the helper which helps pass all the arguments in order.
    return CallHelper(*reinterpret_cast<Class *>(object), m_Function, args, 
                      typename VectorUnpack::make_indicies<sizeof...(Args)>::type());
  }

  ///////////////////////////////////////////////////////////////
  // Method_const_T
  ///////////////////////////////////////////////////////////////

  // Helper to call a const method which unpacks all the arguments in the vector.
  template<typename Class, typename Return, typename ...Args, size_t ...N>
  Any CallHelperConst(const Class &object, std::function<Return(const Class &, Args...)> func, 
                      const std::vector<Any> &args, VectorUnpack::indicies<N...>)
  {
    return func(object, args[N]...);
  }

  // Constructor for a const method.
  template<typename Class, typename Return, typename ...Args>
  Method_const_T<Class, Return, Args...>::Method_const_T(const std::string &name, std::function<Return(const Class &, Args...)> func)
    : Method(name, GET_META(Return), sizeof...(Args), false, true)
    , m_Function(func)
  {
    CaptureArgumentMeta<Args...>(m_ArgumentMeta);
  }

  // Calling a const method with a non-const object.
  template<typename Class, typename Return, typename ...Args>
  Any Method_const_T<Class, Return, Args...>::Call(void *object, const std::vector<Any> &args) const
  {
    return Call(static_cast<const void *>(object), args);
  }

  // Call a const method.
  template<typename Class, typename Return, typename ...Args>
  Any Method_const_T<Class, Return, Args...>::Call(const void *object, const std::vector<Any> &args) const
  {
    // Call the helper which helps pass all the arguments in order.
    return CallHelperConst(*reinterpret_cast<const Class *>(object), m_Function, args, 
                           typename VectorUnpack::make_indicies<sizeof...(Args)>::type());
  }

  ///////////////////////////////////////////////////////////////
  // Method_static_T
  ///////////////////////////////////////////////////////////////

  // Helper for a static method to unpack the arguments in the vector in order.
  template<typename Return, typename ...Args, size_t ...N>
  Any CallHelperStatic(std::function<Return(Args...)> func, const std::vector<Any> &args, 
                       VectorUnpack::indicies<N...>)
  {
    return func(args[N]...);
  }

  // Constructor for a static method.
  template<typename Return, typename ...Args>
  Method_static_T<Return, Args...>::Method_static_T(const std::string &name, std::function<Return(Args...)> func)
    : Method(name, GET_META(Return), sizeof...(Args), true, false)
    , m_Function(func)
  {
    CaptureArgumentMeta<Args...>(m_ArgumentMeta);
  }

  // Call the static method.
  template<typename Return, typename ...Args>
  Any Method_static_T<Return, Args...>::CallStatic(const std::vector<Any>& args) const
  {
    // Call the helper which helps pass all the arguments in order.
    return CallHelperStatic(m_Function, args, 
                            typename VectorUnpack::make_indicies<sizeof...(Args)>::type());
  }

  ///////////////////////////////////////
  // Method Creation
  ///////////////////////////////////////

  // Create a non-const method.
  template<typename Class, typename Return, typename ...Args>
  Method *CreateMethod(const std::string &name, Return (Class::*func)(Args...))
  {
    auto function = [func](Class &object, Args... args)
    {
      return (object.*func)(args...);
    };

    return new Method_T<Class, Return, Args...>(name, function);
  }

  // Create a const method.
  template<typename Class, typename Return, typename ...Args>
  Method *CreateMethod(const std::string &name, Return (Class::*func)(Args...) const)
  {
    auto function = [func](const Class &object, Args... args)
    {
      return (object.*func)(args...);
    };

    return new Method_const_T<Class, Return, Args...>(name, function);
  }

  // Create a static method.
  template<typename Class, typename Return, typename ...Args>
  Method *CreateMethod(const std::string &name, Return (*func)(Args...))
  {
    auto function = [func](Args... args)
    {
      return func(args...);
    };

    return new Method_static_T<Return, Args...>(name, function);
  }
}