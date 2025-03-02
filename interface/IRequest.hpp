/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IRequest Interface ]                                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef	  __IREQUEST_HPP__
# define   __IREQUEST_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IServer.hpp"

/*******************************************************************************
    * http::IRequest Interface :
*******************************************************************************/

namespace http
{
    class IRequest
    {
    public:
        typedef mem::shared_ptr<IRequest>           SharedPtr;
        typedef std::vector<IServer::SharedPtr>     ServerVector;

        virtual ~IRequest(void) {};

        virtual void      selectMatechedRoute(const ServerVector& aServers) = 0;
        virtual void      setHeader(string& aLine) = 0;
        virtual void      setBodyPath(const string& aPath) = 0;
        virtual void      uriAppend(const string& path) = 0;
        virtual bool      hasBody(void) const = 0;

        virtual string            getVersion(void) const = 0;
        virtual string		      getMethod(void) const = 0;
        virtual string		      getUriPath(void) const = 0;
        virtual string	          getUriQuery(void) const = 0;
        virtual const string&     getHeader(string const& key) const = 0;
        virtual const IServer&    getMatchedServer(void) const = 0;
        virtual const Location&   getMatchedLocation(void) const = 0;
        virtual const string&     getBodyPath(void) const = 0;

        virtual void   display(void) const = 0;
    };
}

#endif	/* __IREQUEST_HPP__ */