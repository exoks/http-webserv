/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpSendHandler implementation file ]                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpSendHandler.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: SendHandler ]===============================================
http::SendHandler::SendHandler(IClient::SharedPtr aClient,
                               IResponse::SharedPtr aResponse)
    : mClient(aClient),
      mResponse(aResponse)
{
    mTerminated = (mClient && mResponse) ? false : true;
}

//===[ Destructor: SendHandler ]================================================
http::SendHandler::~SendHandler(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ getHandle: return Handler Handle ]========================================
const Handle& http::SendHandler::getHandle(void) const
{
    return (mClient->getSocket());
}

//===[ getMode: return Handler Mode ]============================================
IMultiplexer::Mode http::SendHandler::getMode(void) const
{
    return (IMultiplexer::Write);
}

//===[ handleEvent: return IEventHandlerQueue ]=================================
IEventHandler::IEventHandlerQueue http::SendHandler::handleEvent(void)
{
    IEventHandlerQueue eventHandlers;

    ssize_t bytesSent = mClient->send(mResponse->toRaw());
    if  (bytesSent == -1)
    {
        Logger::log("error  ", "HTTP: Failed to send !!", 2);
        mTerminated = true;
        return (eventHandlers);
    }

    mResponse->removeBytesSent(bytesSent);
    if (mResponse->eof())
    {
        mResponse->log(mClient->getInfo());

        if (mResponse->getHeader("connection") != "close") {
            eventHandlers.push(http::Factory::createRecvHandler(mClient));
        }
        else {
            mTerminated = true;
        }
    }

    mClient->updateActivityTime();
    return (eventHandlers);
}

//===[ isTerminated: return Handler Status ]====================================
bool http::SendHandler::isTerminated(void) const
{
    if (mTerminated == true)
        return (true);

    time_t lastActivity = mClient->getLastActivityTime();
    time_t timeout      = mResponse->getSendTimeout();
    time_t currentTime  = time(NULL);

    return (currentTime - lastActivity >= timeout);
}
