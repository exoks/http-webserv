/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Shared_ptr header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-06-26                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef  __TYPEDEFS_HPP__
# define  __TYPEDEFS_HPP__

//====[ Includes : ] ===========================================================
# include <string>
# include <vector>
# include <queue>
# include <map>

//===[ definitions : ] ==========================================================
const std::string                       ServerVersion = "WebServ/1.0.0";
const std::string                       CgiVersion = "CGI/1.0.0";
const std::string                       EmptyString = "";
const std::string                       CRLF_CRLF = "\r\n\r\n";
const std::string                       CRLF = "\r\n";

//===[ Stirng : ] ==============================================================
typedef std::string                     string;
typedef std::pair<string, string>       StringPair;
typedef std::vector<string>             StringVector;
typedef std::map<string, string>        StringMap;

//===[ integer : ] =============================================================
typedef unsigned int                    u_int;
typedef long                            ssize_t;

//===[ int/Stirng : ] ==========================================================
typedef std::map<u_int, string>         UintStringMap;

//===[ Handle : ] ==============================================================
typedef int                             Handle;
typedef int							    fd_t;
typedef string                          Address;
typedef string                          Port;
typedef std::queue<Handle>              HandleQueue;

#endif /* __TYPEDEFS_HPP__ */