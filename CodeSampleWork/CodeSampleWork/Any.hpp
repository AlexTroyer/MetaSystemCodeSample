/*****************************************************************************
File:   Any.hpp
Author: Alex Troyer
  The Any type is made to hold any type.
*****************************************************************************/
#pragma once

// Construct an Any by copying the given object.
template<typename T>
Any::Any(const T &rhs)
  : m_MetaData(GET_META(T))
{
  m_Data = m_MetaData->GetObjectInfo()->Copy(&rhs);

  FATAL_ERROR_IF(m_Data == nullptr, m_MetaData->GetName() + " doesn't have a copy constructor!");
}

// Create a reference to an object and store it in the Any.
// The object is not owned by the Any.
template<typename T>
Any Any::AnyRef(T &rhs)
{
  Any any;
  any.SetReference(rhs);
  return any;
}

// Create a refence to a const object and store it in the Any.
// The object is not owned by the Any and tries to be const safe
// (It's up to the user if this is used in a const safe way).
template<typename T>
Any Any::AnyRef(const T &rhs)
{
  Any any;
  any.SetReference(rhs);
  return any;
}

// Conversion operator to any type that is needed.
template<typename T>
Any::operator T&() const
{
  return Get<T>();
}

// Assignment operator from another type by copying the data.
template<typename T>
T &Any::operator=(const T &rhs)
{
  // We can't assign to the data if this is const.
  if(!m_IsConst)
  {
    // The types are the same, use the assignment operator of the type.
    if(m_MetaData == GET_META(T))
    {
      return *reinterpret_cast<T *>(m_Data) = rhs;
    }
    // Otherwise we need to destroy the data since we are changing types.
    // Don't destroy the data if we have no type information or we are holding a
    // reference.
    else if(m_MetaData && !m_HoldsReference)
    {
      m_MetaData->GetObjectInfo()->Destroy(m_Data);
    }
  }

  m_MetaData = GET_META(T);
  // Make a copy of the data.
  m_Data = m_MetaData->GetObjectInfo()->Copy(&rhs);
  m_HoldsReference = false;
  m_IsConst = false;

  // Error if the class doesn't have a copy constructor.
  FATAL_ERROR_IF(m_Data == nullptr, m_MetaData->GetName() + " doesn't have a copy constructor!");

  return *reinterpret_cast<T *>(m_Data);
}

// Calls the assignment operator for assigning an object to an Any.
template<typename T>
void Any::Set(const T &rhs)
{
  *this = rhs;
}

// Set a reference to the object.  A reference will not be destroyed by the Any.
template<typename T>
void Any::SetReference(T &rhs)
{
  m_HoldsReference = true;
  m_MetaData = GET_META(T);
  m_Data = reinterpret_cast<void *>(&rhs);
}

// Set a reference to a const object. This will not be destroyed or assigned over,
// but this could be a bit dangerous.  Be careful when using this.
template<typename T>
void Any::SetReference(const T &rhs)
{
  SetReference(const_cast<T &>(rhs));
  m_IsConst = false;
}

// Get the data casted as the given type.
template<typename T>
T &Any::Get() const
{
  return *reinterpret_cast<T *>(m_Data);
}