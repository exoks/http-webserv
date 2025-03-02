/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Reactor header file ]                                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-16                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __REACTOR_HPP__
# define   __REACTOR_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IReactor.hpp"
# include "IMultiplexer.hpp"

# include <stdexcept>
# include <unistd.h>

/*******************************************************************************
    * Class Reactor :
*******************************************************************************/
class Reactor : public IReactor
{
public:
    typedef IEventHandler                       IEH;
    typedef IMultiplexer                        IMux;
    typedef std::map<Handle, IEH::SharedPtr>	IEventHandlerMap;

    Reactor(IMultiplexer* aMultiplexer);
    ~Reactor(void);

    void              registerEventHandler(IEH::SharedPtr aHandler);
    void              registerEventHandler(IEventHandlerQueue& aHandlers);
    IEH::SharedPtr    unregisterEventHandler(IEH::SharedPtr aHandler);
    void              handleEvents(long long aTimeout_ms);
    void              cleanupTerminatedHandlers(void);

private:
    IMux::SharedPtr   mMultiplexer;
    IEventHandlerMap  mEventHandlers;
};

#endif /* __REACTOR_HPP__ */
