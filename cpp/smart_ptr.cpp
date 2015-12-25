/**
 * Smart (shared) pointer
 */
#include <iostream>
#include <sstream>
#include <memory>


template <typename T>
class smart_ptr
{
#define DBG_PRINT(STR) \
	std::cout << this << '(' << m_pObj; \
	if(m_pObj) std::cout << ", " << *m_pRef << " = " << *m_pObj; \
	std::cout << "): " << STR << std::endl;

public:
	smart_ptr(T* f_pObj = NULL):
		m_pObj(f_pObj),
		m_pRef(f_pObj ? new unsigned int(1) : NULL)
	{
		DBG_PRINT("ctor");
	}
	smart_ptr(const smart_ptr& f_ptr)
	{
		copy(f_ptr);
		DBG_PRINT("copy ctor " << &f_ptr);
	}
	smart_ptr& operator=(const smart_ptr& f_ptr)
	{
		DBG_PRINT("op= " << &f_ptr);
		decRef();
		copy(f_ptr);
	}

	~smart_ptr()
	{
		DBG_PRINT("dtor");
		decRef();
	}

	T& operator*() const
	{
		DBG_PRINT("op*");
		return *m_pObj;
	}
	T* operator->() const
	{
		DBG_PRINT("op->");
		return m_pObj;
	}

private:
	void decRef()
	{
		DBG_PRINT("dec ref");
		if(m_pRef && --*m_pRef == 0)
		{
			DBG_PRINT("delete");
			delete m_pObj;
			delete m_pRef;
			m_pObj = NULL;
			m_pRef = NULL;
		}
	}

	void copy(const smart_ptr& f_ptr)
	{
		m_pObj = f_ptr.m_pObj;
		m_pRef = f_ptr.m_pRef;
		if(m_pRef)
			++*m_pRef;
		DBG_PRINT("copied " << &f_ptr);
	}

private:
	T* m_pObj;
	unsigned int* m_pRef;
};

// A class for smart pointer testing
class CTest
{
public:
	const std::string& func() const
	{
		static const std::string s_str("func");
		return s_str;
	}
	friend std::ostream& operator<<(std::ostream& f_os, const CTest&) { f_os << "<class>"; return f_os; }
};

// ====================================
int main(int, char**)
{
	// Basic type
	{
		int* x;
		x = new int(7);

		std::cout << "> smart_ptr()" << std::endl;
		smart_ptr<int> p0;
		p0 = p0;
		std::cout << "> smart_ptr(int*)" << std::endl;
		smart_ptr<int> p1(x);
		std::cout << "> smart_ptr(smart_ptr)" << std::endl;
		smart_ptr<int> p2(p1);
		std::cout << "> smart_ptr=" << std::endl;
		smart_ptr<int> p3; p3 = p2;
		{
			std::cout << "{" << std::endl;
			std::cout << "> smart_ptr(new int)" << std::endl;
			smart_ptr<int> p4(new int(13));
			std::cout << "> existing smart_ptr=" << std::endl;
			p1 = p4;
			std::cout << "}" << std::endl;
		}
		std::cout << "> *smart_ptr" << std::endl;
		int& a = *p1;
		std::cout << a << std::endl;

		std::cout << "> done" << std::endl;
	}
	// User class
	{
		std::cout << std::endl;

		std::cout << "> smart_ptr(class*)" << std::endl;
		smart_ptr<CTest> p0(new CTest);
		std::cout << "> smart_ptr->" << std::endl;
		const std::string& s = p0->func();
		std::cout << s << std::endl;

		std::cout << "> done" << std::endl;
	}

	return 0;
}

