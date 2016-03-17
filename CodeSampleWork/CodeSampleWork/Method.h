/*****************************************************************************
File:   Method.h
Author: Alex Troyer
  Stores method information for a class.
*****************************************************************************/
#pragma once

#include "DataInfo.h"
#include "Any.h"
#include <vector>
#include <functional>
#include <type_traits>
#include "Macros.h"

// If a method you are registering is overloaded, you must use
// "METHOD_NAME_OVERLOAD", "METHOD_OVERLOAD", or "METHODS" to specify what methods you
// want to bind. Otherwise the compiler doesn't know which overload to choose.

// Adds a method with a name with the given type.
#define METHOD_NAME_OVERLOAD(name, type, method) \
data->AddMethod(Meta::CreateMethod<T>(name, static_cast<type>(&T::method)))

// Adds a method with the given name.
#define METHOD_NAME(name, method) \
METHOD_NAME_OVERLOAD(name, decltype(&T::method), method)

// Adds a method with the given method signature.
#define METHOD_OVERLOAD(type, method) \
METHOD_NAME_OVERLOAD(#method, type, method)

// Adds the given method.
#define METHOD(method) \
METHOD_NAME_OVERLOAD(#method, decltype(&T::method), method)

// Helper macro to expand the "METHODS" macro.
#define FOR_ALL_OVERLOADS(method, type) \
METHOD_OVERLOAD(type, method);

// Used to add multiple methods with overloads.  If you want to specify a name,
// you have to use multiple "METHOD_NAME_OVERLOAD" macros.
// Example of using:
// METHODS(MyMethod, void (T::*)(int),
//                   void (T::*)(float) const,
//                   void (T::*)(int, int));
#define METHODS(method, ...) \
PASTE_PARAMS(FOR_ALL_OVERLOADS, method, __VA_ARGS__) struct forcesemicolon

namespace Meta
{
  // Base class for a method.
  // The method "GetMetaData" is the return value.
  class Method : public DataInfo
  {
  public:
    Method(const std::string &name, Meta::Data *returnData, size_t argNum, bool isStatic, bool isConst);

    bool IsConst() const;
    size_t GetArgNum() const;
    const std::vector<Meta::Data *> &GetArguments() const;

    virtual Any Call(void *object, const std::vector<Any> &args = {}) const;
    virtual Any Call(const void *object, const std::vector<Any> &args = {}) const;
    virtual Any CallStatic(const std::vector<Any> &args = {}) const;

    template<typename T, typename ...Args>
    Any Call(T &object, Args... args) const;
    template<typename T, typename ...Args>
    Any Call(const T &object, Args... args) const;
    template<typename... Args>
    Any CallStatic(Args... args) const;

  protected:
    std::vector<Meta::Data *> m_ArgumentMeta;

  private:
    size_t m_ArgNum;
    bool m_IsConst;
  };

  // Holds methods and their overloads.
  class MethodOverloads
  {
  public:
    friend class Data;

    bool IsStatic() const;

    Any Call(void *object, const std::vector<Any> &args = {}) const;
    Any Call(const void *object, const std::vector<Any> &args = {}) const;
    Any CallStatic(const std::vector<Any> &args = {}) const;

    template<typename T, typename ...Args>
    Any Call(T &object, Args... args) const;
    template<typename T, typename ...Args>
    Any Call(const T &object, Args... args) const;
    template<typename ...Args>
    Any CallStatic(Args... args) const;

  private:
    void AddMethod(Method *method);
    Data *m_Owner = nullptr;

    bool m_IsStatic = false;

    std::vector<std::shared_ptr<Method>> m_Methods;
  };

  // Non const non static method.
  template<typename Class, typename Return, typename ...Args>
  class Method_T : public Method
  {
  public:
    Method_T(const std::string &name, std::function<Return (Class &, Args...)> func);

    virtual Any Call(void *object, const std::vector<Any> &args = {}) const;

  private:

    std::function<Return (Class &, Args...)> m_Function;
  };

  // Const method.
  template<typename Class, typename Return, typename ...Args>
  class Method_const_T : public Method
  {
  public:
    Method_const_T(const std::string &name, std::function<Return(const Class &, Args...)> func);

    virtual Any Call(void *object, const std::vector<Any> &args = {}) const;
    virtual Any Call(const void *object, const std::vector<Any> &args = {}) const;

  private:

    std::function<Return(const Class &, Args...)> m_Function;
  };

  // Static method.
  template<typename Return, typename ...Args>
  class Method_static_T : public Method
  {
  public:
    Method_static_T(const std::string &name, std::function<Return(Args...)> func);

    virtual Any CallStatic(const std::vector<Any> &args = {}) const;

  private:

    std::function<Return (Args...)> m_Function;
  };
}

#include "Method.hpp"