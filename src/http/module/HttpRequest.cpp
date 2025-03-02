/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpRequest implementation file ]                                    #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-09                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpRequest.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: Request ]===================================================
http::Request::Request(string& aBuffer)
{
    std::stringstream bufferStream(aBuffer);
    string line;

    std::getline(bufferStream, line);
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.resize(line.size() - 1);
    _setRequestLine(line);

    while (std::getline(bufferStream, line) && line != "\r" && !line.empty())
        setHeader(line);

    if (getHeader("host").empty()) {
        throw (http::Exception("missing host header", http::BAD_REQUEST));
    }

    aBuffer = bufferStream.str().substr(bufferStream.tellg());
}

//===[ Destructor: Request ]====================================================
http::Request::~Request(void)
{
    file::removeFile(mBodyPath);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: selectMatechedRoute ]============================================
void	http::Request::selectMatechedRoute(const ServerVector& aServerList)
{
    mMatchedServer = _selectMatchedServer(aServerList);
    mMatchedLocation = _selectMatchedLocation();
}

//===[ Method: setHeader ]======================================================
void	http::Request::setHeader(string& aLine)
{
    if (!aLine.empty() && aLine[aLine.size() - 1] == '\r')
         aLine.resize(aLine.size() - 1);
    if ( aLine.find(':') == string::npos) {
       throw (http::Exception("Invalid header", http::BAD_REQUEST)); 
    }
    StringPair header = str::lineToPair(aLine, ':');
    
    for (size_t index = 0; index < header.first.size(); ++index)
    {
        if (http::isValidHeader(header.first[index]) == false) {
            throw (http::Exception("Invalid header", http::BAD_REQUEST));
        }
    }

    header.first = str::toLower(header.first);
    mHeaders[header.first] = header.second;
}

//===[ Method: setBodyPath ]====================================================
void	http::Request::setBodyPath(const string& aPath)
{
    mBodyPath = aPath;
}

//===[ Method: uriAppend ]======================================================
void	http::Request::uriAppend(const string& path)
{
    mUri += path;
}

//===[ Method: hasBody ]========================================================
bool http::Request::hasBody(void) const
{
    return (!mBodyPath.empty());
}

/*** * Getters :
*******************************************************************************/

//===[ Method: geMethod ]======================================================
string http::Request::getMethod(void) const
{
    return (mMethod);
}

//===[ Method: getUri ]========================================================
string	http::Request::getUriPath(void) const
{
    return (mUri);
}

//====[ Method: getVersion ]===================================================
string http::Request::getVersion(void) const
{
    return (mVersion);
}

//====[ Method: getUriQuery ]==================================================
string  http::Request::getUriQuery(void) const
{
    return 	(mQuery);
}

//====[ Method: getHeaders ]===================================================
const string& http::Request::getHeader(string const& key) const
{
    StringMap::const_iterator it = mHeaders.find(str::toLower(key));
    if (it == mHeaders.end())
    {
        return (EmptyString);
    }
    return (it->second);
}

//====[ Method: getMatchedServer ]=============================================
const http::IServer& http::Request::getMatchedServer(void) const
{
    return (*mMatchedServer);
}

//===[ Method : getMatchedLocation ]===========================================
const http::Location& http::Request::getMatchedLocation(void) const
{
    return (*mMatchedLocation);
}

//====[ Method: getBodyPath ]==================================================
const string& http::Request::getBodyPath(void) const
{
    return (mBodyPath);
}

//====[ Method: display ]======================================================
void http::Request::display(void) const
{
    StringMap::const_iterator it;

    std::cout << getMethod() << " " << getUriPath() << " "
    << getVersion() << std::endl;

    std::cout << "**********************> Query :" << std::endl;
    std::cout << getUriQuery() << std::endl; 

    std::cout << "**********************> Headers :" << std::endl;
    for (it = mHeaders.begin(); it != mHeaders.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: _setRequestLine ]================================================
void	http::Request::_setRequestLine(const string& aLine)
{
    StringVector parts = str::split(aLine, ' ');
    if (parts.size() != 3) {
        string msg = "Invalid request line";
        throw (http::Exception(msg, http::BAD_REQUEST));
    }
    mMethod = parts[0];
    if (http::isValidMethod(mMethod) == false) {
        string msg = "Invalid request method";
        throw (http::Exception(msg, http::NOT_IMPLEMENTED)); 
    }
    mVersion = parts[2];
    if (http::isValidProtocol(mVersion) == false) {
        string msg = "Invalid http version";
        throw (http::Exception(msg, http::VERSION_NOT_SUPPORTED)); 
    }
    mUri = _parseUri(parts[1]);
}

//====[ Method: parseUri ]======================================================
string& http::Request::_parseUri(string &aUri)
{
    for (size_t i = 0; i < aUri.size(); ++i)
    {
        if (!http::isSpecialChars(aUri[i])) {
            throw (http::Exception("Invalid uri", http::BAD_REQUEST)); 
        }
    }
    
    http::decoder(aUri);
    _parseQuery(aUri);
    if (aUri.size() > 2048) {
        throw(http::Exception("Uri too long", http::URI_TOO_LONG));
    }
    return (aUri);
}

//====[ Method: parseQuery ]====================================================
void http::Request::_parseQuery(string& aUri)
{
    size_t fragmentPosition = aUri.find('#');
    if (fragmentPosition != string::npos)
    {
        size_t endPosition = aUri.find('?');
        aUri.erase(fragmentPosition, endPosition - fragmentPosition);
    }

    if (aUri.find('?') == string::npos)
        return;

    mQuery = aUri.substr(aUri.find('?') + 1);
    aUri.erase(aUri.find('?'));
}

//====[ Method: selectMatechedServer]==========================================
http::IServer::SharedPtr  
http::Request::_selectMatchedServer(const ServerVector& aServerList)
{
    for (size_t i= 0; i < aServerList.size(); ++i)
    {
        if (aServerList[i]->isMatch(getHeader("host")))
            mMatchedServer = aServerList[i];
    }
    if (!mMatchedServer)
        mMatchedServer = aServerList[0];
    return (mMatchedServer);
}

//===[ Method: _isInnerPath ]===================================================
bool http::Request::_isInnerPath(const string& uri, const string& requestUri)
{
    string remaining = requestUri.substr(uri.size());
    remaining.erase(0, remaining.find_first_not_of('/'));

    size_t firstSlashPos = remaining.find('/');
    if (firstSlashPos == string::npos)
        return (false);
    
    if (firstSlashPos == remaining.size() - 1)
        return (false);
    
    return (true);
}

//====[ Method: setMatchedLocation ]============================================
http::Location::SharedPtr    http::Request::_selectMatchedLocation(void)
{
    IServer::LocationMap locations  = mMatchedServer->getLocations();
    IServer::LocationMap::const_iterator it = locations.begin();
    string bestLocation;

    string dir = mUri.substr(0, mUri.find_last_of('/') + 1);

    for (; it != locations.end(); ++it)
    {
        if (dir.find(it->first) == 0 && it->first.size() > bestLocation.size())
        {
            string select = it->first;
            if (select[select.size() - 1] == '/' && !_isInnerPath(select, dir))
                bestLocation = it->first;
            else if (select[select.size() - 1] != '/' || select.size() == 1)
                bestLocation = it->first;
        }
    }

    if (bestLocation.empty() == true)
        throw(http::Exception("Not found a Matched Location", http::NOT_FOUND));
    return (locations[bestLocation]);
}
