
/**
START OF GCInfo CLASS
*/
template<class T> class GCInfo {

public:
	T *memberPtr;
	unsigned  referenceCount;
	bool isArray;
	unsigned  arraySize;

	GCInfo(T *ptr,unsigned size=0)
	{

		referenceCount=1;
		memberPtr=ptr;
		isArray=false;
		if(size>0)
		isArray=true;
		arraySize=size;
	}


	virtual ~GCInfo()
	{
	}

	bool operator==(const GCInfo &obj)
	{
	    return (this->memberPtr==obj.memberPtr);
    }
};

/**
END OF GCInfo CLASS.
*/
