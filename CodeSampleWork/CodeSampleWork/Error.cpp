/*****************************************************************************
File:   Error.cpp
Author: Alex Troyer
  Helper to display errors to the screen to notify that there is something 
  wrong.
*****************************************************************************/
#include "Error.h"
#include <iostream>

void DisplayError(const std::string &error, bool isFatal,
                  const std::string &funcName, const std::string &sourceFile,
                  unsigned int lineNum)
{
  std::string fullError = "Function: " + funcName +
                          "\nSource: " + sourceFile + 
                          "\nLine: " + std::to_string(lineNum) + "\n" +
                          error;
  std::cerr << error << std::endl;

  if(isFatal)
  {
    __debugbreak();
  }
}
