/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpServer header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-06-02                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef	  __HTTPSERVER_HPP__
# define   __HTTPSERVER_HPP__
/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "Directive.hpp"
# include "HttpErrorPage.hpp"
# include "IServer.hpp"
# include "HttpLocation.hpp"

/*******************************************************************************
    * DEFAULT VALUES :
*******************************************************************************/
# define   DEFAULT_LISTEN           "0.0.0.0:80"
# define   DEFAULT_MIME_TYPE        "application/octet-stream"
# define   DEFAULT_SERVER_ROOT      "./"
# define   KL_TIMEOUT               6
# define   READ_TIMEOUT             60
# define   SEND_TIMEOUT             600
# define   CGI_TIMEOUT              120
# define   _1_MB_                   1048576

/*******************************************************************************
    * PorotocolFactory Class :
*******************************************************************************/
namespace http
{
	class Server : public IServer
	{
    public:
        typedef std::map<string, http::Location::SharedPtr> LocationMap;

        Server(Directive::SharedPtr aServerDir);
        virtual ~Server(void);

        bool    isMatch(const string& aHostName) const;
        bool    isKeepAlive(void) const;
        time_t  toTime(const StringVector& aTime, time_t aDefault) const;

        void    setListens(const StringVector& aListens);
        void    setServerNames(const StringVector& aServerName);
        void    setMaxBodySize(const StringVector& aMaxBodySize);
        void    setDefaultMimeType(const StringVector& aDefaultType);
        void    setMimeTypes(const StringVector& aTypes);
        void    setServerRoot(const StringVector& aRoot);
        void    setLocations(Directive::SharedPtr aServerDir,
                                Directive::DirPtrVector aLocation);

        const StringVector&       getListens(void) const;
        const StringVector&       getServerNames(void) const;
        unsigned long             getMaxBodySize(void) const;
        time_t                    getKeepAliveTimeout(void) const;
        time_t                    getReadTimeout(void) const;
        time_t                    getSendTimeout(void) const;
        time_t                    getCgiTimeout(void) const;
        const string&             getMimeType(const string& aExtansion) const;
        IResponse::SharedPtr      getErrorPage(u_int aCode) const;
        const LocationMap&        getLocations(void) const;

    private:
	    StringVector    mListens;
	    StringVector    mServerName;
        string          mRoot; 
        bool            mKeepAlive;
        time_t          mTimeout[4];
        unsigned long   mMaxBodySize;
        StringMap       mMimeTypes;
        ErrorPages      mErrorPages;
        LocationMap     mLocations;
    };
}

#endif /* __HTTPSERVER_HPP__ */