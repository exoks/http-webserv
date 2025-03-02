/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpErrorPage implementation file ]                                  #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpErrorPage.hpp"

/*******************************************************************************
	* Construction :
*******************************************************************************/

//===[ Constructor: ErrorPages ]================================================
http::ErrorPages::ErrorPages(void) {}

//===[ Constructor: ErrorPages ]================================================
http::ErrorPages::ErrorPages(const StringVector& aErrorPages, const string& aRoot)
{
	for (size_t index = 0; index < aErrorPages.size(); ++index)
    {
        StringVector values = str::split(aErrorPages[index], ' '); 
        if (values.size() < 2)
            continue;

        std::string path = values[values.size() - 1];
        if (path[path.size() - 1] == '/')
            continue;
    
        for (size_t i = 0; i < values.size() - 1; ++i)
        {
            int code = str::strToInt(values[i]);
            if (code <= 0 || code > 999)
                continue ;
			
            setErrorPage(code, aRoot + path);
        }
    }
}

//===[ Destructor: ErrorPages ]=================================================
http::ErrorPages::~ErrorPages(void) {}

/*******************************************************************************
	* Public Methods :
*******************************************************************************/


void	http::ErrorPages::setErrorPage(u_int aCode, const string& aPath)
{
	mErrorPages[aCode] = aPath;
}

//===[ Method: build error page ]===============================================
http::IResponse::SharedPtr http::ErrorPages::build(u_int aCode) const
{
    http::FileResponse* fileRes = new http::FileResponse();
    
    try
    {
        std::string path = mErrorPages.at(aCode);
        fileRes->setHeader("Connection", "close");
        fileRes->setHeader("Content-Type", "text/html");
        fileRes->setStatusCode(aCode);
        fileRes->setToErrorMode();
        fileRes->setPath(path);
        return (fileRes);
    }
    catch (...)
    {
        delete (fileRes);
        RawResponse* response = new http::RawResponse();
        response->setTemplateOn();
        response->setToErrorMode();
        response->setStatusCode(aCode);
        response->setBody(CUSTOM_ERROR_PAGE);
        response->setHeader("Connection", "close");
        response->setHeader("Content-Type", "text/html");
        return (response);
    }
}
