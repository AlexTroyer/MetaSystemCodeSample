/*****************************************************************************
File:   Meta.hpp
Author: Alex Troyer
  The heart of what makes Meta work.
*****************************************************************************/
#pragma once

namespace Meta
{
  // The struct that is created by the CLASS_START macro that is specialized to
  // construct the RegisterMetaData class where the Register function is called.
  template<typename T>
  struct RegisterMetaDataStruct
  {
    static void Register()
    {}
  };

  // Constructor for RegisterMetaData which calls the RegisterMetaDataStruct Register
  // function to register the meta data.
  template<typename T>
  RegisterMetaData<T>::RegisterMetaData()
  {
    RegisterMetaDataStruct<T>::Register();
  }

  // Initialize the meta data to nullptr because this happens before constructors.
  template<typename T>
  Data *DataStorage<T>::m_Data = nullptr;

  // Register simple meta data information.
  template<typename T>
  SimpleRegisterMetaData<T>::SimpleRegisterMetaData(const std::string &name, 
                                                    size_t size, 
                                                    ObjectInfoBase *objectInfo)
  {
    BindData();
    Meta::DataStorage<T>::SetData(name, size, objectInfo);
  }

  // This method helps with keeping data referenced (I think?  I can't remember)
  template<typename T>
  void SimpleRegisterMetaData<T>::BindData()
  {
    Meta::DataStorage<T>::m_Data;
  }

  template<typename T>
  RegisterMetaData<T> DataStorage<T>::m_RegisterMetaData;

  // Get meta data.
  template<typename T>
  Data *DataStorage<T>::GetData()
  {
    return m_Data;
  }
  
  // Set some of the data information, and also store it by name.
  template<typename T>
  void DataStorage<T>::SetData(const std::string &name, 
                               size_t size, 
                               ObjectInfoBase *objectInfo)
  {
    m_Data = new Data;

    m_Data->m_Name = name;
    m_Data->m_Size = size;
    m_Data->m_ObjectInfo = std::shared_ptr<ObjectInfoBase>(objectInfo);

    NamedMetaStorage::AddMetaData(name, m_Data);
  }
}