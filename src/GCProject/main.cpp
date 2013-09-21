#include <iostream>
#include "GCPtr.cpp"

using namespace std;

void createSimplePointer()
{

	float *fptr=new float[5];
	GCPtr<float,5> arr;
    arr=fptr;
	GCPtr<float,5> farr;
	farr=arr;

	printf("\npointer:%u memberpointer:%u\n",fptr,arr.memberPointer);

    /**
    Traversing the array through iterator.
     */
    GCPtr<float>::GCIterator iter=arr.begin();
    GCPtr<float>::GCIterator end=arr.end();

    int i=0;
    for(i=0;i<iter.size();i++)
    {
        iter[i]=i;
    }
    printf("\nprint the value..\n");
    iter=arr.begin();

    /**
    some confution regarding the way this iter opertaor (!=) overloading is working.
   */
    for(;end!=iter;++iter)
    {
       printf("%f ",*iter);
    }

}


template<class T,int size> list< GCInfo<T> >  GCPtr<T,size>::listPtr;
template<class T,int size> bool  GCPtr<T,size>::firstTime=true;

int main()
{
    printf("\nGC Demo Project\n");
	createSimplePointer();
	return 0;
}
