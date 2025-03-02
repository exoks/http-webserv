/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Reactor implementation ]                                            #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-16                                                   #
#                                                                              #
** ************************************************************************* **/

#include "Reactor.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor : ]========================================================
Reactor::Reactor(IMultiplexer *aMultiplexer)
    : mMultiplexer(aMultiplexer)
{
    if (aMultiplexer == NULL)
    {
        std::string error = "Reactor: Multiplexer failed to initialize!";
        throw (std::runtime_error(error));
    }
}

//===[ Destructor : ]=========================================================
Reactor::~Reactor(void) {}

/*******************************************************************************
    * public Methods :
*******************************************************************************/

//===[ Method : register new event handler ]====================================
void Reactor::registerEventHandler(IEH::SharedPtr aEventHandler)
{
    if (aEventHandler.get() != NULL)
    {
        Handle handle = aEventHandler->getHandle();
        IEventHandlerMap::iterator it = mEventHandlers.find(handle);

        if (it != mEventHandlers.end()) {
            unregisterEventHandler(it->second);
        }

        mEventHandlers[handle] = aEventHandler;
        mMultiplexer->registerHandle(handle, aEventHandler->getMode());
    }
}

//===[ Method : register queue of event handlers ]==============================
void Reactor::registerEventHandler(IEventHandlerQueue& aHandlers)
{
    while (aHandlers.empty() == false)
    {
        registerEventHandler(aHandlers.front());
        aHandlers.pop();
    }
}

//===[ Method : unregister event handler ]=====================================
IEventHandler::SharedPtr
Reactor::unregisterEventHandler(IEH::SharedPtr aHandler)
{
    if (aHandler.get() != NULL)
    {
        const Handle& handle = aHandler->getHandle();
        mMultiplexer->removeHandle(handle, aHandler->getMode());
        mEventHandlers.erase(handle);
    }
    return (aHandler);
}

//===[ Method : remove all ended event handlers ]==============================
void Reactor::cleanupTerminatedHandlers(void)
{
    IEventHandlerMap::iterator handlerIt = mEventHandlers.begin();
    while (handlerIt != mEventHandlers.end())
    {
        IEH::SharedPtr handler = handlerIt->second;
        if (handler->isTerminated() == true)
        {
            mMultiplexer->removeHandle(handler->getHandle(),handler->getMode());
            IEventHandlerMap::iterator toErase = handlerIt++;
            mEventHandlers.erase(toErase);
        }
        else
        {
            ++handlerIt;
        }
    }
}

//===[ Method : handle events ]================================================
void Reactor::handleEvents(long long aTimeout_ms)
{
    HandleQueue readyHandles = mMultiplexer->waitEvent(aTimeout_ms);
    while (readyHandles.empty() == false)
    {
        Handle            handle = readyHandles.front();
        IEventHandlerQueue newHandlers = mEventHandlers[handle]->handleEvent();
        registerEventHandler(newHandlers);
        readyHandles.pop();
        usleep(500);
    }
}
