/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ ServerCore header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-28                                                   #
#                                                                              #
** ************************************************************************* **/

#include "ServerCore.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: getInstance ]===============================================
ServerCore::SharedPtr  ServerCore::getInstance(void)
{
    static ServerCore::SharedPtr instance = new ServerCore();
    return (instance);
}

//===[ Constructor: ServerCore ]================================================
ServerCore::ServerCore(void)
{
    IMultiplexer* multiplexer = new SelectMultiplexer();
    mReactor = new Reactor(multiplexer);
    mIsRunning = false;

    mProtocolFactoryMap["http"] = new http::Factory();
}

//===[ Destructor: ServerCore ]=================================================
ServerCore::~ServerCore(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: init Start Servers Handler ]=====================================
void ServerCore::setup(Directive::SharedPtr aGlobalDir)
{
    IProtocolFactoryMap::iterator it = mProtocolFactoryMap.begin();

    while (it != mProtocolFactoryMap.end())
    {
        _setupProtocol(it->first, it->second, aGlobalDir);
        ++it;
    }
}

//===[ Method: run Start Servers Handler ]======================================
void	ServerCore::run(void)
{
    while (mIsRunning)
    {
        mReactor->handleEvents(3000);
        mReactor->cleanupTerminatedHandlers();
    }
}

//===[ Method: stop all Servers ]===============================================
void	ServerCore::stop(void)
{
    mIsRunning = false;
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: setup Protocol ]================================================
void ServerCore::_setupProtocol(const string& aProtocolName,
                               IProtocolFactoryPtr aProtocolFactory,
                               Directive::SharedPtr aGlobalDir)
{
    try
    {
        DirPtrVector protocolDir = aGlobalDir->getNonTerminal(aProtocolName);
        if (protocolDir.size() > 1)
        {
            Logger::log("warning", aProtocolName + ": Just first "
                + aProtocolName + " directive is used", 2);
        }
        if (protocolDir.size() == 0)
            return ;
        ICluster *cluster = aProtocolFactory->createCluster(protocolDir.front());
        IEH::IEventHandlerQueue Handlers = cluster->createHandlers();
        if (Handlers.size() == 0)
            return ;
        mReactor->registerEventHandler(Handlers);
        delete (cluster);
        mIsRunning = true;
    }
    catch(const std::exception& e)
    {
        Logger::log("error  ", e.what(), 2);
    }
}
