/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpCluster implementation File ]                                   #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-03                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpCluster.hpp"

/*******************************************************************************
    * Static Variables :
*******************************************************************************/
http::Cluster::ServerMap http::Cluster::mServers;

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: Cluster ]===================================================
http::Cluster::Cluster(Directive::SharedPtr aHttpDir)
{

    DirPtrVector serverDir = aHttpDir->getNonTerminal("server");
    if (serverDir.size() == 0)
        throw (std::invalid_argument("HTTP: No server directive found"));

    for (size_t index = 0; index < serverDir.size(); ++index)
    {
        serverDir[index]->copyMatchingAttributes(aHttpDir);
        _buildServer(serverDir[index]);
    }

    StringVector tmpDir =  aHttpDir->getTerminal("tmp_dir");
    http::tmpDir((tmpDir.size() != 0) ? tmpDir[0] : "/tmp");
}

//===[ Destructor: Cluster ]====================================================
http::Cluster::~Cluster(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ QueueOfHandler: Cluster ]================================================
IEventHandler::IEventHandlerQueue http::Cluster::createHandlers(void)
{
    IEH::IEventHandlerQueue     Handlers;
    ServerMap::iterator         it = mServers.begin();
    while (it != mServers.end())
    {
        Handlers.push(http::Factory::createAcceptHandler(it->first));
        ++it;
    }
    return (Handlers);
}

/*******************************************************************************
    * Static Methods :
*******************************************************************************/

//===[ Method: get Servers ]====================================================
const http::Cluster::ServerVector& http::Cluster::getServers(Handle aSocket)
{
    return (mServers[aSocket]);
}

//===[ Method: set Servers ]====================================================
void http::Cluster::setServers(Handle aSocket, const ServerVector& aServers)
{
    mServers[aSocket] = aServers;
}

//===[ Method: erase Servers ]==================================================
void http::Cluster::eraseServers(Handle aSocket)
{
    mServers.erase(aSocket);
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: Check if socket is created ]====================================
bool http::Cluster::_isSocketReadyCreated(const string& aListen) const
{
    return (mSockets.find(aListen) != mSockets.end());
}

//===[ Method: Register Server ]===============================================
void http::Cluster::_buildServer(Directive::SharedPtr aServerDir)
{
    http::IServer::SharedPtr server = http::Factory::createServer(aServerDir);
    StringVector listenSet = server->getListens();
    for (size_t index = 0; index < listenSet.size(); ++index)
    {
        std::string listen = listenSet[index];
        if (_isSocketReadyCreated(listen) == false)
        {
            Handle socketfd = 0;
            if ((socketfd = http::Factory::createSocket(listen)) == -1)
                continue;
            mSockets[listen] = socketfd;
        }
        mServers[mSockets[listen]].push_back(server);
    }
}
