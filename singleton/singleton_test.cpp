
//----------------------------------------------------------------------------//
//					Design Patterns
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		Tests - Singleton DP
//	AUTHOR 			Liad Raz
//	DATE			19 Apr 2022
//
//----------------------------------------------------------------------------//

#include <iostream>			// std::cout, std::cin
#include "singleton.hpp"


class Logger
{
public:
    ~Logger() = default;
    // Uncopiable
    Logger(const Logger& other_) = delete;
    Logger& operator=(const Logger& other_) = delete;

    // static std::unique_ptr<Logger> GetInstance();

private:
    Logger() = default;
    friend Singleton<Logger>;

};

//	Logger Deceleration
//----------------------------------------------------------------------------//
// std::unique_ptr<Logger> Logger::GetInstance()
// {
//     return Singleton<Logger>::GetInstance();
// }



//----------------------------------------------------------------------------//
int main()
{
    Logger* log = Singleton<Logger>::GetInstance();
    Logger* log2(Singleton<Logger>::GetInstance());

    std::cout << log << std::endl;
    std::cout << log2 << std::endl;



    std::unique_ptr<Logger> logPtr1 = Singleton<Logger>::GetInstance();
    std::cout << " logPtr1: " << logPtr1.get() << std::endl;

    std::unique_ptr<Logger> logPtr2 = Singleton<Logger>::GetInstance();
    std::cout << " logPtr1: " << logPtr1.get() << " logPtr2: " << logPtr2.get() << std::endl;

    std::unique_ptr<Logger> logPtr3 = Singleton<Logger>::GetInstance();
    std::cout << " logPtr1: " << logPtr1.get() << " logPtr2: " << logPtr2.get() <<
    " logPtr3: " << logPtr3.get() << std::endl;


    return 0;
}