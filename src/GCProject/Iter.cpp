#include<cstdio>
using namespace std;

template<class T>
class Iter {
    T *ptr;
    T *begin;
    T *end;
    unsigned length;
public:
	Iter()
	{
		//printf("\ninside the iter constructor.");
		ptr=begin=end=NULL;
		length=0;
	}
	Iter(T *p,T *s, T *e)
	{
	    ptr=p;
	    begin=s;
	    end=e;
	    length=end-begin;
    }
	virtual ~Iter()
	{
		//printf("\ninside the destructor..\n");
	}

	unsigned size()
	{
       return length;
    }

	/**
	some operator overloading.
	*/
	T &operator*()
	{

	   return *ptr;
    }
    Iter operator++()
    {
       ptr++;
       return *this;
    }

    Iter operator--()
    {
       ptr--;
       return *this;
    }
    T &operator[](int index)
    {

       /* if(ptr+index>=end||ptr+index<begin)
        return temp;
        else*/
        return ptr[index];
    }
    bool operator==(Iter<T> &o)
    {
        return (this->ptr==o.ptr);
    }
    bool operator!=(Iter<T> &o)
    {
        return (this->ptr!=o.ptr);
    }
};

/**
END OF Iter CLASS.
*/
