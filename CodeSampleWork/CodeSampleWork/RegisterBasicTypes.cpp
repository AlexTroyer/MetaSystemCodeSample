/*****************************************************************************
File:   RegisterBasicTypes.cpp
Author: Alex Troyer
  Registers some basic types to the meta system.
*****************************************************************************/
#include "Meta.h"
#include <string>

// Macro tries to take sizeof void so I have to do it this way...
Meta::SimpleRegisterMetaData<void> Meta::SimpleRegister<void>::m_Register("void", 0, new Meta::ObjectInfo<void>());
DEFINE_SIMPLE_TYPE(int);
DEFINE_SIMPLE_TYPE_NAME("u_int", unsigned int);
DEFINE_SIMPLE_TYPE(bool);
DEFINE_SIMPLE_TYPE(short);
DEFINE_SIMPLE_TYPE_NAME("u_short", unsigned short);

// Yes these are technically three different things...
DEFINE_SIMPLE_TYPE(char);
DEFINE_SIMPLE_TYPE_NAME("s_char", signed char);
DEFINE_SIMPLE_TYPE_NAME("u_char", unsigned char);

DEFINE_SIMPLE_TYPE(double);
DEFINE_SIMPLE_TYPE(float);

DEFINE_SIMPLE_TYPE_NAME("string", std::string);
