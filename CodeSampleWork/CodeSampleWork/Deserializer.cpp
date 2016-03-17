/*****************************************************************************
File:   Deserializer.cpp
Author: Alex Troyer
  Takes information and reads it using the meta system.
*****************************************************************************/
#include "Deserializer.h"

namespace Util
{
  // Constructor which opens up the given file.
  Deserializer::Deserializer(const std::string &file)
  {
    Open(file);
  }

  // Opens the given file, and returns if opening was successful or not.
  bool Deserializer::Open(const std::string &file)
  {
    m_OpenedFileName = file;

    m_Stream.open(file);

    return IsGood();
  }

  void Deserializer::Close()
  {
    m_Stream.close();
  }

  bool Deserializer::IsGood() const
  {
    return m_Stream.good();
  }

  bool Deserializer::Failed() const
  {
    return m_Stream.fail();
  }

  const std::string &Deserializer::GetOpenedFile() const
  {
    return m_OpenedFileName;
  }

  void Deserializer::Read(int &i)
  {
    m_Stream >> i;
  }

  void Deserializer::Read(unsigned &u)
  {
    m_Stream >> u;
  }

  void Deserializer::Read(bool &b)
  {
    m_Stream >> std::boolalpha >> b;
  }

  void Deserializer::Read(float &f)
  {
    m_Stream >> f;
  }

  void Deserializer::Read(double &d)
  {
    m_Stream >> d;
  }

  void Deserializer::Read(short &s)
  {
    m_Stream >> s;
  }

  void Deserializer::Read(unsigned short &s)
  {
    m_Stream >> s;
  }

  void Deserializer::Read(unsigned char &c)
  {
    m_Stream >> c;
  }

  void Deserializer::Read(signed char &c)
  {
    m_Stream >> c;
  }

  void Deserializer::Read(char &c)
  {
    m_Stream >> c;
  }

  // A string is something between two quotes, like a string in C++.
  void Deserializer::Read(std::string &str)
  {
    ReadUntil('"');
    int temp = m_Stream.get();

    if(temp != '"')
    {
      do
      {
        str += static_cast<char>(temp);
        temp = m_Stream.get();
      }
      while(temp != '"' && IsGood());
    }
  }

  // Reads the next sequence of characters up until whitespace.
  void Deserializer::ReadString(std::string &str)
  {
    m_Stream >> str;
  }

  // Read until a given character.
  bool Deserializer::ReadUntil(char c)
  {
    int temp;

    do
    {
      temp = m_Stream.get();
    }
    while(temp != c && IsGood());

    return IsGood();
  }

  // Read until the given sequence of characters has been read.
  bool Deserializer::ReadUntil(const std::string &str)
  {
    unsigned index = 0;
    int temp;

    while(index < str.size() && IsGood())
    {
      temp = m_Stream.get();
      if(temp == str[index])
      {
        index++;
      }
      else
      {
        index = 0;
      }
    }

    return IsGood();
  }
}