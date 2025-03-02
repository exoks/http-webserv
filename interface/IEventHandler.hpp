/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IEventHandler Interface ]                                           #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-16                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IEVENTHANDLER_HPP__
# define   __IEVENTHANDLER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IMultiplexer.hpp"

/*******************************************************************************
    * IEventHandler Interface :
*******************************************************************************/
class IEventHandler
{
public:
    typedef mem::shared_ptr<IEventHandler>   SharedPtr;
    typedef std::queue<SharedPtr>            IEventHandlerQueue;

    virtual ~IEventHandler(void) {};
    
    virtual const Handle&         getHandle(void) const = 0;
    virtual IMultiplexer::Mode    getMode(void) const = 0;
    virtual IEventHandlerQueue    handleEvent(void) = 0;
    virtual bool                  isTerminated(void) const = 0;
};

#endif /* __IEVENTHANDLER_HPP__ */
