/*****************************************************************************
File:   Any.cpp
Author: Alex Troyer
  The Any type is made to hold any type.
*****************************************************************************/
#include "Any.h"
#include "Meta.h"

// Default constructs a type based off of the meta data given.
Any::Any(Meta::Data *metaData)
  : m_MetaData(metaData)
{
  m_Data = m_MetaData->GetObjectInfo()->Construct();

  // It is possible for a class to not have a default constructor
  FATAL_ERROR_IF(m_Data == nullptr, metaData->GetName() + " doesn't have a default constructor!");
}

// Copy constructor for Any.
Any::Any(const Any &any)
  : m_MetaData(any.m_MetaData)
{
  if(m_MetaData)
  {
    m_Data = m_MetaData->GetObjectInfo()->Copy(any.GetInternal());

    FATAL_ERROR_IF(m_Data == nullptr, m_MetaData->GetName() + " doesn't have a copy constructor!");
  }
}

// Move constructor for Any.
// Take all the data from the other Any and put it in this one.
Any::Any(Any &&any)
  : m_MetaData(any.m_MetaData)
  , m_Data(any.m_Data)
  , m_HoldsReference(any.m_HoldsReference)
{
  any.m_MetaData = nullptr;
  any.m_Data = nullptr;
  any.m_HoldsReference = false;
}

// Destroys any data that the Any created.
// Will not destroy the given data if it is holding a reference or if the data
// is const.
Any::~Any()
{
  if(!m_HoldsReference && m_MetaData && !m_IsConst)
  {
    m_MetaData->GetObjectInfo()->Destroy(m_Data);
  }
}

// Assignment operator from another Any.  Will copy the data from the other any.
Any &Any::operator=(const Any &rhs)
{
  m_HoldsReference = false;

  m_MetaData = rhs.m_MetaData;
  m_Data = m_MetaData->GetObjectInfo()->Copy(rhs.m_Data);
  m_HoldsReference = false;

  FATAL_ERROR_IF(m_Data == nullptr, m_MetaData->GetName() + " doesn't have a copy constructor!");

  return *this;
}

// Move assignment operator from another Any.  Takes the data from the other Any without
// copy.
Any &Any::operator=(Any &&rhs)
{
  m_MetaData = rhs.m_MetaData;
  rhs.m_MetaData = nullptr;

  m_Data = rhs.m_Data;
  rhs.m_Data = nullptr;

  m_HoldsReference = rhs.m_HoldsReference;
  rhs.m_HoldsReference = false;

  return *this;
}

// Get the internal data if needed for something.
void *Any::GetInternal() const
{
  return m_Data;
}

// Get the Meta data information for the type in this Any.
Meta::Data *Any::GetMeta() const
{
  return m_MetaData;
}

// See if the data in the Any is const.
bool Any::IsConst() const
{
  return m_IsConst;
}

// See if the Any is holding a reference.
bool Any::HoldsReference() const
{
  return m_IsConst;
}
