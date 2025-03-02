/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IServer Interface ]                                                 #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __ISERVER_HPP__
# define   __ISERVER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IResponse.hpp"
# include "HttpLocation.hpp"

/*******************************************************************************
    * http::IServer Interface :
*******************************************************************************/

namespace http
{
    class IServer
    {
    public:
        typedef mem::shared_ptr<IServer>                SharedPtr;
        typedef std::map<string, Location::SharedPtr>   LocationMap;


        virtual	~IServer(void) {};

        virtual bool                  isKeepAlive(void) const = 0;
        virtual bool 				  isMatch(const string& aHost) const = 0;

        virtual const StringVector&   getListens(void) const = 0;
        virtual const StringVector&   getServerNames(void) const = 0;
        virtual time_t                getKeepAliveTimeout(void) const = 0;
        virtual time_t                getReadTimeout(void) const = 0;
        virtual time_t                getSendTimeout(void) const = 0;
        virtual time_t                getCgiTimeout(void) const = 0;
        virtual unsigned long         getMaxBodySize(void) const = 0;
        virtual const string&         getMimeType(const string& aExtansion) const = 0;
        virtual IResponse::SharedPtr  getErrorPage(u_int aCode) const = 0;
        virtual const LocationMap&    getLocations(void) const = 0;
    };
}

#endif	/* __ISERVER_HPP__ */