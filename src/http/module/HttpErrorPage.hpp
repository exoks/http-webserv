/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpErrorPage header file ]                                          #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef	  __HTTPERRORPAGE_HPP__
# define   __HTTPERRORPAGE_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IResponse.hpp"
# include "HttpAResponse.hpp"
# include "HttpRawResponse.hpp"
# include "HttpFileResponse.hpp"

/*******************************************************************************
    * CUSTOM Error PAGE :
*******************************************************************************/

# define CUSTOM_ERROR_PAGE "\
    <!DOCTYPE html>\n\
            <head>\n\
                <title>[ $(MESSAGE) ]</title>\n\
                <style>\n\
                    body {\n\
                        font-family: Arial, sans-serif;\n\
                        background: #282828;\n\
                        padding: 10px;\n\
                        color: #c6c6c6;\n\
                        margin: 0 auto;\n\
                        height: 100%;\n\
                        min-height: 100vh;\n\
                        box-sizing: border-box;\n\
                    }\n\
                </style>\n\
            </head>\n\
            <body>\n\
                <center><h1>$(MESSAGE)</h1></center>\n\
                <hr />\n\
                <center><p> $(SERVER) </p></center>\n\
            </body>\n\
    </html>\n"

/*******************************************************************************
    * HttpErrorPage Class :
*******************************************************************************/
namespace http
{
    class ErrorPages
    {
    public:
        ErrorPages(void);
        ErrorPages(const StringVector& aErrorPages, const string& aRoot);
        virtual ~ErrorPages(void);

        void			      setErrorPage(u_int aCode, const string& aPath);
        IResponse::SharedPtr  build(u_int aCode) const;

    private:
        UintStringMap		mErrorPages;
    };
}

#endif /* __HTTPERRORPAGE_HPP__ */