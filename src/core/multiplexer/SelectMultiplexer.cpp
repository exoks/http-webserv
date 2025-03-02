/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ SelectMultiplexer implementation file ]                              #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#include "SelectMultiplexer.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor : ]==========================================================
SelectMultiplexer::SelectMultiplexer(void)
    : mMaxHandle(0)
{
    FD_ZERO(&mReadSet);
    FD_ZERO(&mWriteSet);
}

//===[ Destructor : ]===========================================================
SelectMultiplexer::~SelectMultiplexer(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method : Wait for events ]===============================================
HandleQueue SelectMultiplexer::waitEvent(long long aTimeout_ms)
{
    timeval		timeout;
    timeout.tv_sec = aTimeout_ms / 1000;
    timeout.tv_usec = (aTimeout_ms % 1000) * 1000;

    fd_set readSet = mReadSet;
    fd_set writeSet = mWriteSet;

    ::select(mMaxHandle + 1, &readSet, &writeSet, NULL, &timeout);
    return (_collectReadyHandles(readSet, writeSet));
}

//===[ Method : register handle ]===============================================
void SelectMultiplexer::registerHandle(const Handle& aHandle, Mode aMode)
{
    fd_set& targetSet = ((aMode == IMultiplexer::Read) ? mReadSet : mWriteSet);
    FD_SET(aHandle, &targetSet);
    mMaxHandle = (mMaxHandle > aHandle) ? mMaxHandle : aHandle;
}

//===[ Method : remove handle ]================================================
void SelectMultiplexer::removeHandle(const Handle& aHandle, Mode aMode)
{
    fd_set& targetSet = ((aMode == IMultiplexer::Read) ? mReadSet : mWriteSet);
    FD_CLR(aHandle, &targetSet);
    _updateMaxHandleFromSets();
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method : collect ready handles ]=========================================
HandleQueue
SelectMultiplexer::_collectReadyHandles(fd_set& aReadSet, fd_set& aWriteSet)
{
    HandleQueue		readyHandles;
    for (int iHandle = 0 ; iHandle < mMaxHandle + 1 ; ++iHandle)
    {
        if (FD_ISSET(iHandle, &aReadSet) || FD_ISSET(iHandle, &aWriteSet))
            readyHandles.push(iHandle);
    }
    return (readyHandles);
}

//===[ Method : update max handle from sets ]===================================
void SelectMultiplexer::_updateMaxHandleFromSets(void)
{
    int newMaxFd = 0;

    for (int iHandle = 0 ; iHandle < mMaxHandle + 1 ; iHandle++)
    {
        if (FD_ISSET(iHandle, &mReadSet) || FD_ISSET(iHandle, &mWriteSet))
            newMaxFd = iHandle;
    }
    mMaxHandle = newMaxFd;
}
