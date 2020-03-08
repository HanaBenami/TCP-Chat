// uncopyable.cpp                                                       
// Last update: 12/02/2020                                                      
// Author: Hana
/*----------------------------------------------------------------------------*/
#ifndef __UNCOPYABLE_HPP__
#define __UNCOPYABLE_HPP__
/*----------------------------------------------------------------------------*/
class Uncopyable
{
protected:
	explicit Uncopyable();
	virtual ~Uncopyable();
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};

inline Uncopyable::Uncopyable()
{ }

inline Uncopyable::~Uncopyable()
{ }
/*----------------------------------------------------------------------------*/
#endif // __UNCOPYABLE_HPP__ 
