#include "TestObjectInfo.h"
#include "Meta.h"
#include <iostream>

// A class with all generated constructors and destructor.
class ObjectInfoTestNoDelete
{
};

// A class with deleted constructors and destructor.
class ObjectInfoTestDelete
{
public:
  ObjectInfoTestDelete() = delete;
  ObjectInfoTestDelete(const ObjectInfoTestDelete &) = delete;
  ObjectInfoTestDelete(ObjectInfoTestDelete &&) = delete;
  ~ObjectInfoTestDelete() = delete;
};

// A class with all constructors and destructor public.
class ObjectInfoTestPublic
{
public:
  ObjectInfoTestPublic() {};
  ObjectInfoTestPublic(const ObjectInfoTestPublic &) {};
  ObjectInfoTestPublic(ObjectInfoTestPublic &&) {};
  ~ObjectInfoTestPublic() {};
};

// A class with all constructors and destructor private.
class ObjectInfoTestPrivate
{
private:
  ObjectInfoTestPrivate(){};
  ObjectInfoTestPrivate(const ObjectInfoTestPrivate &){};
  ObjectInfoTestPrivate(ObjectInfoTestPrivate &&){};
  ~ObjectInfoTestPrivate(){};
};

CLASS_START(ObjectInfoTestNoDelete)
CLASS_END;

CLASS_START(ObjectInfoTestDelete)
CLASS_END;

CLASS_START(ObjectInfoTestPublic)
CLASS_END;

CLASS_START(ObjectInfoTestPrivate)
CLASS_END;

static void TestNotDeleted()
{
  // All constructors and destructor should be callable.

  bool success = true;

  std::cout << "ObjectInfo Test: No Deleted Constructors/Destructor" << std::endl
    << "-------------" << std::endl;

  Meta::ObjectInfoBase *notDeleted = GET_META(ObjectInfoTestNoDelete)->GetObjectInfo();

  void *ptr = notDeleted->Construct();
  if(ptr == nullptr)
  {
    std::cout << "Constructor: Failed" << std::endl;
    success = false;
  }

  void *copyPtr = notDeleted->Copy(ptr);
  if(copyPtr == nullptr)
  {
    std::cout << "Copy Constructor: Failed" << std::endl;
    success = false;
  }

  void *movePtr = notDeleted->MoveConstruct(ptr);
  if(movePtr == nullptr)
  {
    std::cout << "Move Constructor: Failed" << std::endl;
    success = false;
  }

  notDeleted->Destroy(ptr);
  notDeleted->Destroy(copyPtr);
  notDeleted->Destroy(movePtr);
  
  if(!notDeleted->HasDestructor())
  {
    std::cout << "Destructor: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void TestDeleted()
{
  // All constructors and destructor should not do anything.

  bool success = true;

  std::cout
    << "ObjectInfo Test: Deleted Constructors/Destructor" << std::endl
    << "-------------" << std::endl;

  Meta::ObjectInfoBase *deleted = GET_META(ObjectInfoTestDelete)->GetObjectInfo();

  if(deleted->Construct() != nullptr)
  {
    std::cout << "Deleted Constructor: Failed" << std::endl;
    success = false;
  }

  if(deleted->Copy(nullptr) != nullptr)
  {
    std::cout << "Deleted Copy Constructor: Failed" << std::endl;
    success = false;
  }

  if(deleted->MoveConstruct(nullptr) != nullptr)
  {
    std::cout << "Deleted Move Constructor: Failed" << std::endl;
    success = false;
  }

  if(deleted->HasDestructor())
  {
    std::cout << "Deleted Destructor: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;

}

static void TestPublic()
{
  // All constructors and destructor should be callable.

  bool success = true;

  std::cout << "ObjectInfo Test: Public Constructors/Destructor" << std::endl
    << "-------------" << std::endl;

  Meta::ObjectInfoBase *publicClass = GET_META(ObjectInfoTestPublic)->GetObjectInfo();

  void *ptr = publicClass->Construct();
  if(ptr == nullptr)
  {
    std::cout << "Constructor: Failed" << std::endl;
    success = false;
  }

  void *copyPtr = publicClass->Copy(ptr);
  if(copyPtr == nullptr)
  {
    std::cout << "Copy Constructor: Failed" << std::endl;
    success = false;
  }

  void *movePtr = publicClass->MoveConstruct(ptr);
  if(movePtr == nullptr)
  {
    std::cout << "Move Constructor: Failed" << std::endl;
    success = false;
  }

  publicClass->Destroy(ptr);
  publicClass->Destroy(copyPtr);
  publicClass->Destroy(movePtr);

  if(!publicClass->HasDestructor())
  {
    std::cout << "Destructor: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

static void TestPrivate()
{
  // All constructors and destructor should not do anything.

  bool success = true;

  std::cout
    << "ObjectInfo Test: Private Constructors/Destructor" << std::endl
    << "-------------" << std::endl;

  Meta::ObjectInfoBase *privateClass = GET_META(ObjectInfoTestPrivate)->GetObjectInfo();

  if(privateClass->Construct() != nullptr)
  {
    std::cout << "Deleted Constructor: Failed" << std::endl;
    success = false;
  }

  if(privateClass->Copy(nullptr) != nullptr)
  {
    std::cout << "Deleted Copy Constructor: Failed" << std::endl;
    success = false;
  }

  if(privateClass->MoveConstruct(nullptr) != nullptr)
  {
    std::cout << "Deleted Move Constructor: Failed" << std::endl;
    success = false;
  }

  if(privateClass->HasDestructor())
  {
    std::cout << "Deleted Destructor: Failed" << std::endl;
    success = false;
  }

  if(success)
  {
    std::cout << "Success" << std::endl;
  }

  std::cout << std::endl;
}

void TestObjectInfo()
{
  TestNotDeleted();
  TestDeleted();
  TestPublic();
  TestPrivate();
}
