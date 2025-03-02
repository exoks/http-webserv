//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀       𓐓  AcceptHandler.cpp 𓐔
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma>
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/11 15:27:14 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/07/06 15:31:56 by hael-mou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀

# include "HttpAcceptHandler.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: AcceptHandler ]=============================================
http::AcceptHandler::AcceptHandler(Handle aHandle)
    : mHandle(aHandle)
{
    mTerminated = (aHandle == -1) ? true : false;
}

//===[ Destructor: AcceptHandler ]==============================================
http::AcceptHandler::~AcceptHandler(void)
{
    close(mHandle);
    Logger::log("notice ","HTTP: Closing Socket["
        + str::to_string(mHandle)
        + "], Done.", 1);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: handle The Accept Events ]=======================================
IEventHandler::IEventHandlerQueue  http::AcceptHandler::handleEvent(void)
{
    IEventHandlerQueue   eventHandlers;
    socklen_t            clientAddrLen;
    struct sockaddr_in   clientAddr;
    Handle               clientSocket;

    while (true)
    {
        http::IClient::SharedPtr client;

        clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(mHandle, (sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1)
            break;
        sock::setNonBlocking(clientSocket);
        client = http::Factory::createClient(clientSocket,
                                             clientAddr,
                                             clientAddrLen);
        if (client.get() == NULL)
        {
            close(clientSocket);
            continue;
        }

        Logger::log("notice ","HTTP: New Client '" + client->getInfo()
                  + "' connected", 1);
        const ServerVector& servers = http::Cluster::getServers(mHandle);
        http::Cluster::setServers(client->getSocket(), servers);
        eventHandlers.push(http::Factory::createRecvHandler(client));
    }
    return (eventHandlers);
}

//===[ Method: return Handler Mode ]============================================
IMultiplexer::Mode  http::AcceptHandler::getMode(void) const
{
    return (IMultiplexer::Read);
}

//===[ Method: return Handler Handle ]==========================================
const Handle&  http::AcceptHandler::getHandle(void) const
{
    return (mHandle);
}

//===[ Method: check if Handler is Terminated ]=================================
bool  http::AcceptHandler::isTerminated(void) const
{
    return (mTerminated);
}
