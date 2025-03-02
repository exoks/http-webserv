/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ CgiResponse header file ]                                            #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef    __HTTPCGIRESPONSE_HPP__
# define    __HTTPCGIRESPONSE_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "HttpFileResponse.hpp"
# include "HttpException.hpp"

# include <iostream>
# include <fstream>

/*******************************************************************************
    * HttpCgiResponse Class :
*******************************************************************************/
namespace http
{
    class CgiResponse : public FileResponse
    {
    public:
        CgiResponse(void);
        virtual ~CgiResponse(void);

        virtual void setPath(const string& aCgiOut);
    
    private:
        void    _parseCgiHeaders(void);
        void    _setHeaders(string& line);
    };
}

#endif	/* __HTTPCGIRESPONSE_HPP__ */