/*****************************************************************************
File:   TestAny.cpp
Author: Alex Troyer
  Tests the "Any" type to make sure it works correctly.
*****************************************************************************/
#include "TestAny.h"
#include "Meta.h"
#include "Any.h"
#include <iostream>
#include <chrono>
#include <random>
#include <limits>

static std::default_random_engine s_Generator;

// Random number generators to make that these properties truly work and just by the luck of the numbers.

template<typename T>
T RandomMinMaxInt()
{
  std::uniform_int_distribution<T> random(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  return random(s_Generator);
}

template<typename T>
T RandomMinMaxReal()
{
  std::uniform_real_distribution<T> random(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  return random(s_Generator);
}

static void AnyAssignmentTest()
{
  bool success = true;

  std::cout << "Any Test" << std::endl
            << "-------------" << std::endl;
  int testInt = RandomMinMaxInt<int>();
  Any intAny = testInt;

  // Test int construction.
  if(intAny.Get<int>() != testInt)
  {
    std::cout << "Int: Failed" << std::endl;
    success = false;
  }

  unsigned testUnsignedInt = RandomMinMaxInt<unsigned>();
  Any unsignedIntAny = testUnsignedInt;

  // Test unsigned int construction.
  if(unsignedIntAny.Get<unsigned>() != testUnsignedInt)
  {
    std::cout << "Unsigned Int: Failed" << std::endl;
    success = false;
  }

  float testFloat = RandomMinMaxReal<float>();
  Any floatAny = testFloat;

  // Test float construction.
  if(floatAny.Get<float>() != testFloat)
  {
    std::cout << "Float: Failed" << std::endl;
    success = false;
  }
  
  std::string str = "This is some string";
  Any stringAny = str;

  // Test float construction.
  if(stringAny.Get<std::string>() != str)
  {
    std::cout << "String: Failed" << std::endl;
    success = false;
  }
  
  str = "Here's another one for a test...";
  stringAny = std::move(str);

  // Test for reassignment
  if(stringAny.Get<std::string>() != str)
  {
    std::cout << "String Reassignment: Failed" << std::endl;
    success = false;
  }

  // Another string to test with.
  Any anotherAny = stringAny;
  if(anotherAny.Get<std::string>() != stringAny.Get<std::string>())
  {
    std::cout << "Any Construction: Failed" << std::endl;
    success = false;
  }
  
  // Any assignment.
  stringAny = std::string("Yet another test string");
  anotherAny = stringAny;

  if(anotherAny.Get<std::string>() != stringAny.Get<std::string>())
  {
    std::cout << "Any Reassignment: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

void TestAny()
{
  time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  s_Generator.seed(static_cast<unsigned>(time));

  AnyAssignmentTest();
}
