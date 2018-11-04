#pragma once
#include <mutex>

template <typename T>
class Singleton
{
public:
	static T* Instance()
	{
		std::call_once(initFlag, create);
		return instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

private:
	static void create()
	{
		instance = new T;
		std::atexit(&Singleton<T>::destroy);
	}

	static void destroy()
	{
		delete instance;
	}

	static std::once_flag initFlag;
	static T* instance;
};

template <typename T> std::once_flag Singleton<T>::initFlag;
template <typename T> T* Singleton<T>::instance = nullptr;