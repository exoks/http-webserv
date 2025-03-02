/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IReactor Interface ]                                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IREACTOR_HPP__
# define   __IREACTOR_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IEventHandler.hpp"

/*******************************************************************************
    * IReactor Interface :
*******************************************************************************/
class IReactor
{
public:
    typedef IEventHandler                   IEH;
    typedef mem::shared_ptr<IReactor>       SharedPtr;
    typedef std::queue<IEH::SharedPtr>      IEventHandlerQueue;

    virtual	~IReactor(void) {};
    
    virtual void            registerEventHandler(IEH::SharedPtr aHandler) = 0;
    virtual void            registerEventHandler(IEventHandlerQueue& aHandlers) = 0;
    virtual IEH::SharedPtr  unregisterEventHandler(IEH::SharedPtr aHandler) = 0;
    virtual void            handleEvents(long long aTimeout_ms) = 0;
    virtual void            cleanupTerminatedHandlers(void) = 0;
};

#endif	/* __IREACTOR_HPP__ */
