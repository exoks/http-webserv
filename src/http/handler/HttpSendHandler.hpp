/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpSendHandler header file ]                                        #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __HTTPSENDHANDLER_HPP__
# define __HTTPSENDHANDLER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IEventHandler.hpp"
# include "IClient.hpp"
# include "IResponse.hpp"

# include "HttpFactory.hpp"

/*******************************************************************************
    * class : SendHandler :
*******************************************************************************/
namespace http
{
    class SendHandler : public IEventHandler
    {
    public:
        SendHandler(IClient::SharedPtr aClient, IResponse::SharedPtr aResponse);
        virtual ~SendHandler(void);

        const Handle&           getHandle(void) const;
        IMultiplexer::Mode      getMode(void) const;
        IEventHandlerQueue      handleEvent(void);
        bool                    isTerminated(void) const;

    private:
        bool                    mTerminated;
        IClient::SharedPtr      mClient;
        IResponse::SharedPtr    mResponse;
    };
}

#endif	/* __HTTPSENDHANDLER_HPP__ */