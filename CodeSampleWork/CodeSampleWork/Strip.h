/*****************************************************************************
File:   Strip.h
Author: Alex Troyer
  Strips off any type information like references, pointers, constness to get
  the type.
*****************************************************************************/
#pragma once

#define GET_TYPE(x) Meta::Strip<x>::type

namespace Meta
{
  // Get the type of the object by stripping off references, pointers, etc...
  template<typename T>
  struct Strip
  {
    typedef T type;
  };

  template<typename T>
  struct Strip <const T>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <T *>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <const T *>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <T &>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <T &&>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <T[]>
  {
    typedef typename Strip<T>::type type;
  };

  template<typename T>
  struct Strip <const T[]>
  {
    typedef typename Strip<T>::type type;
  };
}