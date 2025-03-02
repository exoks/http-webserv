//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀          𓐓  Utils.hpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀  Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇     Created: 2024/05/15 11:58:00 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇     Updated: 2024/05/30 14:05:50 by hael-mou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                             
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                            
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                        𓄂 oussama ezzaou𓆃
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

#ifndef   __UTILS_HPP__
# define   __UTILS_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include <sstream>
# include <cstring>
# include <string>
# include <cstdio>
# include <ctime>

# include <sys/socket.h>
# include <netinet/in.h> 
# include <sys/types.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <stdlib.h> 
# include <unistd.h>
# include <fcntl.h>
# include <netdb.h>

/*******************************************************************************
    * string Utils :
*******************************************************************************/
namespace str
{
    string        strtrim(const string& str);
    string        toLower(const string& str);
    StringPair    lineToPair(const string& line, const char sep);
    StringVector  split(const string str, const char sep);
    string        addrtoString(const in_addr_t& addr, const socklen_t& addrLen);
    string&       replace(string& str, const string& from, const string& to);
    int           strToInt(const string& str); 
    char**        to_Array(const StringVector& strVector);

    template <typename T>
    string    to_string(const T& number)
    {
        std::ostringstream oss;
        oss << number;
        return (oss.str());
    };
};

/*******************************************************************************
    * Logger Utils :
*******************************************************************************/
namespace Logger
{
    void       log(const string& level, const string& message, int fd = 1);
    string     getcurrentTime(const char* format = "%a %b %d %Y %H:%M:%S %Z");
};

/*******************************************************************************
    * Socket Utils :
*******************************************************************************/
namespace sock
{
    Handle  createSocket(int family, int type, int protocol);
    void    bind(Handle socket, const string& host, const string& port);
    void    startListening(Handle socket, int backlog = 10);
    void    setReuseAddr(Handle& socket);
    void    setNonBlocking(Handle& socket);

}

/*******************************************************************************
    * File Utils :
*******************************************************************************/
namespace file
{
    bool       fileExists(const string& path);
    string     generateTempFileName(const string& dir);
    fd_t       createFile(const string& path);
    ssize_t    getFileSize(fd_t fd);
    void       closeFile(fd_t fd);
    void       removeFile(const string& path);
    string     getExtension(const string& file);
    bool       isDirectory(const string& path);
}

/*******************************************************************************
    * Http Utils :
********************************************************************************/
namespace http
{
    std::string tmpDir(const std::string& dir);
    bool        isSpecialChars(const char &c);
    bool        isValidHeader(const char &c);
    bool        isValidMethod(const string& method);
    bool        isValidProtocol(const string& protocol);
    void        decoder(string &str);
    std::string getRelativePath(const string& reqUri, const string& locaUri);
    bool        isCgiPath(const string& path, const StringVector& cgiExt);
}

/*******************************************************************************
    * http Special Chars :
*******************************************************************************/
namespace http
{
    enum SpecialChars{
    Dot                 = '.',
    Dash                =  '-',
    Plus                = '+',
    Comma               = ',',
    Slash               = '/',
    Colon               = ':',
    Space               = ' ',
    Equals              = '=',
    Asterisk            = '*',
    Ampersand           = '&',
    Underscore          ='_',
    QuestionMark        = '?',
    ExclamationMark     = '!',
    LeftCurlyBracket    = '{',
    LeftSquareBracket   = '[',
    RightCurlyBracket   = '}',
    RightSquareBracket  = ']',
    };
}

/*********************************************************************************
    * Status Code :
*******************************************************************************/
namespace http
{
    enum StatusCode
    {
        CONTINUE                        = 100,
        SWITCHING_PROTOCOLS             = 101,
        PROCESSING                      = 102,
        EARLY_HINTS                     = 103,

        OK                              = 200,
        CREATED                         = 201,
        ACCEPTED                        = 202,
        NON_AUTHORITATIVE               = 203,
        NO_CONTENT                      = 204,
        RESET_CONTENT                   = 205,
        PARTIAL_CONTENT                 = 206,
        MULTI_STATUS                    = 207,
        ALREADY_REPORTED                = 208,
        IM_USED                         = 226,

        MULTIPLE_CHOICES                = 300,
        MOVED_PERMANENTLY               = 301,
        MOVED_TEMPORARILY               = 302,
        SEE_OTHER                       = 303,
        NOT_MODIFIED                    = 304,
        USE_PROXY                       = 305,
        SWITCH_PROXY                    = 306,
        TEMPORARY_REDIRECT              = 307,
        PERMANENT_REDIRECT              = 308,

        BAD_REQUEST                     = 400,
        UNAUTHORIZED                    = 401,
        PAYMENT_REQUIRED                = 402,
        FORBIDDEN                       = 403,
        NOT_FOUND                       = 404,
        METHOD_NOT_ALLOWED              = 405,
        NOT_ACCEPTABLE                  = 406,
        REQUEST_TIMEOUT                 = 408,
        CONFLICT                        = 409,
        GONE                            = 410,
        LENGTH_REQUIRED                 = 411,
        PRECONDITION_FAILED             = 412,
        PAYLOAD_TOO_LARGE               = 413,
        URI_TOO_LONG                    = 414,
        UNSUPPORTED_MEDIA_TYPE          = 415,
        RANGE_NOT_SATISFIABLE           = 416,
        EXPECTATION_FAILED              = 417,
        IM_A_TEAPOT                     = 418,
        MISDIRECTED_REQUEST             = 421,

        INTERNAL_SERVER_ERROR           = 500,
        NOT_IMPLEMENTED                 = 501,
        BAD_GATEWAY                     = 502,
        SERVICE_UNAVAILABLE             = 503,
        GATEWAY_TIMEOUT                 = 504,
        VERSION_NOT_SUPPORTED           = 505,
        INSUFFICIENT_STORAGE            = 507,
        LOOP_DETECTED                   = 508,
        NOT_EXTENDED                    = 510,
    };
}

#endif /* __UTILS_HPP__ */
