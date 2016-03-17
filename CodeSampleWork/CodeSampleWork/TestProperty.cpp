/*****************************************************************************
File:   TestProperty.cpp
Author: Alex Troyer
  Tests properties to make sure it works correctly.
*****************************************************************************/
#include "TestProperty.h"
#include "Meta.h"
#include "Any.h"
#include "Property.h"
#include <iostream>

class PropertyTest
{
public:
  int GetValue() const
  {
    return m_Value;
  }

  void SetValue(const int &value)
  {
    m_Value = value;
  }

  int OnlyGet() const
  {
    return m_OnlyGet;
  }

  static int GetStaticValueGet()
  {
    return 234235;
  }

  static int GetStaticValue()
  {
    return m_AnotherStatic;
  }

  static void SetStaticValue(int num)
  {
    m_AnotherStatic = num;
  }

  int m_Member = 234;
  const int m_Constant = 42;
  static int m_StaticValue;
  static const int m_ConstantStaticValue = 34985745;

private:
  int m_Value = 17;
  int m_OnlyGet = 42;
  static int m_AnotherStatic;

  META_PRIVATE_ACCESS;
};

int PropertyTest::m_StaticValue = 2359;
int PropertyTest::m_AnotherStatic = 235;

CLASS_START(PropertyTest)
  PROPERTY("Value", GetValue, SetValue);
  PROPERTY("OnlyGet", OnlyGet);
  PROPERTY("Static", GetStaticValue, SetStaticValue);
  PROPERTY("StaticGet", GetStaticValueGet);
  MEMBER(m_Member);
  MEMBER(m_StaticValue);
  MEMBER(m_Constant);
  MEMBER(m_ConstantStaticValue);
CLASS_END;

void TestProperty()
{
  // Verify that getting and setting the different properties work.

  PropertyTest test;
  Meta::Data *meta = GET_META_VAR(test);

  bool success = true;

  std::cout << "Property Test" << std::endl
            << "-------------" << std::endl;

  if(meta->GetProperty("Value")->Get(test).Get<int>() != test.GetValue())
  {
    std::cout << "Get/Set Property; Get: Failed" << std::endl;
    success = false;
  }

  meta->GetProperty("Value")->Set(test, 42);

  if(meta->GetProperty("Value")->Get(test).Get<int>() != test.GetValue())
  {
    std::cout << "Get/Set Property; Set: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("OnlyGet")->Get(test).Get<int>() != test.OnlyGet())
  {
    std::cout << "Get Property: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("Static")->Get().Get<int>() != test.GetStaticValue())
  {
    std::cout << "Get/Set Static Property; Get: Failed" << std::endl;
    success = false;
  }

  meta->GetProperty("Static")->Set(44559);

  if(meta->GetProperty("Static")->Get().Get<int>() != test.GetStaticValue())
  {
    std::cout << "Get/Set Static Property; Set: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("StaticGet")->Get().Get<int>() != test.GetStaticValueGet())
  {
    std::cout << "Get Static Property: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("m_Member")->Get(test).Get<int>() != test.m_Member)
  {
    std::cout << "Get Member: Failed" << std::endl;
    success = false;
  }

  meta->GetProperty("m_Member")->Set(test, 7843);

  if(meta->GetProperty("m_Member")->Get(test).Get<int>() != test.m_Member)
  {
    std::cout << "Set Member: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("m_StaticValue")->Get().Get<int>() != test.m_StaticValue)
  {
    std::cout << "Get Static Member: Failed" << std::endl;
    success = false;
  }

  meta->GetProperty("m_StaticValue")->Set(7451);

  if(meta->GetProperty("m_StaticValue")->Get().Get<int>() != test.m_StaticValue)
  {
    std::cout << "Set Static Member: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("m_Constant")->Get(test).Get<int>() != test.m_Constant)
  {
    std::cout << "Get Constant Member: Failed" << std::endl;
    success = false;
  }

  if(meta->GetProperty("m_ConstantStaticValue")->Get().Get<int>() != test.m_ConstantStaticValue)
  {
    std::cout << "Get Const Static Member: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}
