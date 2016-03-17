/*****************************************************************************
File:   Serializer.hpp
Author: Alex Troyer
  Takes information and writes it to a file using the meta system.
*****************************************************************************/
#pragma once

namespace Util
{
  // Writes a given object to a file using the meta system.
  template<typename T>
  void Serializer::Write(const T &object)
  {
    const Meta::Data *meta = GET_META(T);
    
    InsertTabs();
    WriteString(meta->GetName());
    InsertNewline();

    WriteString("{"); 
    InsertNewline();

    IncrementTabs();

    const Meta::OrderedVector &orderedData = meta->GetOrderedData();

    for(const std::shared_ptr<Meta::DataInfo> &dataInfo : orderedData)
    {
      // Serialize the property if we marked so.
      if(dataInfo->IsSerializable())
      {
        InsertTabs();
        WriteString(dataInfo->GetName());
        WriteString(" ");
        dataInfo->Serialize(static_cast<const void *>(&object), *this);
        InsertNewline();
      }
    }

    DecrementTabs();

    WriteString("}");
    InsertNewline();
  }

  template<typename T>
  void Write(Serializer &stream, const T &object)
  {
    stream.Write(object);
  }
}