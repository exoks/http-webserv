/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IClient Interface ]                                                 #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __ICLIENT_HPP__
# define __ICLIENT_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

/*******************************************************************************
    * http::IClient Interface :
*******************************************************************************/

namespace http
{
    class IClient
    {
    public:
        typedef mem::shared_ptr<IClient>    SharedPtr;

        virtual	~IClient(void) {};

        virtual void               updateActivityTime(void) = 0;

        virtual const Handle&      getSocket(void) const = 0;
        virtual const string&      getInfo(void) const = 0;
        virtual time_t             getLastActivityTime(void) const = 0;

        virtual string             recv(void) const = 0;
        virtual ssize_t            send(const string& aData) const = 0;
    };
}

#endif	/* __ICLIENT_HPP__ */