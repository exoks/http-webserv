/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpGetHandler implementation file ]                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-17                                                   #
#                                                                              #
** ************************************************************************* **/

# include "HttpGetHandler.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: GetdHandler ]===============================================
http::GetHandler::GetHandler(IClient::SharedPtr aClient,
                             IRequest::SharedPtr aRequest)
    : mClient(aClient),
      mRequest(aRequest)
{
    mTerminated = (mClient && mRequest) ? false : true;
}

//===[ Destructor: GetdHandler ]================================================
http::GetHandler::~GetHandler(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//=== [ Method : handleEvent ] =================================================
IEventHandler::IEventHandlerQueue http::GetHandler::handleEvent(void)
{
    IEventHandlerQueue eventHandlers;

    try
    {
        if (mRequest->getMethod() != "GET") {
            throw (http::Exception("Unknown Method", http::FORBIDDEN));
        }
        if (mRequest->getMatchedLocation().isAllowedMethod("GET") == false)
        {
            string msg = "Method Not Allowed";
            throw (http::Exception(msg, http::METHOD_NOT_ALLOWED));
        }

        mRessourcePath = _getAbsolutePath();
        if (mRessourcePath.empty() || access(mRessourcePath.c_str(), F_OK))
        {
            (errno == ENOENT) ?
            throw (http::Exception("File not found", http::NOT_FOUND)) :
            throw (http::Exception("Access denied", http::FORBIDDEN))  ;
        }
        eventHandlers.push(
            http::Factory::createSendHandler(mClient, _generateResponse())
        );
    }
    catch(http::Exception& aException)
    {
        aException.setClientInfo(mClient->getInfo());
        string msg = aException.what();
        Logger::log("error  ", "HTTP: "+ msg , 2);

        IResponse::SharedPtr errRes = mRequest->getMatchedServer()
                                      .getErrorPage(aException.getStatusCode());
        eventHandlers.push(http::Factory::createSendHandler(mClient, errRes));
    }
    catch (IEventHandler* aSwitcher)
    {
        eventHandlers.push(aSwitcher);
    }

    return (eventHandlers);
}

//===[ getMode: return Handler Mode ]===========================================
IMultiplexer::Mode http::GetHandler::getMode(void) const
{
    return (IMultiplexer::Write);
}
//===[ getHandle: return Handler Handle ]=======================================
const Handle& http::GetHandler::getHandle(void) const
{
    return (mClient->getSocket());
}

// ==[ isTerminated: return Handler Status ]====================================
bool http::GetHandler::isTerminated(void) const
{
    return (mTerminated);
}

/** ****************************************************************************
    * Private Methods :
*******************************************************************************/

std::string http::GetHandler::_getAbsolutePath(void)
{
    const string&   locationUri   = mRequest->getMatchedLocation().getUri();
    const string&   locationRoot  = mRequest->getMatchedLocation().getRoot();
    std::string     requestUri    = mRequest->getUriPath();

    return (locationRoot + http::getRelativePath(requestUri, locationUri));
}

//===[ Method: _resourceHandler ]===============================================
http::AResponse::SharedPtr    http::GetHandler::_generateResponse(void)
{
    if (file::isDirectory(mRessourcePath)) {
        return (_handleDirectory());
    }
    return (_handleFile(mRessourcePath));
}

// ===[ Method : Handle File ]==================================================
http::AResponse::SharedPtr  http::GetHandler::_handleFile(const string& aPath)
{
    const Location& location     = mRequest->getMatchedLocation();
    std::string   extension      = file::getExtension(aPath);
    const string  contentType    = mRequest->getMatchedServer()
                                        .getMimeType(extension);

    if (http::isCgiPath("." + extension, location.getCgiExt())) {
        throw (http::Factory::createCgiHandler(mClient, mRequest));
    }

    FileResponse* response = new http::FileResponse();
    try {
        response->setSendTimeout(mRequest->getMatchedServer().getSendTimeout());
        response->setVersion(mRequest->getVersion());
        response->setHeader("Content-Type", contentType);
        response->setHeader("Connection", mRequest->getHeader("Connection"));
        response->setStatusCode(http::OK);
        response->setPath(aPath);
    } catch (std::exception& e) {
        delete (response); throw e;
    }
    return (response);
}

// ===[ Method : Handle Directory ]=============================================
http::AResponse::SharedPtr  http::GetHandler::_handleDirectory(void)
{
    const http::IServer&    matchedServer     = mRequest->getMatchedServer();
    const http::Location&   matchedlocation   = mRequest->getMatchedLocation();

    if (mRessourcePath[mRessourcePath.length() - 1] != '/')
    {
        RawResponse*  response = new http::RawResponse();
        response->setStatusCode(http::MOVED_PERMANENTLY);
        response->setSendTimeout(matchedServer.getSendTimeout());
        response->setVersion(mRequest->getVersion());
        response->setHeader("Location", mRequest->getUriPath() + "/");
        response->setHeader("Connection", mRequest->getHeader("Connection"));
        return (response);
    }

    StringVector indexFiles = matchedlocation.getIndexFiles();
    if (!indexFiles.empty())
    {
        for (size_t index = 0; index < indexFiles.size(); index++)
        {
            if (!access((mRessourcePath + indexFiles[index]).c_str(), F_OK))
            {
                mRequest->uriAppend(indexFiles[index]);
                return (_handleFile(mRessourcePath + indexFiles[index]));
            }
        }
    }

    if (matchedlocation.isAutoIndex())
    {
        DIR* dir = opendir(mRessourcePath.c_str());
        if (dir != NULL)
        {
            RawResponse*  response = new http::RawResponse();
            response->setStatusCode(http::OK);
            response->setSendTimeout(matchedServer.getSendTimeout());
            response->setVersion(mRequest->getVersion());
            response->setBody(_autoIndex(dir));
            response->setHeader("Connection", mRequest->getHeader("Connection"));
            closedir(dir);
            return (response);
        }
    }

    throw(http::Exception("No Index Or Autoindex", http::FORBIDDEN));
}

///===[ Method : _autoIndex ]====================================================
std::string http::GetHandler::_autoIndex(DIR *dir)
{
    std::string autoindex;
    std::string content;
    struct dirent *ent;

    autoindex += CUSTOM_AUTOINDEX_PAGE;
    str::replace(autoindex, "$(RESOURCE_PATH)", mRessourcePath);
    while ((ent = readdir(dir)) != NULL)
    {
        content += "<tr>";
        if (ent->d_type == DT_REG)
        {
            content += "<td><span class=\"icon file-icon\"></span><a href=\"" + std::string(ent->d_name) + "\">" + ent->d_name + "</a></td>";
        }
        else if (ent->d_type == DT_DIR)
        {
            content += "<td><span class=\"icon dir-icon\"></span><a href=\"" + std::string(ent->d_name) + "/\">" + ent->d_name + "/</a></td>";
        }
        content += "</tr>";
    }
    str::replace(autoindex, "$(CONTENT)", content);

    return (autoindex);
}
