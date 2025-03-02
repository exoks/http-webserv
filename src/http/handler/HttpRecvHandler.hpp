/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpRecvHandler header file ]                                       #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-09                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef __HTTPRECVHANDLER_HPP__
# define __HTTPRECVHANDLER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "IEventHandler.hpp"
# include "IClient.hpp"
# include "IServer.hpp"
# include "IRequest.hpp"
# include "IReader.hpp"

# include "HttpException.hpp"

# include "HttpFactory.hpp"

/*******************************************************************************
    * class : RecvHandler :
*******************************************************************************/
namespace http
{
    class RecvHandler : public IEventHandler
    {
    public:
        typedef std::vector<IServer::SharedPtr>     ServerVector;
        typedef IEventHandlerQueue (RecvHandler::*Operation)(void);

        RecvHandler(IClient::SharedPtr aClient);
        virtual ~RecvHandler(void);
        
        const Handle&           getHandle(void) const;
        IMultiplexer::Mode      getMode(void) const;
        IEventHandlerQueue      handleEvent(void);
        bool                    isTerminated(void) const;

    private:
        bool                    mTerminated;
        IClient::SharedPtr      mClient;
        IRequest::SharedPtr     mRequest;
        Operation               mCurrentOperation;                
        string                  mReceivedData;
        IReader::SharedPtr      mBodyReader;

        const IServer&          _getMatchedServer(void) const;

        IEventHandlerQueue      _recvHeaders(void);
        IEventHandlerQueue      _recvBody(void);
        IEventHandlerQueue      _recvComplete(void);
    };
};

#endif /* __HTTPRECVHANDLER_HPP__ */