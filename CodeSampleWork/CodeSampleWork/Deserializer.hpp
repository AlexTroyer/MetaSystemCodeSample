/*****************************************************************************
File:   Deserializer.hpp
Author: Alex Troyer
  Takes information and reads it using the meta system.
*****************************************************************************/
#pragma once

namespace Util
{
  template<typename T>
  void Deserializer::Read(T &object)
  {
    Meta::Data *meta = GET_META(T);

    // Read until we reach the first open curley brace.
    ReadUntil('{');

    std::string name;
    ReadString(name);
    
    // If we read a close curley brace, stop reading.
    while(name.find('}') == std::string::npos && IsGood())
    {
      // Find the property name.
      Meta::Property *prop = meta->GetProperty(name);

      // If it exists, deserialize it.  Otherwise display an error.
      if(prop)
      {
        prop->Deserialize(&object, *this);
      }
      else
      {
        FATAL_ERROR("Property '" + name + "' doesn't exist on class '" + meta->GetName() + "'");
      }

      ReadString(name);
    }
  }

  // Read the object from the stream.
  template<typename T>
  void Read(Deserializer &stream, T &object)
  {
    stream.Read(object);
  }
}