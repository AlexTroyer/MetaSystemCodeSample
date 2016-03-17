/*****************************************************************************
File:   Method.cpp
Author: Alex Troyer
  Stores method information for a class.
*****************************************************************************/
#include "Method.h"
#include "Meta.h"

namespace Meta
{
  // Constructor for method.
  Method::Method(const std::string &name, Meta::Data *returnData, size_t argNum, bool isStatic, bool isConst)
    : DataInfo(name, returnData, isStatic)
    , m_ArgNum(argNum)
    , m_IsConst(isConst)
  {
  }

  // Whether or not the method is const.
  bool Method::IsConst() const
  {
    return m_IsConst;
  }
  
  // Get the argument count.
  size_t Method::GetArgNum() const
  {
    return m_ArgNum;
  }

  // Get the meta data of all the arguments in order.
  const std::vector<Meta::Data*> &Method::GetArguments() const
  {
    return m_ArgumentMeta;
  }

  // Default call for a non-const object, which does nothing.
  Any Method::Call(void *, const std::vector<Any> &) const
  {
    return Any();
  }

  // Default call for a const object, which does nothing.
  Any Method::Call(const void *, const std::vector<Any> &) const
  {
    return Any();
  }

  // Default call for a static method, which does nothing.
  Any Method::CallStatic(const std::vector<Any> &) const
  {
    return Any();
  }

  // Whether or not the methods that are overloaded are static.
  bool MethodOverloads::IsStatic() const
  {
    return m_IsStatic;
  }

  // Get whether or not the types of the Any types and the argument meta data are
  // the same.
  static bool IsSameTypes(const std::vector<Any> &args, const std::vector<Meta::Data *> &argMeta)
  {
    for(unsigned i = 0; i < args.size(); i++)
    {
      if(args[i].GetMeta() != argMeta[i])
      {
        return false;
      }
    }

    return true;
  }

  // Finds a method that share the same properties as the arguments passed in.
  static std::shared_ptr<Method> FindMethod(const std::vector<std::shared_ptr<Method>> &methods,
                                            const std::vector<Any> &args, 
                                            bool isConst)
  {
    // If we only have one method, we simplify the search a bit.
    if(methods.size() == 1)
    {
      // As long as the argument count is the same, if it is a const object calling 
      // a const method or a non-const object calling either a const or non-const method,
      //and if the arguments are correct, we can use this given method.
      if(methods.front()->GetArgNum() == args.size() &&
         (methods.front()->IsConst() == isConst || 
         (isConst == false && methods.front()->IsConst())) &&
         IsSameTypes(args, methods.front()->GetArguments()))
      {
        return methods.front();
      }

      return nullptr;
    }

    // We can call a const method with a non const object so hold onto the
    // const method if we find it and don't find a non const one down the road.
    std::shared_ptr<Method> constMethod = nullptr;

    // Go through all the methods to find a match.
    for(const std::shared_ptr<Method> &method : methods)
    {
      // If the methods have the same argument count and the argument types match
      // up, we have a match.
      if(method->GetArgNum() == args.size() &&
         IsSameTypes(args, method->GetArguments()))
      {
        // If the const is correct, return this method.
        if(method->IsConst() == isConst)
        {
          return method;
        }
        // Since we have a non-const object with a const method,
        // we can't assume even though this is correct that this is the best match.
        // Hold onto this method to return later if we don't find a non-const method.
        else if(isConst == false)
        {
          constMethod = method;
        }
      }
    }

    // If we found the const method for a non-const object, return that now.
    // Otherwise we are returning a nullptr.
    return constMethod;
  }

  // Call for a non-const object.
  Any MethodOverloads::Call(void *object, const std::vector<Any> &args) const
  {
    // If these methods are static, we can't call this method with an object.
    if(IsStatic() == true)
    {
      return Any();
    }

    // Find the method for the given arguments.
    std::shared_ptr<Method> method = FindMethod(m_Methods, args, false); 

    // If we found the method, call it.
    if(method)
    {
      return method->Call(object, args);
    }
    else
    {
      return Any();
    }
  }

  Any MethodOverloads::Call(const void *object, const std::vector<Any> &args) const
  {
    // If these methods are static, we can't call this method with an object.
    if(IsStatic() == true)
    {
      return Any();
    }

    // Find the method for the given arguments.
    std::shared_ptr<Method> method = FindMethod(m_Methods, args, true);

    // If we found the method, call it.
    if(method)
    {
      return method->Call(object, args);
    }
    else
    {
      return Any();
    }
  }

  Any MethodOverloads::CallStatic(const std::vector<Any> &args) const
  {
    // If these methods aren't static, we can't call this method.
    if(IsStatic() == false)
    {
      return Any();
    }

    // Find the method for the given arguments.
    std::shared_ptr<Method> method = FindMethod(m_Methods, args, false);

    // If we found the method, call it.
    if(method)
    {
      return method->CallStatic(args);
    }
    else
    {
      return Any();
    }
  }

  // Adds a given method.
  void MethodOverloads::AddMethod(Method *method)
  {
    // If we have no methods, figure out if the method is static or not 
    // and use that in the future.
    // Also store th method.
    if(m_Methods.empty())
    {
      m_IsStatic = method->IsStatic();
      m_Methods.push_back(std::shared_ptr<Method>(method));
    }
    else
    {
      // Error if we add a method that is static or not static when this is holding
      // the opposite.
      FATAL_ERROR_IF(m_IsStatic != method->IsStatic(), 
                     "Method '" + method->GetName() + "' on class '" 
                     + m_Owner->GetName() + 
                     "' has a static and non-static overload which is not allowed.");
      
      // In debug, iterate through all the methods and make sure there are no conflicts with
      // argument count, argument types, and if the methods are the same const.
      #ifdef _DEBUG
      for(std::shared_ptr<Method> &storedMethod : m_Methods)
      {
        if(method->GetArgNum() == storedMethod->GetArgNum() &&
           method->GetArguments() == storedMethod->GetArguments() &&
           method->IsConst() == storedMethod->IsConst())
        {
          FATAL_ERROR("Method '" + method->GetName() + "' has an overload with the same signature on class '" +
                      m_Owner->GetName() + "'");
        }
      }
      #endif

      // We passed all the tests at this point so add the method as an overload.
      m_Methods.push_back(std::shared_ptr<Method>(method));
    }
  }
}