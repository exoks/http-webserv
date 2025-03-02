/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpCgiHandler header file ]                                         #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-29                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef	  __HTTPCGIHANDLER_HPP__
# define   __HTTPCGIHANDLER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IEventHandler.hpp"
# include "IResponse.hpp"
# include "IRequest.hpp"
# include "IClient.hpp"

# include "HttpCgiResponse.hpp"
# include "HttpFactory.hpp"

# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>

/*******************************************************************************
    * class : CgiHandler :
*******************************************************************************/
namespace http
{   
    class CgiHandler : public IEventHandler
    {
    public:
        typedef IEventHandlerQueue (CgiHandler::*Operation)(void);

        CgiHandler(IClient::SharedPtr aClient, IRequest::SharedPtr aRequest);
        virtual ~CgiHandler(void);

        const Handle&        getHandle(void) const;
        IMultiplexer::Mode   getMode(void) const;
        IEventHandlerQueue   handleEvent(void);
        bool                 isTerminated(void) const;
        void                 cleanHandler(void);

    private:
        IClient::SharedPtr  mClient;
        IRequest::SharedPtr mRequest;
        fd_t                mOutFd;
        std::string         mOutPath;
        int                 mCgiPid;
        Operation           mCurrentOperation;
        time_t              mCgiLastActiveTime;
    
        StringVector _buildCgiEnv(void);
        void         _checkScriptAccess(const std::string& aPath);
        void         _setupCgiIo(void);
        string       _getAbsoluteCgiPath(void);

        IEventHandlerQueue _setupCgi(void);
        IEventHandlerQueue _waitCgiToFinish(void);
        IEventHandlerQueue _createResponse(void);

    };
};

#endif /* __HTTPCGIHANDLER_HPP__ */
