/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpCgiResponse Implementation file ]                                #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-18                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpCgiResponse.hpp"

/*******************************************************************************
	* Construction :
*******************************************************************************/

//=== [ Constructor: CgiResponse ] =============================================
http::CgiResponse::CgiResponse(void)
{
	setStatusCode(200);
}

//=== [ Destructor: CgiResponse ] ==============================================
http::CgiResponse::~CgiResponse(void)
{
	file::removeFile(mFilePath);
}

/*******************************************************************************
	* Public Methods :
*******************************************************************************/

//===[ setPath: set file path ]================================================
void http::CgiResponse::setPath(const string& aCgiOut)
{
	mFilePath = aCgiOut;
	mFileStream.open(mFilePath.c_str(), std::ios::in);
    if (!mFileStream.is_open() && errno == ENOENT) {
        throw (http::Exception("File not found", http::NOT_FOUND)); 
    }
    if (!mFileStream.is_open() && errno != ENOENT) {
        throw (http::Exception("Access denied", http::FORBIDDEN));
    }
	_parseCgiHeaders();
}

/*******************************************************************************
	* Private Methods :
*******************************************************************************/

//===[ _parseCgiHeaders: parse cgi headers ]====================================
void http::CgiResponse::_parseCgiHeaders(void)
{
	while (getline(mFileStream, mReadBuffer))
	{
		if (!mReadBuffer.empty() && mReadBuffer[mReadBuffer.size() - 1] == '\r')
		{
			mReadBuffer.resize(mReadBuffer.size() - 1);
		}
		if (mFileStream.eof() && !mReadBuffer.empty())
		{
		  throw (http::Exception("cgi Bad response", http::INTERNAL_SERVER_ERROR));
		}
		if (mReadBuffer.empty())
			break ;
		_setHeaders(mReadBuffer);
	}
	setHeader("Content-Length", _fileSize());
}

//===[ Method: set Headers ]====================================================
void http::CgiResponse::_setHeaders(string& aLine)
{
	if (aLine.find(':') == string::npos)
	{
		throw (http::Exception("Invalid header", http::INTERNAL_SERVER_ERROR));
	}

	StringPair header = str::lineToPair(aLine, ':');

	for (size_t index = 0; index < header.first.size(); ++index)
	{
		if (http::isValidHeader(header.first[index]) == false)
		{
			throw (http::Exception("Invalid header", http::INTERNAL_SERVER_ERROR));
		}
	}

	header.first = str::toLower(header.first);
	if (header.first == "status") 
	{
		int status = std::atoi(header.second.c_str());
		if (status < 100 || status > 599) {
			throw (http::Exception("cgi Invalid status", http::INTERNAL_SERVER_ERROR));
		}
		setStatusCode(status);
		return ;
	}
	setHeader(header.first, header.second);
}
