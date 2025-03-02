/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IMultiplexer Interface ]                                            #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IMULTIPLEXER_HPP__
#define    __IMULTIPLEXER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

/*******************************************************************************
    * Imultiplexer Interface :
*******************************************************************************/
class IMultiplexer
{
public:
    typedef enum {Read, Write}					Mode;
    typedef mem::shared_ptr<IMultiplexer>		SharedPtr;

    virtual ~IMultiplexer(void) {};

    virtual HandleQueue  waitEvent(long long aTimeout_ms) = 0;
    virtual void         registerHandle(const Handle& aHandle, Mode aMode) = 0;
    virtual void         removeHandle(const Handle& aHandle, Mode aMode) = 0;
};

#endif /* __IMULTIPLEXER_HPP__ */
