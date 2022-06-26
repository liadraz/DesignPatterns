
//----------------------------------------------------------------------------//
//					Design Patterns
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		API - Factory Design Pattern
//	AUTHOR 		    Liad Raz
//	DATE			19 Apr 2022
//
//----------------------------------------------------------------------------//

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>               // std::shared_ptr
#include <functional>           // std::function
#include <unordered_map>        // std::unordered_map

#include "singleton.hpp"


//----------------------------------------------------------------------------//
//	Factory Declaration
//  DESCRIPTION     Creator class which creates templated classes by the user.
//                  The Factory has two main methods:
//                  1. Add, which adds a new type of class that can be created on run-time.
//                  The factory class holds an Hash-Table of 'Create' functions, each relates to a different class type.
//                  2. Create, the factory method which creates an instance of a class determined by a key.
//
//----------------------------------------------------------------------------//
template <typename Base, typename Key, typename Args>
class Factory
{
public:
    //
	// Special constructors
    // NOTE     Factory class will be instantiated as a singleton. see USE.
    // USE      Factory<Base, Key, Args>* fact(Singleton<Factory<Base, Key, Args>>::GetInstance());
    ~Factory() = default;
    // Class is Uncopiable
    Factory(const Factory& other_) = delete;
    Factory& operator=(const Factory& other_) = delete; 
    
    enum Status {ADDED = 0, UPDATED = 1};

    // Alias for a general-purpose function.    i.e syntax std::function<return(params)>;
    using CreatorFunc_ty = std::function<std::shared_ptr<Base>(Args args)>;
    
    // DESCRIPTION  Adds a creator function to the unordered_map(hashTable) field.
    // NOTE         returns Status UPDATED when key exists; ADDED when key was added. 
    Status Add(const Key& key_, CreatorFunc_ty creator_); 

    // DESCRIPTION  Creates a new Object-task
    // IMPORTANT    Undefined behaviour when passing a key which does not exist.
    std::shared_ptr<Base> Create(const Key& key_, Args args_);

private:
    // NOTE     Singleton requirements
    Factory() = default;
    friend Singleton<Factory>;

    // DESCRIPTIONS unordered_map is (key-value pairs) impl like Hash Table.
    std::unordered_map<Key, CreatorFunc_ty> m_tasks;
};


//----------------------------------------------------------------------------//
//	Factory Class Definitions
//----------------------------------------------------------------------------//
template <typename Base, typename Key, typename Args>
typename Factory<Base, Key, Args>::Status Factory<Base, Key, Args>::Add(const Key& key_, CreatorFunc_ty creator_)
{
    Status status = Status::ADDED;

    if (m_tasks.end() != m_tasks.find(key_))
    {
        status = Status::UPDATED;
    }

    m_tasks[key_] = creator_;

    return status;
}


template <typename Base, typename Key, typename Args>
std::shared_ptr<Base> Factory<Base, Key, Args>::Create(const Key& key_, Args args_)
{
    try
    {
        return m_tasks[key_](args_);
    }
    catch(const std::exception& e)
    {
        throw;
    }
}



#endif // FACTORY_HPP