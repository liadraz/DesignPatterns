
//----------------------------------------------------------------------------//
//					Design Patterns
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		Tests - Factory Method DP
//	AUTHOR 		    Liad Raz
//	DATE			19 Apr 2022
//
//----------------------------------------------------------------------------//

#include <iostream>			// std::cout
#include <memory>           // std::shared_ptr
#include <string>			// std::string

#include "factory.hpp"

using namespace hrd21;


//----------------------------------------------------------------------------//
// << Interafce >>
class IAnimal
{
public:
    //
    // Special Ctors
    IAnimal();
    virtual ~IAnimal() = 0;
    // Cctor. copy= are blocked from use
    IAnimal(const IAnimal& other_) = default;
    IAnimal& operator=(const IAnimal& other_) = default;

    enum Key {DOG, CAT};

    virtual void PrintSound() = 0;
};

IAnimal::~IAnimal() = default;


//
class Dog : public IAnimal
{
public:
    //
    // Special Ctors
    Dog() : m_arg(1)
    {}

    ~Dog() noexcept override;
    // Cctor. copy= are blocked from use
    Dog(const Dog& other_) = default;
    Dog& operator=(const Dog& other_) = default;

    void PrintSound() override;

    // Creation Function
    static std::shared_ptr<Dog> CreateDog(int param_)
    {
        return std::shared_ptr<Dog>(new Dog());
    }

private:
    int m_arg;
};

// Definitions
void Dog::PrintSound()
{
    std::cout << "Hwo Hwo" << std::endl;
}

//----------------------------------------------------------------------------//
class Cat : public IAnimal
{
public:
    Cat() : m_arg(1)
    {}
    ~Cat();

    void PrintSound() override;

    // Creation Function
    static std::shared_ptr<Cat> CreateCat(int param_)
    {
        return std::shared_ptr<Cat>(new Cat());
    }

private:
    int m_arg;
};

// Definitions
void Cat::PrintSound()
{
    std::cout << "MEOW" << std::endl;
}



int main()
{
    std::cout << "Tests Factory Method Design Pattern" << std::endl;

    std::shared_ptr<Factory<IAnimal, IAnimal::Key, int>>
    fact(Singleton<Factory<IAnimal, IAnimal::Key, int>>::GetInstance());


    fact.get()->Add(IAnimal::Key::DOG, Dog::CreateDog);


    return 0;
}