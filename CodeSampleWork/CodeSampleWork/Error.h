/*****************************************************************************
File:   Error.h
Author: Alex Troyer
  Helper to display errors to the screen to notify that there is something
  wrong.
*****************************************************************************/
#pragma once

#include <string>

#ifdef _DEBUG
#define FATAL_ERROR(msg) \
DisplayError(msg, true, __FUNCTION__, __FILE__, __LINE__) 

#define FATAL_ERROR_IF(cond, msg) \
{ \
  if(cond)\
    FATAL_ERROR(msg); \
} struct forcesemicolon
#else
#define FATAL_ERROR(msg) struct forcesemicolon
#define FATAL_ERROR_IF(cond, msg) struct forcesemicolon
#endif

void DisplayError(const std::string &error, bool isFatal,
                  const std::string &funcName, const std::string &sourceFile,
                  unsigned int lineNum);