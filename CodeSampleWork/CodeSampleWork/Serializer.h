/*****************************************************************************
File:   Serializer.h
Author: Alex Troyer
  Takes information and writes it to a file using the meta system.
*****************************************************************************/
#pragma once

#include <string>
#include <fstream>
#include "Meta.h"
#include "DataInfo.h"

namespace Util
{
  class Serializer
  {
  public:
    Serializer(const std::string &file, bool append = false);
    bool Open(const std::string &file, bool append = false);
    void Close();
    bool IsGood() const;
    bool Failed() const;
    const std::string &GetOpenedFile() const;

    void Write(const int &i);
    void Write(const unsigned &u);
    void Write(const bool &b);
    void Write(const float &f);
    void Write(const double &d);
    void Write(const short &s);
    void Write(const unsigned short &s);
    void Write(const unsigned char &c);
    void Write(const signed char &c);
    void Write(const char &c);
    void Write(const std::string &str);
    void Write(const char *str);
    void WriteString(const std::string &str);

    void InsertTabs();
    void InsertNewline();
    void IncrementTabs();
    void DecrementTabs();

    template<typename T>
    void Write(const T &object);

  private:
    std::ofstream m_Stream;
    std::string m_OpenedFileName;
    int m_Tabs = 0;
  };

  template<typename T>
  void Write(Serializer &stream, const T &object);
}

#include "Serializer.hpp"