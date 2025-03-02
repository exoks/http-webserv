/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ ServerCore header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-28                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __SERVERCORE_HPP__
# define __SERVERCORE_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IProtocolFactory.hpp"
# include "SelectMultiplexer.hpp"
# include "Directive.hpp"
# include "Reactor.hpp"

# include "HttpFactory.hpp"

# include <exception>

/*******************************************************************************
    * ServerCore Class :
*******************************************************************************/
class ServerCore
{
public:
    typedef IEventHandler                             IEH;
    typedef IProtocolFactory::SharedPtr               IProtocolFactoryPtr;
    typedef std::map<string, IProtocolFactoryPtr>     IProtocolFactoryMap;
    typedef std::vector<Directive::SharedPtr>         DirPtrVector;
    typedef mem::shared_ptr<ServerCore>               SharedPtr;

    ~ServerCore(void);

    static ServerCore::SharedPtr  getInstance(void);

    void    setup(Directive::SharedPtr aGlobalDir);
    void    run(void);
    void    stop(void);

private:
    IProtocolFactoryMap     mProtocolFactoryMap;
    IReactor::SharedPtr     mReactor;
    bool                    mIsRunning;
  
    ServerCore(void);

    void    _setupProtocol(const string& aProtocolName,
                           IProtocolFactoryPtr aProtocolFactory,
                           Directive::SharedPtr aGlobalDir);
};

#endif /* __SERVERCORE_HPP__ */
