/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpServer implementation file ]                                     #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-06-02                                                   #
#                                                                              #
** ************************************************************************* **/

# include "HttpServer.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: Server ]===================================================
http::Server::Server(Directive::SharedPtr aServerDir)
{
    setListens(aServerDir->getTerminal("listen"));
    setServerNames(aServerDir->getTerminal("server_names"));
    setMaxBodySize(aServerDir->getTerminal("max_body_size"));
    setDefaultMimeType(aServerDir->getTerminal("default_type"));
    setMimeTypes(aServerDir->getTerminal("meme_type"));
    setServerRoot(aServerDir->getTerminal("root"));

    mTimeout[0] = toTime(aServerDir->getTerminal("keepalive_timeout"), KL_TIMEOUT);
    mTimeout[1] = toTime(aServerDir->getTerminal("read_timeout"), READ_TIMEOUT);
    mTimeout[2] = toTime(aServerDir->getTerminal("send_timeout"), SEND_TIMEOUT);
    mTimeout[3] = toTime(aServerDir->getTerminal("cgi_timeout"), CGI_TIMEOUT);

    mErrorPages = ErrorPages(aServerDir->getTerminal("error_page"), mRoot);

    setLocations(aServerDir, aServerDir->getNonTerminal("location"));
}

//===[ Destructor: Server ]====================================================
http::Server::~Server(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: check if server name match ]=====================================
bool  http::Server::isMatch(const string& aHostName) const
{
    for (size_t index = 0; index < mServerName.size(); ++index)
    {
        if (aHostName == mServerName[index])
            return (true);
    }
    return (false);
}

//===[ Method: check if keep alive ]============================================
bool    http::Server::isKeepAlive(void) const
{
    return (mKeepAlive);
}

//===[ Method: to time ]=======================================================
time_t  http::Server::toTime(const StringVector& aTime, time_t aDefault) const
{
    time_t time = aDefault;

    if (aTime.size() == 1)
    {
        if ((time = str::strToInt(aTime[0])) > 5)
            return (time);
        Logger::log("warning", "HTTP: ignored: '" +  aTime[0] + "'", 2);
    }
    return (aDefault);
}

/*** * Setters :
*******************************************************************************/

//===[ Method: set listens ]====================================================
void    http::Server::setListens(const StringVector& aListens)
{
    for (size_t index = 0; index < aListens.size(); ++index)
    {
        StringPair addressPort = str::lineToPair(aListens[index], ':');
        int port =  str::strToInt (addressPort.second);
        if (port <= 0 || port > 65535)
        {
            Logger::log("warning", "HTTP: Listen directive ignored: '"
                +  aListens[index] + "'", 2);
            continue ;
        }
        std::string address = str::strtrim(addressPort.first);
        std::string portString = str::strtrim(addressPort.second);
        mListens.push_back(address + ":" + portString);
    }
    if (mListens.size() == 0)
    {
        mListens.push_back(DEFAULT_LISTEN);
    }
}

//===[ Method: set server names ]===============================================
void    http::Server::setServerNames(const StringVector& aServerName)
{
    if (aServerName.size() == 1)
        mServerName = str::split(aServerName[0], ' ');
}

//===[ Method: set client max body size ]=======================================
void http::Server::setMaxBodySize(const StringVector& aMaxBodySize)
{
    mMaxBodySize = 0;
    if (aMaxBodySize.size() == 1)
    {
        if ((mMaxBodySize = str::strToInt(aMaxBodySize[0])) > 0) {
            mMaxBodySize *= _1_MB_;
            return ;
        }
        Logger::log("warning", "HTTP: Client max body size ignored: '"
            +  aMaxBodySize[0] + "'", 2);
    }
}

//===[ Method: set Default MimeType ]===========================================
void    http::Server::setDefaultMimeType(const StringVector& aDefaultType)
{
    mMimeTypes["default"] = DEFAULT_MIME_TYPE;
    if (aDefaultType.size() == 1)
    {
        mMimeTypes["default"] = aDefaultType[0];
    }
}

//===[ Method: set Mime Types ]=================================================
void    http::Server::setMimeTypes(const StringVector& aMimeTypes)
{
    for (size_t index = 0; index < aMimeTypes.size(); ++index)
    {
        StringPair extansionType = str::lineToPair(aMimeTypes[index], ' ');
        mMimeTypes[extansionType.first] = str::strtrim(extansionType.second);
    }
}

//===[ Method: set Server Root ]================================================
void    http::Server::setServerRoot(const StringVector& aRoot)
{
    if (aRoot.size() == 1)
    {
        mRoot = aRoot[0];
        if (!mRoot.empty() && mRoot[mRoot.size() - 1] != '/')
            mRoot += "/";
        return;
    }
    mRoot = DEFAULT_SERVER_ROOT;
}

//===[ Method: set Location ]===================================================
void http::Server::setLocations(Directive::SharedPtr aServerDir,
                                Directive::DirPtrVector aLocation)
{
    for (size_t index = 0; index < aLocation.size(); ++index)
    {
        aLocation[index]->copyMatchingAttributes(aServerDir);
        try
        {
            Location* location = new Location(aLocation[index]);
            mLocations.insert(std::make_pair(location->getUri(), location));
        }
        catch(const std::exception& e)
        {
            Logger::log("warning", 
            "HTTP: unable to create location without specifying a URI.", 2);
        }
    }
}

/*** * getters :
*******************************************************************************/

//===[ Method: get listens ]====================================================
const StringVector&  http::Server::getListens(void) const
{
    return (mListens);
}

//===[ Method: get server names ]===============================================
const StringVector&  http::Server::getServerNames(void) const
{
    return (mServerName);
}

//===[ Method: get client max body size ]=======================================
unsigned long  http::Server::getMaxBodySize(void) const
{
    return (mMaxBodySize);
}

//===[ Method: get keep alive timeout ]=========================================
time_t         http::Server::getKeepAliveTimeout(void) const
{
    return (mTimeout[0]);
}

//===[ Method: get read timeout ]===============================================
time_t         http::Server::getReadTimeout(void) const
{
    return (mTimeout[1]);
}

//===[ Method: get send timeout ]===============================================
time_t         http::Server::getSendTimeout(void) const
{
    return (mTimeout[2]);
}

//===[ Method: get cgi timeout ]===============================================
time_t         http::Server::getCgiTimeout(void) const
{
    return (mTimeout[3]);
}

//===[ Method: get Mime Types ]=================================================
const std::string &http::Server::getMimeType(const string& aExtansion) const
{
    StringMap::const_iterator it = mMimeTypes.find(aExtansion);

    if (it != mMimeTypes.end())
        return (it->second);

    return (mMimeTypes.at("default"));
}

//===[ Method: get Error Pages ]================================================
http::IResponse::SharedPtr http::Server::getErrorPage(u_int aCode) const
{
    return (mErrorPages.build(aCode));
}

//===[ Method : Get Locations ]==================================================
const   http::Server::LocationMap& http::Server::getLocations(void) const
{
    return (mLocations);
}
