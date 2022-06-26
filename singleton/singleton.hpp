
//----------------------------------------------------------------------------//
//					Design Patterns
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		API - Singleton DP
//	AUTHOR 			Liad Raz
//	DATE			19 Apr 2022
//
//----------------------------------------------------------------------------//

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <mutex>	// mutex
#include <atomic>	// atomic
#include <memory>	// uniue

/*		
	Singleton Class Requirements
	----------------------------
    1. Uncopiable.
    2. Generic - template.
    3. Delayed initialisation (lazy loading).
    4. Thread safe
    5. DCLP - Double-Checked Locking is Fixed

 
	User Uses
	---------------------------
	Follow the instructions when definining a singleton class ==>
    1. ~Dtor = default
    2. default Ctor = private
    3. Singleton class, friend
    4. Pointer of type <T> is of unique_ptr type.
*/


//----------------------------------------------------------------------------//
//	Singleton Deceleration
//----------------------------------------------------------------------------//
template <typename T>
class Singleton
{
public:
	//
	// Special constructors
	// NOTE 	All ctors are Blocked to use by the user.
	Singleton() = delete;
	~Singleton() = delete;
	// Uncopiable
	Singleton(const Singleton& other_) = delete;
	Singleton& operator=(const Singleton& other_) = delete;

	//
	static T* GetInstance();

private:
	static std::atomic<T*> s_m_instance;
	static std::mutex s_m_lock;
};

// Init static members
template <typename T>
std::atomic<T*> Singleton<T>::s_m_instance(nullptr);

template <typename T>
std::mutex Singleton<T>::s_m_lock;


//----------------------------------------------------------------------------//
//	Singleton Class Definitions
//----------------------------------------------------------------------------//

template <typename T>
T* Singleton<T>::GetInstance()
{
	// std::memory_order ==>
	// specifies how memory accesses are to be ordered around an atomic operation.
	// memory_order_relaxed - there are no synchronization or ordering 
	// constraints imposed on other reads or writes.
	T* tmp = s_m_instance.load(std::memory_order_relaxed);
	
	//
	// << atomic_thread_fence, Establishes memory synchronization ordering >>
	// A relaxed atomic load immediately followed by an acquire fence will 
	// convert that load into a read-acquire.
	// std::atomic_thread_fence(std::memory_order_acquire); 

	if (tmp == nullptr)
	{
		// In case this is the first instance creation a Mutex will be activated.
		
		// lock_guard, is a mutex wrapper that provides a convenient RAII-style mechanism. 
		std::lock_guard<std::mutex> lock(Singleton<T>::s_m_lock);
		tmp = s_m_instance.load(std::memory_order_relaxed);

		if (tmp == nullptr)
		{
			// auto tmp = std::unique_ptr<T>();
			tmp = new T;
			// To keep the class as RAII and in order to invoke dtor
			// Solutions :
			// 1. static std::unique_ptr<T> tempPtr(tmp);
			// 2. create a Guard class wrapper which will use delete in its dtor which deallocate the instance.

			// A relaxed atomic store immediately preceded by a release fence 
			// will convert that store into a write-release.
			// std::atomic_thread_fence(std::memory_order_release);

			s_m_instance.store(tmp.get(), std::memory_order_relaxed);
		}
	}

	return tmp;
}



#endif // SINGLETON_HPP



/*
	DCLP
	----
	Once the singleton is created, there isn’t really any need for a lock anymore.
	The double-checked locking pattern avoids this lock. 
*/




