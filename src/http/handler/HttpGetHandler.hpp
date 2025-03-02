/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpGetHandler header file ]                                        #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-17                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __HTTPGETHANDLER_HPP__
# define __HTTPGETHANDLER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IEventHandler.hpp"
# include "IResponse.hpp"
# include "IRequest.hpp"
# include "IClient.hpp"

# include "HttpFactory.hpp"
# include "HttpRawResponse.hpp"
# include "HttpException.hpp"

# include <dirent.h>

/*******************************************************************************
    * class : GEtHandler :
*******************************************************************************/
namespace http
{   
    class GetHandler : public IEventHandler
    {
    public:
        GetHandler(IClient::SharedPtr aClient, IRequest::SharedPtr aRequest);
        virtual ~GetHandler(void);

        const Handle&           getHandle(void) const;
        IMultiplexer::Mode      getMode(void) const;
        IEventHandlerQueue      handleEvent(void);
        bool                    isTerminated(void) const;

    private:
        IClient::SharedPtr      mClient;
        IRequest::SharedPtr     mRequest;
        bool                    mTerminated;
        std::string             mRessourcePath;
    
        std::string                 _getAbsolutePath(void);
        http::AResponse::SharedPtr  _generateResponse(void);
        http::AResponse::SharedPtr  _handleDirectory(void);
        http::AResponse::SharedPtr  _handleFile(const string& aPath);
        std::string                 _autoIndex(DIR *dir);
    };
};


/*******************************************************************************
    * CUSTOM AutoIndex :
*******************************************************************************/

#define CUSTOM_AUTOINDEX_PAGE "\
<!DOCTYPE html>\n\
<html>\n\
    <head>\n\
        <title>Index of $(RESOURCE_PATH)</title>\n\
        <style>\n\
            body {\n\
                font-family: Arial, sans-serif;\n\
            }\n\
            h1 {\n\
                color: #333;\n\
            }\n\
            a {\n\
                text-decoration: none;\n\
                color: #007bff;\n\
            }\n\
            a:hover {\n\
                text-decoration: underline;\n\
            }\n\
            .icon {\n\
                display: inline-block;\n\
                width: 16px;\n\
                height: 16px;\n\
                margin-right: 5px;\n\
                vertical-align: middle;\n\
            }\n\
            .file-icon {\n\
               background: url('data:image/png;base64,<file_icon_base64>') no-repeat center center;\n\
            }\n\
            .dir-icon {\n\
                background: url('data:image/png;base64,<dir_icon_base64>') no-repeat center center;\n\
            }\n\
            table {\n\
                width: 100%;\n\
                border-collapse: collapse;\n\
            }\n\
            th, td {\n\
                padding: 8px;\n\
                text-align: left;\n\
                border-bottom: 1px solid #ddd;\n\
            }\n\
            th {\n\
                background-color: #f2f2f2;\n\
            }\n\
        </style>\n\
    </head>\n\
    <body>\n\
        <h1>Index of $(RESOURCE_PATH)</h1>\n\
        <hr>\n\
        <table>\n\
            <tr>\n\
                <th>Name</th>\n\
            </tr>\n\
            $(CONTENT)\n\
        </table>\n\
    </body>\n\
</html>"

#endif /* __HTTPGETHANDLER_HPP__ */