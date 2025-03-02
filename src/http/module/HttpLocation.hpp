/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpLocation header file ]                                          #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-09                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __HTTPLOCATION_HPP__
# define __HTTPLOCATION_HPP__


/******************************************************************************
    * Defaults :
******************************************************************************/
# define DEFAULT_ROOT  "./" 
# define DEFAULT_AUTOINDEX "off"

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

#include "Directive.hpp"

/*******************************************************************************
    * HttpLocation Class :
*******************************************************************************/
namespace http
{
    class Location
    {
        public:
            typedef mem::shared_ptr<http::Location>         SharedPtr;
            typedef struct { int code; string uri;}   Redirect;

            Location(Directive::SharedPtr aLocationDir);
            ~Location(void);

            bool                    isAllowedMethod(const string& aMethod) const;
            bool                    isAutoIndex(void) const;
        
            const string&           getUri(void) const;
            const string&           getRoot(void) const;
            const string&           getUpload(void) const;
            const Redirect&         getRedirect(void) const;
            const StringVector&     getCgiExt(void) const;
            const StringVector&     getIndexFiles(void) const;

            void    setUri(const StringVector&  aUri);
            void    setRoot(const StringVector&  aRoot);
            void    setUpload(const StringVector& aCgiExt);
            void    setCgiExt(const StringVector& aCgiExt);
            void    setRedirect(const StringVector&  aReturn);
            void    setAutoIndex(const StringVector& aAutoIndex);
            void    setIndexFiles(const StringVector& aIndexFiles);
            void    setAllowedMethods(const StringVector& aAllowedMethods);

        private:
            bool            mAutoIndex;
            string          mUri;
            string          mRoot;
            string          mUpload;
            StringVector    mCgiExt;
            StringVector    mIndexFiles;
            StringVector    mAllowedMethods;
            Redirect        mRedirect;
    };
};

#endif /* __HTTPLOCATION_HPP__ */

