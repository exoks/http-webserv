/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IResponse Interface ]                                               #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-16                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IResponse_HPP__
# define   __IResponse_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

/*******************************************************************************
    * IRequest Interface :
*******************************************************************************/
namespace http
{
    class IResponse
    {
    public:
        typedef mem::shared_ptr<IResponse>           SharedPtr;

        virtual ~IResponse(void) {}

        
        virtual string          toRaw(void) = 0;
        virtual void            removeBytesSent(size_t bytesSent) = 0;
        virtual time_t          getSendTimeout(void) const = 0;
        virtual const string&   getHeader(const string& aHeader) const = 0;
        virtual bool            eof(void) const = 0;
        virtual void            log(const string& aClientInfo) const = 0;
        virtual void            display(void) const = 0;
    };
}

#endif	/* __IResponse_HPP__ */