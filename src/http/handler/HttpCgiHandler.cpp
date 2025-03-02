/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpCgiHandler implementation file ]                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-29                                                   #
#                                                                              #
** ************************************************************************* **/

# include "HttpCgiHandler.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: CgidHandler ]===============================================
http::CgiHandler::CgiHandler(IClient::SharedPtr aClient,
                             IRequest::SharedPtr aRequest)
    : mClient(aClient),
      mRequest(aRequest),
      mCgiPid(-1)
{
    mOutPath = file::generateTempFileName(http::tmpDir(""));
    mOutFd   = file::createFile(mOutPath);
    if (mOutFd == -1) {
        std::string msg = "Can't create temporary file";
        throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR));
    }
    mCurrentOperation = &CgiHandler::_setupCgi;
}

//===[ Destructor: CgidHandler ]================================================
http::CgiHandler::~CgiHandler(void)
{
    file::closeFile(mOutFd);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ handleEvent: return IEventHandlerQueue ]=================================
IEventHandler::IEventHandlerQueue http::CgiHandler::handleEvent(void)
{
    IEventHandlerQueue eventHandlers;

    try
    {
        std::string method = mRequest->getMethod();
        if (mRequest->getMatchedLocation().isAllowedMethod(method) == false)
        {
            string msg = "Method Not Allowed";
            throw (http::Exception(msg, http::METHOD_NOT_ALLOWED));
        }

        eventHandlers = (this->*mCurrentOperation)();
    }
    catch(http::Exception& aException)
    {
        cleanHandler();
        aException.setClientInfo(mClient->getInfo());
        string msg = aException.what();
        Logger::log("error  ", "CGI: "+ msg , 2);

        IResponse::SharedPtr errRes = mRequest->getMatchedServer()
                                      .getErrorPage(aException.getStatusCode());
        eventHandlers.push(http::Factory::createSendHandler(mClient, errRes));
    }

    return (eventHandlers);
}

//===[ CgiMode: return Handler Mode ]============================================
IMultiplexer::Mode http::CgiHandler::getMode(void) const
{
    return (IMultiplexer::Write);
}
//===[ CgiHandle: return Handler Handle ]========================================
const Handle& http::CgiHandler::getHandle(void) const
{
    return (mClient->getSocket());
}

// ==[ isTerminated: return Handler Status ]====================================
bool http::CgiHandler::isTerminated(void) const
{
    return (false);
}

//===[ Method: cleanHandler ]===================================================
void http::CgiHandler::cleanHandler(void)
{
    file::closeFile(mOutFd);
    file::removeFile(mOutPath);

    if (::waitpid(mCgiPid, NULL, WNOHANG) == 0)
    {
        ::kill(mCgiPid, SIGKILL);
        waitpid(mCgiPid, NULL, 0);
    }
}

/** ****************************************************************************
    * Process Methods :
*******************************************************************************/

//===[ Method: buildCgi ]=======================================================
IEventHandler::IEventHandlerQueue http::CgiHandler::_setupCgi(void)
{
    mCgiPid = fork();
    if (mCgiPid == -1) {
        throw (http::Exception("fork failed", http::INTERNAL_SERVER_ERROR));
    }

    if (mCgiPid == 0)
    {
        StringVector env  = _buildCgiEnv();
        string       path = _getAbsoluteCgiPath();

        StringVector args ;
        args.push_back(path);

        _checkScriptAccess(path);
        _setupCgiIo();

        int er = chdir(path.substr(0, path.find_last_of("/") + 1).c_str());
        if (er == -1) {
            Logger::log("cgi    ", strerror(errno), 2);
            std::exit(EXIT_FAILURE);
        }
        ::execve(path.c_str(), str::to_Array(args), str::to_Array(env));
        Logger::log("cgi    ", strerror(errno), 2);
        std::exit(EXIT_FAILURE);
    }

    mCgiLastActiveTime = time(NULL);
    mCurrentOperation  = &CgiHandler::_waitCgiToFinish;
    return (_waitCgiToFinish());
}

//===[ Method: WaitCgiToFinish ]===============================================
IEventHandler::IEventHandlerQueue http::CgiHandler::_waitCgiToFinish(void)
{
    time_t cgiTimeout  = mRequest->getMatchedServer().getCgiTimeout();
    time_t currentTime = time(NULL);
    int    waitStatus;

    if (waitpid(mCgiPid, &waitStatus, WNOHANG) != 0)
    {

        if (WEXITSTATUS(waitStatus) != 0)
        {
          throw http::Exception("CgiScript failed",http::INTERNAL_SERVER_ERROR);
        }
        return (_createResponse());
    }

    if (currentTime - mCgiLastActiveTime >= cgiTimeout)
    {
        throw (http::Exception("cgi timeout", http::INTERNAL_SERVER_ERROR));
    }

    return (IEventHandlerQueue());
}

//===[ Method: createResponse ]===============================================
IEventHandler::IEventHandlerQueue http::CgiHandler::_createResponse(void)
{
    IEventHandlerQueue eventHandlers;

    http::CgiResponse* response = new http::CgiResponse();
    response->setSendTimeout(mRequest->getMatchedServer().getSendTimeout());
    response->setVersion(mRequest->getVersion());
    response->setPath(mOutPath);
    eventHandlers.push(http::Factory::createSendHandler(mClient, response));

    return (eventHandlers);
}

/** ***************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: _buildCgiEnv ]===================================================
StringVector http::CgiHandler::_buildCgiEnv(void)
{
    StringVector env;
    StringPair ServerInfo = str::lineToPair(mClient->getInfo(), ':');

    env.push_back("GATEWAY_INTERFACE=" + CgiVersion);
    env.push_back("SERVER_SOFTWARE=" + ServerVersion);
    env.push_back("SERVER_PROTOCOL=" + mRequest->getVersion());
    env.push_back("SERVER_NAME=" + ServerInfo.first);
    env.push_back("CLIENT_PORT=" + ServerInfo.second);

    env.push_back("CONNECTION=" + mRequest->getHeader("Connection"));
    env.push_back("CONTENT_LENGTH=" + mRequest->getHeader("Content-Length"));
    env.push_back("CONTENT_TYPE=" + mRequest->getHeader("Content-Type"));
    env.push_back("REQUEST_METHOD=" + mRequest->getMethod());
    env.push_back("REQUEST_URI=" + mRequest->getUriPath());
    env.push_back("QUERY_STRING=" + mRequest->getUriQuery());
    env.push_back("PATH_INFO=" + mRequest->getUriPath());
    env.push_back("PATH_TRANSLATED=" + _getAbsoluteCgiPath());

    env.push_back("HTTP_ACCEPT=" + mRequest->getHeader("Accept"));
    env.push_back("HTTP_ACCEPT_CHARSET=" + mRequest->getHeader("Accept-Charset"));
    env.push_back("HTTP_ACCEPT_ENCODING=" + mRequest->getHeader("Accept-Encoding"));
    env.push_back("HTTP_ACCEPT_LANGUAGE=" + mRequest->getHeader("Accept-Language"));
    env.push_back("HTTP_HOST=" + mRequest->getHeader("Host"));
    env.push_back("HTTP_REFERER=" + mRequest->getHeader("Referer"));
    env.push_back("HTTP_USER_AGENT=" + mRequest->getHeader("User-Agent"));
    env.push_back("HTTP_COOKIE=" + mRequest->getHeader("Cookie"));
    env.push_back("UPLOAD_DIR=" + mRequest->getMatchedLocation().getUpload());

    return (env);
}

//===[ Method: _getAbsoluteCgiPath ]============================================
string http::CgiHandler::_getAbsoluteCgiPath(void)
{
    const string&  locationUri  = mRequest->getMatchedLocation().getUri();
    const string&  locationRoot = mRequest->getMatchedLocation().getRoot();
    std::string    requestUri   = mRequest->getUriPath();

    return (locationRoot + http::getRelativePath(requestUri, locationUri));
}

//===[ Method: _checkScriptAccess ]============================================
void http::CgiHandler::_checkScriptAccess(const string& path)
{
    if (access(path.c_str(), X_OK) == -1)
    {
        Logger::log("cgi    ", path + " : " + strerror(errno), 2);
        std::exit(EXIT_FAILURE);
    }
}

//===[ Method: _setupCgiIo ]====================================================
void http::CgiHandler::_setupCgiIo(void)
{
    int stdoutFd = dup2(mOutFd, STDOUT_FILENO);
    if (stdoutFd == -1)
    {
        Logger::log("cgi    ", "Failed to dup2 stdout", 2);
        std::exit(EXIT_FAILURE);
    }

    if (mRequest->hasBody())
    {
        const std::string& bodyPath = mRequest->getBodyPath();
        int inputFd = open(bodyPath.c_str(), O_RDONLY);
        if (inputFd == -1)
        {
            Logger::log("cgi    ", "Failed to open input file", 2);
            std::exit(EXIT_FAILURE);
        }

        int stdinFd = dup2(inputFd, STDIN_FILENO);
        if (stdinFd == -1)
        {
            Logger::log("cgi    ", "Failed to dup stdin file", 2);
            std::exit(EXIT_FAILURE);
        }
        close(inputFd);
    }
}
