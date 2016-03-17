/*****************************************************************************
File:   Any.h
Author: Alex Troyer
  The Any type is made to hold any type.
*****************************************************************************/
#pragma once

#include "Meta.h"
#include "Error.h"

class Any
{
public:
  Any() = default;
  Any(Meta::Data *metaData);
  Any(const Any &any);
  Any(Any &&any);

  template<typename T>
  Any(const T &rhs);

  template<typename T>
  static Any AnyRef(T &rhs);

  template<typename T>
  static Any AnyRef(const T &rhs);

  ~Any();

  template<typename T>
  operator T&() const;

  template<typename T>
  T &operator=(const T &rhs);

  Any &operator=(const Any &rhs);

  Any &operator=(Any &&rhs);

  template<typename T>
  void Set(const T &rhs);

  template<typename T>
  void SetReference(T &rhs);
  template<typename T>
  void SetReference(const T &rhs);

  template<typename T>
  T &Get() const;

  void *GetInternal() const;
  Meta::Data *GetMeta() const;
  bool IsConst() const;
  bool HoldsReference() const;

private:
  void *m_Data = nullptr;
  Meta::Data *m_MetaData = nullptr;
  bool m_HoldsReference = false;
  bool m_IsConst = false;
};

#include "Any.hpp"
