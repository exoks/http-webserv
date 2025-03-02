/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IReader Interface ]                                                 #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-22                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IREADER_HPP__
# define   __IREADER_HPP__


/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

/*******************************************************************************
    * http::IReader Interface :
*******************************************************************************/
namespace http
{
    class IReader
    {
    public:
        typedef mem::shared_ptr<IReader>           SharedPtr;

        virtual ~IReader(void) {};
    
        virtual void            build(string& ReceivedData) = 0;
        virtual bool            isComplete(void) const = 0;
        virtual const string&   getFilePath(void) const = 0;
    };
}

#endif /* __IREADER_HPP__ */