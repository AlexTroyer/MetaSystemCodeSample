/*****************************************************************************
File:   Main.cpp
Author: Alex Troyer
  Where main lives.
*****************************************************************************/
#include "Meta.h"
#include "TestObjectInfo.h"
#include "TestAny.h"
#include "TestProperty.h"
#include "TestMethod.h"
#include "TestSerializer.h"
#include <iostream>

int main()
{
  // Run all the tests for the meta system.
  TestObjectInfo();
  TestAny();
  TestProperty();
  TestMethod();
  TestSerializer();

  std::getchar();

  return 0;
}