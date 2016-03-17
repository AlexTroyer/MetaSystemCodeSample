/*****************************************************************************
File:   ObjectInfo.hpp
Author: Alex Troyer
  Stores information on an object like its constructors.
*****************************************************************************/
#pragma once
namespace Meta
{
  // The following functions are decided by taking advantage of Substition Failure
  // Is Not An Error, or SFINAE.  The way this works is the last argument for the function
  // is a dummy argument that isn't touched and is only looked at during compilation.
  // 
  // std::enable_if looks at the condition inside of its template arguments to see 
  // whether or not if "type" will be defined or not.  If it is, we have an argument that
  // has a type and makes a compilable function.  If the condition is false, type isn't
  // defined making an error.  Because of how templates work the error doesn't show up
  // because there is another function that fits.
  //
  // For the following functions, I'm using SFINAE to choose which function to use.
  // To take the Construct function as an example, if the object has a default constructor,
  // I want to be able to use the constructor.  If it doesn't have a callable one though,
  // a function that calls its default constructor would be a compile error which I don't 
  // want.  So I just create a function that does nothing in its place in order to 
  // get around the error.
  //
  // You can learn more about this here:
  // https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
  // http://en.cppreference.com/w/cpp/language/sfinae

  // Function to construct the object if it has a default constructor.
  template<typename T>
  void *Construct(typename std::enable_if<std::is_default_constructible<T>::value>::type * = nullptr)
  {
    return reinterpret_cast<void *>(new T);
  }

  // Function to construct the object if it doesn't have a default constructor.
  template<typename T>
  void *Construct(typename std::enable_if<!std::is_default_constructible<T>::value>::type * = nullptr)
  {
    return nullptr;
  }

  // Function to placement construct the object if it has a default constructor.
  template<typename T>
  void PlacementConstruct(void *ptr, typename std::enable_if<std::is_default_constructible<T>::value>::type * = nullptr)
  {
    new (ptr) T();
  }

  // Function to placement construct the object if it doesn't have a default constructor.
  template<typename T>
  void PlacementConstruct(void *, typename std::enable_if<!std::is_default_constructible<T>::value>::type * = nullptr)
  {
  }

  // Function to copy the object if it has a copy constructor.
  template<typename T>
  void *Copy(const void *ptr, typename std::enable_if<std::is_copy_constructible<T>::value>::type * = nullptr)
  {
    return reinterpret_cast<void *>(new T(*reinterpret_cast<const T *>(ptr)));
  }

  // Function to copy the object if it doesn't have a copy constructor.
  template<typename T>
  void *Copy(const void *, typename std::enable_if<!std::is_copy_constructible<T>::value>::type * = nullptr)
  {
    return nullptr;
  }

  // Function to placement copy the object if it has a copy constructor.
  template<typename T>
  void PlacementCopy(void *ptr, const void *object, typename std::enable_if<std::is_copy_constructible<T>::value>::type * = nullptr)
  {
    new (ptr)T(*reinterpret_cast<const T *>(object));
  }

  // Function to placement copy the object if it doesn't have a copy constructor.
  template<typename T>
  void PlacementCopy(void *, const void *, typename std::enable_if<!std::is_copy_constructible<T>::value>::type * = nullptr)
  {
  }

  // Function to move construct the object if it has a move constructor.
  template<typename T>
  void *MoveConstruct(void *ptr, typename std::enable_if<std::is_move_constructible<T>::value>::type * = nullptr)
  {
    return reinterpret_cast<void *>(new T(std::move(*reinterpret_cast<T *>(ptr))));
  }

  // Function to move construct the object if it doesn't have a move constructor.
  template<typename T>
  void *MoveConstruct(void *, typename std::enable_if<!std::is_move_constructible<T>::value>::type * = nullptr)
  {
    return nullptr;
  }

  // Function to placement move construct the object if it has a move constructor.
  template<typename T>
  void PlacementMove(void *ptr, void *object, typename std::enable_if<std::is_move_constructible<T>::value>::type * = nullptr)
  {
    new (ptr)T(std::move(*reinterpret_cast<T *>(object)));
  }

  // Function to placement move construct the object if it doesn't have a move constructor.
  template<typename T>
  void PlacementMove(void *, void *, typename std::enable_if<!std::is_move_constructible<T>::value>::type * = nullptr)
  {
  }

  // Function to call delete on the object if it is destructible.
  template<typename T>
  void Destroy(void *ptr, typename std::enable_if<std::is_destructible<T>::value>::type * = nullptr)
  {
    delete reinterpret_cast<T *>(ptr);
  }

  // Function to call delete on the object if it isn't destructible.
  template<typename T>
  void Destroy(void *, typename std::enable_if<!std::is_destructible<T>::value>::type * = nullptr)
  {
  }

  // Function to call the destructor on the object if it is destructible.
  template<typename T>
  void Destructor(void *ptr, typename std::enable_if<std::is_destructible<T>::value>::type * = nullptr)
  {
    T *object = reinterpret_cast<T *>(ptr);
    (void)object;
    object->~T();
  }

  // Function to call the destructor on the object if it isn't destructible.
  template<typename T>
  void Destructor(void *, typename std::enable_if<!std::is_destructible<T>::value>::type * = nullptr)
  {
  }

  // Now I use the above functions to call inside of the ObjectInfo class.

  template<typename T>
  class ObjectInfo : public ObjectInfoBase
  {
  public:
    virtual void *Construct() const
    {
      return Meta::Construct<T>();
    }

    virtual void PlacementConstruct(void *ptr) const
    {
      Meta::PlacementConstruct<T>(ptr);
    }

    virtual void *Copy(const void *ptr) const
    {
      return Meta::Copy<T>(ptr);
    }

    virtual void PlacementCopy(void *ptr, const void *object) const
    {
      Meta::PlacementCopy<T>(ptr, object);
    }

    virtual void *MoveConstruct(void *ptr) const
    {
      return Meta::MoveConstruct<T>(ptr);
    }

    virtual void PlacementMove(void *ptr, void *object) const
    {
      Meta::PlacementMove<T>(ptr, object);
    }

    virtual void Destroy(void *ptr) const
    {
      Meta::Destroy<T>(ptr);
    }

    virtual void Destructor(void *ptr) const
    {
      Meta::Destructor<T>(ptr);
    }

    virtual bool HasConstructor() const
    {
      return std::is_default_constructible<T>::value;
    }

    virtual bool HasCopyConstructor() const
    {
      return std::is_copy_constructible<T>::value;
    }

    virtual bool HasMoveConstructor() const
    {
      return std::is_move_constructible<T>::value;
    }

    virtual bool HasDestructor() const
    {
      return std::is_destructible<T>::value;
    }

    virtual bool IsPolymorphic() const
    {
      return std::is_polymorphic<T>::value;
    }
  };
}