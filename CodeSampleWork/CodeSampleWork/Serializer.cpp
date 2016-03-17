/*****************************************************************************
File:   Serializer.cpp
Author: Alex Troyer
  Takes information and writes it to a file using the meta system.
*****************************************************************************/
#include "Serializer.h"

namespace Util
{
  // Constructs a serializer with a file to open, and whether or not to append.
  Serializer::Serializer(const std::string &file, bool append)
  {
    Open(file, append);
  }

  // Opens a file with the option to append to it.
  bool Serializer::Open(const std::string &file, bool append)
  {
    m_OpenedFileName = file;

    if(append)
      m_Stream.open(file, std::ofstream::app);
    else
      m_Stream.open(file);

    return IsGood();
  }

  void Serializer::Close()
  {
    m_Stream.close();
  }

  bool Serializer::IsGood() const
  {
    return m_Stream.good();
  }

  bool Serializer::Failed() const
  {
    return m_Stream.fail();
  }

  const std::string &Serializer::GetOpenedFile() const
  {
    return m_OpenedFileName;
  }

  void Serializer::Write(const int &i)
  {
    m_Stream << i;
  }

  void Serializer::Write(const unsigned &u)
  {
    m_Stream << u;
  }

  void Serializer::Write(const bool &b)
  {
    m_Stream << std::boolalpha << b;
  }

  void Serializer::Write(const float &f)
  {
    m_Stream << f;
  }

  void Serializer::Write(const double &d)
  {
    m_Stream << d;
  }

  void Serializer::Write(const short &s)
  {
    m_Stream << s;
  }

  void Serializer::Write(const unsigned short &s)
  {
    m_Stream << s;
  }

  void Serializer::Write(const unsigned char &c)
  {
    m_Stream << c;
  }

  void Serializer::Write(const signed char &c)
  {
    m_Stream << c;
  }

  void Serializer::Write(const char &c)
  {
    m_Stream << c;
  }

  void Serializer::Write(const std::string &str)
  {
    m_Stream << "\"" << str << "\"";
  }

  void Serializer::Write(const char *str)
  {
    m_Stream << "\"" << str << "\"";
  }

  void Serializer::WriteString(const std::string &str)
  {
    m_Stream << str;
  }

  // Inserts tabs for outputting the file.  Tabs are two spaces.
  void Serializer::InsertTabs()
  {
    for(int i = 0; i < m_Tabs; i++)
    {
      m_Stream << "  ";
    }
  }

  void Serializer::InsertNewline()
  {
    m_Stream << std::endl;
  }

  void Serializer::IncrementTabs()
  {
    ++m_Tabs;
  }

  void Serializer::DecrementTabs()
  {
    --m_Tabs;
  }
}