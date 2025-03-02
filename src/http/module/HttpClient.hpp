/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpClient header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-06                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef	  __HTTPCLIENT_HPP__
# define   __HTTPCLIENT_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IClient.hpp"
# include "HttpCluster.hpp"

/*******************************************************************************
    * Client Class :
*******************************************************************************/
namespace http
{
    class Client : public IClient
    {
    public:
        Client(Handle aSocket, const sockaddr_in &aAddr, socklen_t aAddrLen);
        virtual ~Client(void);

        string              recv(void) const;
        ssize_t             send(const std::string& aData) const;
        void                updateActivityTime(void);

        const Handle&       getSocket(void) const;
        const string&       getInfo(void) const;
        time_t              getLastActivityTime(void) const;

    private:
        Handle             mSocket;
        string             mInfo;
        time_t             mLastActivityTime;

    };
}

#endif /* __HTTPCLIENT_HPP__ */