/*****************************************************************************
File:   TestSerializer.cpp
Author: Alex Troyer
  Tests the serializer to make sure it works correctly.
*****************************************************************************/
#include "TestSerializer.h"
#include "Property.h"
#include "Serializer.h"

#include "Meta.h"
#include <string>
#include <iostream>

class Test
{
public:

  int GetValue() const
  {
    return m_IntValue;
  }

  void SetValue(int val)
  {
    m_IntValue = val;
  }

  bool operator==(const Test &rhs) const
  {
    return m_IntValue == rhs.m_IntValue &&
           m_UnsignedValue == rhs.m_UnsignedValue &&
           m_FloatValue == rhs.m_FloatValue &&
           m_String == rhs.m_String;
  }

  bool operator!=(const Test &rhs) const
  {
    return !(*this == rhs);
  }

  unsigned m_UnsignedValue = 6;
  float m_FloatValue = 7;
  std::string m_String = "String something blah";

private:
  int m_IntValue = 5;

  META_PRIVATE_ACCESS;
};

CLASS_START(Test)
  PROPERTY("Value", GetValue, SetValue).EnableSerialization();
  MEMBER(m_UnsignedValue).EnableSerialization();
  MEMBER(m_FloatValue).EnableSerialization();
  MEMBER(m_String).EnableSerialization();
CLASS_END;

void TestSerializer()
{
  // First write to a file.
  Test test;
  test.SetValue(42);
  test.m_UnsignedValue = 42;
  test.m_FloatValue = 42;
  test.m_String = "Fourty two";
  Util::Serializer stream("test.txt");
  stream.Write(test);

  // Now read from that file we wrote.
  Test readTest;
  Util::Deserializer readStream("test.txt");
  readStream.Read(readTest);

  bool success = true;

  std::cout << "Serialize/Deserailize Test" << std::endl
    << "-------------" << std::endl;

  // If both of the classes are the same, we succeeded.
  if(test != readTest)
  {
    std::cout << "Serialize then Deserialize: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;

}