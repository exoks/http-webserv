/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpRequest header file ]                                           #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-09                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __HTTPREQUEST_HPP__
# define __HTTPREQUEST_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IServer.hpp"
# include "IRequest.hpp"

# include "HttpException.hpp"

# include <iostream>

/*******************************************************************************
    * Class Request :
*******************************************************************************/
namespace http
{

    class Request : public IRequest
    {
    public:
        Request(string& aBuffer);
        virtual ~Request(void);
        
        void            selectMatechedRoute(const ServerVector& aServerList);
        void            setHeader(std::string& aLine);
        void            setBodyPath(const string& aPath);
        void            uriAppend(const string& path);
        bool            hasBody(void) const;
     
        string          getVersion(void) const;
        string          getMethod(void) const;
        string          getUriPath(void) const;
        const string&   getHeader(string const& key) const;
        string          getUriQuery(void) const;
        const IServer&  getMatchedServer(void) const;
        const Location& getMatchedLocation(void) const;
        const string&   getBodyPath(void) const;
     
        void            display(void) const;

    private:
        IServer::SharedPtr  mMatchedServer;
        Location::SharedPtr mMatchedLocation;
        string		        mVersion;
        string		        mMethod;
        string		        mUri;	
        StringMap		    mHeaders;
        string		        mQuery;
        string              mBodyPath;
    
        bool _isInnerPath(const std::string& uri, const std::string& requestUri);
        void _setRequestLine(const std::string& aLine);
        string& _parseUri(std::string& aUri);
        void _parseQuery(std::string& aUri);
        Location::SharedPtr _selectMatchedLocation(void);
        IServer::SharedPtr _selectMatchedServer(const ServerVector& aServerList);
    };
};

#endif /* __HTTPREQUEST_HPP__ */