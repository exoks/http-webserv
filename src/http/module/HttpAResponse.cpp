/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpAResponse implementation file ]                                  #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpAResponse.hpp"

/*******************************************************************************
    * Static Variables :
*******************************************************************************/
const UintStringMap
http::AResponse::sStatusMessage = http::AResponse::_sinitStatusMessage();

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: AResponse ]================================================
http::AResponse::AResponse(void)
    : mMode(1),
      mVersion("HTTP/1.1"),
      mStatusCode(501),
      mStatusline("501 Not Implemented"),
      mSendTimeout(60)
{
    setHeader("Date", Logger::getcurrentTime("%a, %d %b %Y %H:%M:%S %z"));
    setHeader("Server", ServerVersion);
    setHeader("Content-Type", "text/html");
    setHeader("Content-Length", "0");
}

//===[ Destructor: AResponse ]=================================================
http::AResponse::~AResponse(void) {}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Methode : set Version ]=================================================
void    http::AResponse::setVersion(const std::string& aVersion)
{
    mVersion = aVersion;
}

//===[ Methode : set StatusCode ]==============================================
void    http::AResponse::setStatusCode(u_int aStatusCode)
{
    mStatusCode = aStatusCode;
    UintStringMap::const_iterator it = sStatusMessage.find(aStatusCode);
    if (it != sStatusMessage.end())
    {
        mStatusline = it->second;
    }
}

//===[ Methode : set Header ]==================================================
void    http::AResponse::setHeader(const std::string& aHeader,
                                            const string& aValue)
{
    mHeaders[str::toLower(aHeader)] = aValue;
}

//===[ Methode : set Send Timeout ]=============================================
void    http::AResponse::setSendTimeout(time_t aSendTimeout)
{
    mSendTimeout = aSendTimeout;
}

//===[ Methode : set To Error Mode ]===========================================
void    http::AResponse::setToErrorMode(void)
{
    mMode = 2;
}

//===[ Methode : get Header ]==================================================
const string& http::AResponse::getHeader(const string& aHeader) const
{
    StringMap::const_iterator it = mHeaders.find(str::toLower(aHeader));
    if (it != mHeaders.end())
    {
        return (it->second);
    }
    return (EmptyString);
}

//===[ Methode : get Send Timeout ]=============================================
time_t  http::AResponse::getSendTimeout(void) const
{
    return (mSendTimeout);
}

//===[ Methode : toRaw ]=======================================================
std::string http::AResponse::toRaw(void)
{
    std::string rawHeader = mVersion + " " + mStatusline + CRLF;
    StringMap::const_iterator it = mHeaders.begin();
    while (it != mHeaders.end())
    {
        rawHeader += it->first + ": " + it->second + CRLF;
        ++it;
    }
    rawHeader += CRLF;
    return (rawHeader); 
}

//===[ Methode : log ]=========================================================
void http::AResponse::log(const string& aClientInfo) const
{
    string title = (mMode == 1) ? "notice " : "error  ";
    Logger::log(title, "HTTP: Response: '" + mVersion + " " + mStatusline
                     + "' Client: '" + aClientInfo + "'");
}

//===[ Methode : display ]=====================================================
void http::AResponse::display(void) const
{
    std::cout << "statusline: " << mVersion << " " <<  mStatusline << std::endl;
    std::cout << "headers: ===== " << std::endl;
    for (StringMap::const_iterator it = mHeaders.begin();
         it != mHeaders.end(); it++) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "send timeout: " << mSendTimeout << std::endl;
    std::cout << "================" << std::endl;
}

/*******************************************************************************
    * Static Methods :
*******************************************************************************/

//===[ Methode : _sinitStatusMessage ]==========================================
const UintStringMap
http::AResponse::_sinitStatusMessage(void)
{
    UintStringMap statusMessage;

    statusMessage[CONTINUE]               = "100 Continue";
    statusMessage[SWITCHING_PROTOCOLS]    = "101 Switching Protocols";
    statusMessage[PROCESSING]             = "102 Processing";
    statusMessage[EARLY_HINTS]            = "103 Early Hints";

    statusMessage[OK]                     = "200 OK";
    statusMessage[CREATED]                = "201 Created";
    statusMessage[ACCEPTED]               = "202 Accepted";
    statusMessage[NON_AUTHORITATIVE]      = "203 Non-Authoritative Information";
    statusMessage[NO_CONTENT]             = "204 No Content";
    statusMessage[RESET_CONTENT]          = "205 Reset Content";
    statusMessage[PARTIAL_CONTENT]        = "206 Partial Content";
    statusMessage[MULTI_STATUS]           = "207 Multi-Status";
    statusMessage[ALREADY_REPORTED]       = "208 Already Reported";
    statusMessage[IM_USED]                = "226 IM Used";

    statusMessage[MULTIPLE_CHOICES]       = "300 Multiple Choices";
    statusMessage[MOVED_PERMANENTLY]      = "301 Moved Permanently";
    statusMessage[MOVED_TEMPORARILY]      = "302 Moved Temporarily";
    statusMessage[SEE_OTHER]              = "303 See Other";
    statusMessage[NOT_MODIFIED]           = "304 Not Modified";
    statusMessage[USE_PROXY]              = "305 Use Proxy";
    statusMessage[TEMPORARY_REDIRECT]     = "307 Temporary Redirect";
    statusMessage[PERMANENT_REDIRECT]     = "308 Permanent Redirect";

    statusMessage[BAD_REQUEST]            = "400 Bad Request";
    statusMessage[UNAUTHORIZED]           = "401 Unauthorized";
    statusMessage[FORBIDDEN]              = "403 Forbidden";
    statusMessage[NOT_FOUND]              = "404 Not Found";
    statusMessage[METHOD_NOT_ALLOWED]     = "405 Method Not Allowed";
    statusMessage[NOT_ACCEPTABLE]         = "406 Not Acceptable";
    statusMessage[REQUEST_TIMEOUT]        = "408 Request Timeout";
    statusMessage[CONFLICT]               = "409 Conflict";
    statusMessage[GONE]                   = "410 Gone";
    statusMessage[LENGTH_REQUIRED]        = "411 Length Required";
    statusMessage[PRECONDITION_FAILED]    = "412 Precondition Failed";
    statusMessage[PAYLOAD_TOO_LARGE]      = "413 Payload Too Large";
    statusMessage[URI_TOO_LONG]           = "414 URI Too Long";
    statusMessage[UNSUPPORTED_MEDIA_TYPE] = "415 Unsupported Media Type";
    statusMessage[RANGE_NOT_SATISFIABLE]  = "416 Range Not Satisfiable";
    statusMessage[EXPECTATION_FAILED]     = "417 Expectation Failed";
    statusMessage[IM_A_TEAPOT]            = "418 I'm a teapot";
    statusMessage[MISDIRECTED_REQUEST]    = "421 Misdirected Request";

    statusMessage[INTERNAL_SERVER_ERROR]  = "500 Internal Server Error";
    statusMessage[NOT_IMPLEMENTED]        = "501 Not Implemented";
    statusMessage[BAD_GATEWAY]            = "502 Bad Gateway";
    statusMessage[SERVICE_UNAVAILABLE]    = "503 Service Unavailable";
    statusMessage[GATEWAY_TIMEOUT]        = "504 Gateway Timeout";
    statusMessage[VERSION_NOT_SUPPORTED]  = "505 HTTP Version Not Supported";
    statusMessage[INSUFFICIENT_STORAGE]   = "507 Insufficient Storage";
    statusMessage[LOOP_DETECTED]          = "508 Loop Detected";
    statusMessage[NOT_EXTENDED]           = "510 Not Extended";

    return (statusMessage);
}