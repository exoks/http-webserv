/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpBufferReader implementation file ]                               #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-22                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpBufferReader.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//=== [ Constructor: BufferReader ] ============================================
http::BufferReader::BufferReader(int aContentLength, int aMaxBodySize)
    : mContentLength(aContentLength),
      mMaxBodySize(aMaxBodySize),
      mFile(-1)
{
    mPath = file::generateTempFileName(http::tmpDir(""));
    mFile = file::createFile(mPath);
    if (mFile == -1) {
        std::string msg = "Can't create temporary file";
        throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR));
    }
}

//=== [ Destructor: BufferReader ] =============================================
http::BufferReader::~BufferReader(void)
{
    file::closeFile(mFile);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//=== [ Method: build ] =======================================================
void    http::BufferReader::build(string& aRecvData)
{
    ssize_t  bytes_written;

    while (aRecvData.size() > 0)
    {
        bytes_written = ::write(mFile, aRecvData.c_str(),aRecvData.size());
        aRecvData.erase(0, bytes_written);
        if (bytes_written == -1) {
            cleanReader();
            std::string msg = "Can't write to temporary file";
            throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR));
        }
    }
    
    ssize_t fileSize = file::getFileSize(mFile); 
    if (fileSize == -1)
    {
        cleanReader();
        std::string msg = "Can't get temporary file size";
        throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR));
    }

    if ((mMaxBodySize && mContentLength > mMaxBodySize)
        || fileSize > (ssize_t)mContentLength)
    {
        cleanReader();
        std::string msg = "Body size is too big";
        throw (http::Exception(msg, http::PAYLOAD_TOO_LARGE)); 
    }
}

//=== [ Method: cleanReader ] =================================================
void    http::BufferReader::cleanReader(void)
{
    file::closeFile(mFile);
    file::removeFile(mPath);
}

//=== [ Method: isComplete ] ===================================================
bool    http::BufferReader::isComplete(void) const
{
    return (file::getFileSize(mFile) == mContentLength);
}

//=== [ Method: getFilePath ] ==================================================
const string&    http::BufferReader::getFilePath(void) const
{
    return (mPath);
}
