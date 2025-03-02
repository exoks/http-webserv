/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpChunkReader Implementation ]                                    #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-26                                                   #
#                                                                              #
** ************************************************************************* **/

# include "HttpChunkReader.hpp"
# include <iostream>
/*******************************************************************************
    * Construction :
*******************************************************************************/

//=== [ Constructor: BufferReader ] ============================================
http::ChunkReader::ChunkReader(int aMaxBodySize)
    : mMaxBodySize(aMaxBodySize),
      mFile(-1)
{
    mPath = file::generateTempFileName(http::tmpDir(""));
    mFile = file::createFile(mPath);
    if (mFile == -1) {
        std::string msg = "Can't create temporary file";
        throw (http::Exception(msg, http::INTERNAL_SERVER_ERROR));
    }
    mCurrentOperation = &ChunkReader::_processChunkSize;
}

//=== [ Destructor: BufferReader ] =============================================
http::ChunkReader::~ChunkReader(void)
{
    file::closeFile(mFile);
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//=== [ Public Methods: BufferReader ] ==========================================
void    http::ChunkReader::build(std::string& aReceivedData)
{
    std::cout << "ChunkReader::build" << std::endl;
    while (true)
    {
        if (mTerminated == true)
        {
            cleanReader();
            std::string msg = "Invalid Chunked Format";
            throw (http::Exception(msg, http::BAD_REQUEST));
        }

        if (!(this->*mCurrentOperation)(aReceivedData))
        {
            break ;
        }
    }
}

//=== [ Method: cleanReader ] =================================================
void    http::ChunkReader::cleanReader(void)
{
    file::closeFile(mFile);
    file::removeFile(mPath);
}

//===[ Method: isComplete ]=====================================================
bool http::ChunkReader::isComplete() const
{
    return (mTerminated);
}


const string&   http::ChunkReader::getFilePath(void) const
{
    return (mPath);
}

/*******************************************************************************
    * Private Method of HttpReader:
*******************************************************************************/

//===[ Method: ChunkSizeParser ]================================================
bool    http::ChunkReader::_processChunkSize(std::string& aLine)
{
    size_t crlfPos = aLine.find(CRLF);
    if (aLine.empty() || crlfPos == string::npos)
        return (false);

    errno = 0;
    string DataSizeLine = aLine.substr(0, crlfPos + 2);
    mChunkSize = std::strtol(DataSizeLine.c_str(), NULL, 16);
    if (mChunkSize < 0 || errno != 0)
    {
        cleanReader();
        std::string msg = "Invalid chunk size";
        throw (http::Exception(msg, http::BAD_REQUEST));
    }

    aLine = aLine.substr(crlfPos + 2);
    mCurrentOperation = &ChunkReader::_processChunkBody;
    return (true);
}

//===[ Method: ChunkBodyParser ]================================================
bool    http::ChunkReader::_processChunkBody(std::string& aLine)
{
    if (mChunkSize == 0 && aLine.size() == 2)
    {
        mTerminated = true;
        return (false);
    }

    if (aLine.size() >= mChunkSize + 2)
    {
        _unchunk(aLine.substr(0, mChunkSize + 2));
        aLine = aLine.substr(mChunkSize + 2);
        mCurrentOperation = &ChunkReader::_processChunkSize;
        return (true);
    }
    return (false);
}

//===[ Method: unchunk ]========================================================
void    http::ChunkReader::_unchunk(std::string aChunk)
{
    if (aChunk.find(CRLF) != mChunkSize) {
        cleanReader();
        std::string msg = "Invalid chunk size";
        throw (http::Exception(msg, http::BAD_REQUEST));
    }

    aChunk = aChunk.substr(0, mChunkSize);
     while (aChunk.size() > 0)
    {
        ssize_t  bytes_written = ::write(mFile, aChunk.c_str(), aChunk.size());
        aChunk.erase(0, bytes_written);
        if (bytes_written == -1) {
            cleanReader(); 
            std::string msg = "Can't write to chunk temporary file";
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

    if ((mMaxBodySize && fileSize > (ssize_t)mMaxBodySize))
    {
        cleanReader();
        std::string msg = "Body size is too big";
        throw (http::Exception(msg, http::PAYLOAD_TOO_LARGE)); 
    }
}


