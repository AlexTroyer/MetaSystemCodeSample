/*****************************************************************************
File:   TestMethod.cpp
Author: Alex Troyer
  Tests Methods to make sure they work correctly.
*****************************************************************************/
#include "TestMethod.h"
#include "Method.h"
#include "Meta.h"
#include <iostream>

// A class to test with.
class TestClass
{
public:
  int Something(int) {return 2;}
  int SomethingConst(int) const {return 3;}
  static int StaticFunc(int) { return 4;}

  int SameArgCount(int, int) {return 1;}
  int SameArgCount(int, float) {return 2;}
  int SameArgCount(int, double) {return 3;}
  int SameArgCount(char, float) {return 4;}
  int SameArgCount(float, double) {return 5;}

  int DifferentArgCount(int) {return 1;}
  int DifferentArgCount(int, int) {return 2;}
  int DifferentArgCount(int, int, int) {return 3;}
  int DifferentArgCount(float) {return 4;}

  int ConstAndNonConst(int) {return 1;}
  int ConstAndNonConst(int) const {return 2;}

  static int StaticOverload(int) {return 1;}
  static int StaticOverload(int, int) {return 2;}
  static int StaticOverload(float, int) {return 3;}
  static int StaticOverload(int, float) {return 4;};
  static int StaticOverload(float) {return 5;}
};

CLASS_START(TestClass)
  // Non-const registration.
  METHOD(Something);
  // Const registration.
  METHOD(SomethingConst);
  // Static registration.
  METHOD(StaticFunc);
  // Overloaded methods with same argument count.
  METHODS(SameArgCount, int (T::*)(int, int),
                        int (T::*)(int, float),
                        int (T::*)(int, double),
                        int (T::*)(char, float),
                        int (T::*)(float, double));
  // Overloaded methods with diferent argument counts.
  METHODS(DifferentArgCount, int (T::*)(int),
                             int (T::*)(int, int),
                             int (T::*)(int, int, int),
                             int (T::*)(float));

  // A const and non const method.
  METHODS(ConstAndNonConst, int (T::*)(int),
                            int (T::*)(int) const);

  // Static overloads.
  METHODS(StaticOverload, int (*)(int),
                          int (*)(int, int),
                          int (*)(float, int),
                          int (*)(int, float),
                          int (*)(float));

CLASS_END;

static void CallingMethods()
{
  bool success = true;
  std::cout << "Non-overloaded Method Test" << std::endl
    << "-------------" << std::endl;

  TestClass test;
  const TestClass cTest;

  Meta::Data *meta = GET_META_VAR(test);
  
  // Test a regular method call
  if(meta->GetMethod("Something")->Call(test, 47).Get<int>() != test.Something(47))
  {
    std::cout << "Method Call non const: Failed" << std::endl;
    success = false;
  }

  // Make sure that we can't call a non-const method with a const object.
  if(meta->GetMethod("Something")->Call(cTest, 23).GetInternal() != nullptr)
  {
    std::cout << "Method Call non const on const object: Failed" << std::endl;
    success = false;
  }

  // Call a const method using a const object.
  if(meta->GetMethod("SomethingConst")->Call(cTest, 49).Get<int>() != cTest.SomethingConst(49))
  {
    std::cout << "Method Call const: Failed" << std::endl;
    success = false;
  }

  // Call a const method with a non const object.
  if(meta->GetMethod("SomethingConst")->Call(test, 324).Get<int>() != test.SomethingConst(324))
  {
    std::cout << "Method Call const on non const object: Failed" << std::endl;
    success = false;
  }

  // Call a static method.
  if(meta->GetMethod("StaticFunc")->CallStatic(45).Get<int>() != TestClass::StaticFunc(45))
  {
    std::cout << "Method Call Static: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void OverloadedMethodSameArgCount()
{
  // Verify that call methods with the same argument count works with different
  // types.

  bool success = true;
  std::cout << "Overloaded Method Test: Same number of arguments" << std::endl
    << "-------------" << std::endl;

  TestClass test;

  Meta::Data *meta = GET_META_VAR(test);

  if(meta->GetMethod("SameArgCount")->Call(test, 5, 5).Get<int>() != test.SameArgCount(5, 5))
  {
    std::cout << "(int, int): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("SameArgCount")->Call(test, 5, 5.0f).Get<int>() != test.SameArgCount(5, 5.0f))
  {
    std::cout << "(int, float): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("SameArgCount")->Call(test, 5, 5.0).Get<int>() != test.SameArgCount(5, 5.0))
  {
    std::cout << "(int, double): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("SameArgCount")->Call(test, (char)5, 5.0f).Get<int>() != test.SameArgCount((char)5, 5.0f))
  {
    std::cout << "(char, float): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("SameArgCount")->Call(test, 5.0f, 5.0).Get<int>() != test.SameArgCount(5.0f, 5.0))
  {
    std::cout << "(float, double): Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void OverloadedMethodDifferentArgCount()
{
  // Verify that calling methods with different argument counts works.

  bool success = true;
  std::cout << "Overloaded Method Test: Different number of arguments" << std::endl
    << "-------------" << std::endl;

  TestClass test;

  Meta::Data *meta = GET_META_VAR(test);

  if(meta->GetMethod("DifferentArgCount")->Call(test, 1).Get<int>() != test.DifferentArgCount(1))
  {
    std::cout << "(int): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("DifferentArgCount")->Call(test, 1, 1).Get<int>() != test.DifferentArgCount(1, 1))
  {
    std::cout << "(int, int): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("DifferentArgCount")->Call(test, 1, 1, 1).Get<int>() != test.DifferentArgCount(1, 1, 1))
  {
    std::cout << "(int, int, int): Failed" << std::endl;
    success = false;
  }

  if(meta->GetMethod("DifferentArgCount")->Call(test, 1.0f).Get<int>() != test.DifferentArgCount(1.0f))
  {
    std::cout << "(float): Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void OverloadedMethodDifferentConst()
{
  // Verify that two overloaded methods with different const-ness are called correctly.

  bool success = true;
  std::cout << "Overloaded Method Test: Different const-ness" << std::endl
    << "-------------" << std::endl;

  TestClass test;
  const TestClass cTest;

  Meta::Data *data = GET_META_VAR(test);

  if(data->GetMethod("ConstAndNonConst")->Call(test, 5).Get<int>() != test.ConstAndNonConst(5))
  {
    std::cout << "Non const: Failed" << std::endl;
    success = false;
  }

  if(data->GetMethod("ConstAndNonConst")->Call(cTest, 23).Get<int>() != cTest.ConstAndNonConst(23))
  {
    std::cout << "Const: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void OverloadedMethodStatic()
{
  // Verify that overloaded static methods work.

  bool success = true;
  std::cout << "Overloaded Method Test: Static" << std::endl
    << "-------------" << std::endl;

  Meta::Data *data = GET_META(TestClass);

  if(data->GetMethod("StaticOverload")->CallStatic(1).Get<int>() != TestClass::StaticOverload(1))
  {
    std::cout << "(int): Failed" << std::endl;
    success = false;
  }

  if(data->GetMethod("StaticOverload")->CallStatic(1, 1).Get<int>() != TestClass::StaticOverload(1, 1))
  {
    std::cout << "(int, int): Failed" << std::endl;
    success = false;
  }

  if(data->GetMethod("StaticOverload")->CallStatic(1.0f, 1).Get<int>() != TestClass::StaticOverload(1.0f, 1))
  {
    std::cout << "(float, int): Failed" << std::endl;
    success = false;
  }

  if(data->GetMethod("StaticOverload")->CallStatic(1, 1.0f).Get<int>() != TestClass::StaticOverload(1, 1.0f))
  {
    std::cout << "(int, float): Failed" << std::endl;
    success = false;
  }

  if(data->GetMethod("StaticOverload")->CallStatic(1.0f).Get<int>() != TestClass::StaticOverload(1.0f))
  {
    std::cout << "(float): Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

void TestMethod()
{
  CallingMethods();
  OverloadedMethodSameArgCount();
  OverloadedMethodDifferentArgCount();
  OverloadedMethodDifferentConst();
  OverloadedMethodStatic();
}