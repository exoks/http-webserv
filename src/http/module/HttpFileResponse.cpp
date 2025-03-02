/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpFileResponse header file ]                                       #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-18                                                   #
#                                                                              #
** ************************************************************************* **/

#include "HttpFileResponse.hpp"

/*******************************************************************************
    * Construction :
********************************************************************************/

//=== [ Constructor: FileResponse ] ============================================
http::FileResponse::FileResponse(void)
    : mFilePath(""),
      misConverted(false)
{
}

//=== [ Destructor: FileResponse ] =============================================
http::FileResponse::~FileResponse(void)
{
    if (mFileStream.is_open())
        mFileStream.close();
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ setPath: set file path ]================================================
void http::FileResponse::setPath(const string& aFilePath)
{
    mFilePath = aFilePath;
    mFileStream.open(aFilePath.c_str(), std::ios::in);
    if (!mFileStream.is_open() && errno == ENOENT) {
        throw (http::Exception("File not found", http::NOT_FOUND)); 
    }
    if (!mFileStream.is_open() && errno != ENOENT) {
        throw (http::Exception("Access denied", http::FORBIDDEN));
    }
    setHeader("Content-Length", _fileSize());
}

//===[ toRaw: convert file to raw ]=============================================
std::string http::FileResponse::toRaw(void)
{
    if (misConverted == false)
    {
        mReadBuffer = AResponse::toRaw();
        misConverted = true;
    }

    if (mReadBuffer.empty() && mFileStream.is_open())
       mReadBuffer = _readFromFile(READ_BUFFER_SIZE);
    
    return (mReadBuffer);
}

//===[ removeBytesSent: remove bytes sent ]=====================================
void http::FileResponse::removeBytesSent(size_t aBytesSent)
{
    mReadBuffer.erase(0, aBytesSent);
}

//===[ eof: end of file ]======================================================
bool http::FileResponse::eof(void) const
{
    if (!misConverted || !mReadBuffer.empty())
        return (false);
    if (!mFileStream.is_open())
        return (true);
    return (mFileStream.eof());
}

//===[ display: display file ]=================================================
void http::FileResponse::display(void) const
{
    AResponse::display();
    std::cout << "file: "  << mFilePath << std::endl;
    std::cout << "\n=================" << std::endl;
}

//===[ _readFromFile: read file ]==============================================
std::string http::FileResponse::_readFromFile(size_t aSize)
{
    std::string buffer;
    char*       readBuffer = new char[aSize];

    mFileStream.read(readBuffer, aSize);
    buffer.assign(readBuffer, mFileStream.gcount());
    delete[] readBuffer;
    return (buffer);
}

//===[ _fileSize: get file size ]==============================================
std::string http::FileResponse::_fileSize(void)
{
    std::streampos pos = mFileStream.tellg();
    mFileStream.seekg(0, std::ios::end);
    size_t size = mFileStream.tellg() - pos;
    mFileStream.seekg(pos, std::ios::beg);
    return (str::to_string(size));
}
