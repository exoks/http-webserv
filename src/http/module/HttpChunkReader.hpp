 /** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpChunkReader Header File ]                                       #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-22                                                   #
#                                                                              #
** ************************************************************************* **/

# ifndef __HTTPCHUNKREADER_HPP__
# define __HTTPCHUNKREADER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IReader.hpp"

# include "HttpException.hpp"

/*******************************************************************************
    * Class ChunkedReader :
*******************************************************************************/
namespace http
{
    class ChunkReader : public IReader
    {
        typedef bool    (ChunkReader::*Process)(std::string& aLine);

        public:
            ChunkReader(int aMaxBodySize);
            virtual ~ChunkReader();

            void            build(string& ReceivedData);
            void            cleanReader(void);
            bool            isComplete(void) const;
            const string&   getFilePath(void) const;
            
        
        private: 
            Process     mCurrentOperation;
            int         mMaxBodySize;
            bool        mTerminated;
            size_t      mChunkSize;
            string      mPath;
            fd_t        mFile;
        
            bool        _processChunkSize(std::string& aLine);
            bool        _processChunkBody(std::string& aLine);
            void        _unchunk(string aChunk);
    };
}
# endif /* __HTTPCHUNKREADER_HPP__ */
