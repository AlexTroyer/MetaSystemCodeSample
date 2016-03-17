/*****************************************************************************
File:   Deserializer.h
Author: Alex Troyer
  Takes information and reads it using the meta system.
*****************************************************************************/
#pragma once

#include <fstream>
#include <string>
#include "Meta.h"
#include "DataInfo.h"
#include "Property.h"
#include "Error.h"

namespace Util
{
  class Deserializer
  {
  public:
    Deserializer(const std::string &file);
    bool Open(const std::string &file);
    void Close();
    bool IsGood() const;
    bool Failed() const;
    const std::string &GetOpenedFile() const;

    void Read(int &i);
    void Read(unsigned &u);
    void Read(bool &b);
    void Read(float &f);
    void Read(double &d);
    void Read(short &s);
    void Read(unsigned short &s);
    void Read(unsigned char &c);
    void Read(signed char &c);
    void Read(char &c);
    void Read(std::string &str);
    void ReadString(std::string &str);
    
    bool ReadUntil(char c);
    bool ReadUntil(const std::string &str);

    template<typename T>
    void Read(T &object);

  private:
    std::ifstream m_Stream;
    std::string m_OpenedFileName;

  };

  template<typename T>
  void Read(Deserializer &stream, T &object);
}

#include "Deserializer.hpp"