/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ shared_ptr header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-06-26                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __SHARED_PTR_HPP__
# define   __SHARED_PTR_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include <cstddef>

/*******************************************************************************
    * shared_ptr Definition :
*******************************************************************************/
namespace mem
{
    template<typename T>
    class shared_ptr
    {
    public:
        shared_ptr(T* aPtr = NULL);
        shared_ptr(const shared_ptr<T>& aOther);
        virtual	~shared_ptr(void);

        size_t          use_count(void);
        T*              get(void);

        shared_ptr<T>&  operator=(const shared_ptr<T>& aOther);
        T&              operator*(void);
        T*              operator->(void);
        const T&        operator*(void) const;
        const T*        operator->(void) const;
        operator        bool(void) const;


    private:
        T*      mPtr;
        size_t* mRef_count;
    };
};

/******************************************************************************
    *  Shared_ptr Implementation :
******************************************************************************/

/*** * Constructor :
******************************************************************************/

//==[ shared_ptr: Constructor ]================================================
template<typename T>
mem::shared_ptr<T>::shared_ptr(T* aPtr)
    : mPtr(NULL), mRef_count(NULL)
{
    if (aPtr != NULL)
    {
        mPtr = aPtr;
        mRef_count = new size_t(1);
    }
}

//==[ shared_ptr: Copy Constructor ]===========================================
template<typename T>
mem::shared_ptr<T>::shared_ptr(const shared_ptr<T>& aOther)
    : mPtr(NULL), mRef_count(NULL)
{
    if (aOther.mPtr != NULL)
    {
        mPtr = aOther.mPtr;
        mRef_count = aOther.mRef_count;
        ++(*mRef_count);
    }
};

//==[ shared_ptr: Destructor ]=================================================
template<typename T>
mem::shared_ptr<T>::~shared_ptr(void)
{
    if (mRef_count != NULL)
    {
        if (--(*mRef_count) == 0)
        {
            delete mPtr;
            delete mRef_count;
        }
    }
}

/*** * Public Methods :
*******************************************************************************/

//==[ Method : number owners ]=================================================
template<typename T>
size_t mem::shared_ptr<T>::use_count(void)
{
    return ((mRef_count != NULL) ? (*mRef_count) : (0));
}

//==[ Method : get pointer ]===================================================
template<typename T>
T* mem::shared_ptr<T>::get(void)
{
    return (mPtr);
}

/*** * Operators :
*******************************************************************************/

//==[ shared_ptr: operator= ]=================================================
template<typename T>
mem::shared_ptr<T>& mem::shared_ptr<T>::operator=(const shared_ptr<T>& aOther)
{
	if (mPtr != aOther.mPtr)
	{
		if (mRef_count && --(*mRef_count) <= 0)
		{
			delete (mRef_count);
			delete (mPtr);
		}
		mPtr = aOther.mPtr;
		mRef_count = aOther.mRef_count;
		if (mRef_count != NULL)
			++(*mRef_count);
	}
	return (*this);
}

//==[ shared_ptr: operator* ]==================================================
template<typename T>
T& mem::shared_ptr<T>::operator*(void)
{
    return (*mPtr);
}

//==[ shared_ptr: operator-> ]=================================================
template<typename T>
T* mem::shared_ptr<T>::operator->(void)
{
    return (mPtr);
}

//==[ shared_ptr: operator* ]==================================================
template<typename T>
const T& mem::shared_ptr<T>::operator*(void) const
{
    return (*mPtr);
}

//==[ shared_ptr: operator-> ]=================================================
template<typename T>
const T* mem::shared_ptr<T>::operator->(void) const
{
    return (mPtr);
}

//==[ shared_ptr: operatorbool ]===============================================
template<typename T>
mem::shared_ptr<T>::operator bool(void) const
{
    return (mPtr != NULL);
}

#endif /* __SHARED_PTR_HPP__ */
