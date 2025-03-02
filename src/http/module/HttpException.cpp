/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpException implementation file ]                                  #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpException.hpp"

//=== [ constructor: HttpException ] ===========================================
http::Exception::Exception(const string& aMsg, StatusCode aStatusCode)
{
	mMsg = aMsg;
	mStatusCode = aStatusCode;
}

//=== [ destructor: HttpException ] ============================================
http::Exception::~Exception(void) throw()
{
}

//=== [ what: HttpException ] =================================================
const char* http::Exception::what(void) const throw()
{
	return (mMsg.c_str());
}

//=== [ getStatusCode: HttpException ] =========================================
http::StatusCode http::Exception::getStatusCode(void) const
{
	return (mStatusCode);
}

//=== [ setClientInfo: HttpException ] =========================================
void http::Exception::setClientInfo(const string& aClientInfo)
{
	mMsg = "'" + aClientInfo + "' " + mMsg;
}
