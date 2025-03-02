/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpClient implementation file ]                                     #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-06                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpClient.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: Client ]====================================================
http::Client::Client(Handle aSocket,
                     const sockaddr_in& aAddr,
                     socklen_t aAddrLen)
    : mSocket(aSocket),
      mLastActivityTime(time(NULL))
{
    mInfo += str::addrtoString(aAddr.sin_addr.s_addr, aAddrLen);
    mInfo += ":" + str::to_string(ntohs(aAddr.sin_port));
}

//===[ Destructor: Client ]=====================================================
http::Client::~Client(void)
{
    http::Cluster::eraseServers(mSocket);
    close(mSocket);
    Logger::log("notice ", "HTTP: Client '" + mInfo + "' disconnected", 1);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: updateActivityTime ]=============================================
void http::Client::updateActivityTime(void)
{
    mLastActivityTime = time(NULL);
}

//===[ Method: recv ]===========================================================
string http::Client::recv(void) const
{
    char buffer[2048];
	memset(buffer, 0, 2048);

	ssize_t bytesReaded = ::recv(mSocket, buffer, 2048, 0);
	if (bytesReaded < 0)
    {
        std::string msg = "Error while receiving data";
        throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR)); 
    }
    return (string(buffer, bytesReaded));
}


//===[ Method: send ]===========================================================
ssize_t http::Client::send(const std::string& aData) const
{
    return (::send(mSocket, aData.c_str(), aData.size(), 0));
}

/*** * Getters :
*******************************************************************************/

//===[ Method: getSocket ]======================================================
const Handle& http::Client::getSocket(void) const
{
    return (mSocket);
}

//===[ Method: getInfo ]========================================================
const std::string& http::Client::getInfo(void) const
{
    return (mInfo);
}

//===[ Method: getLastActivityTime ]============================================
time_t http::Client::getLastActivityTime(void) const
{
    return (mLastActivityTime);
}
