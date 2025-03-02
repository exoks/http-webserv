/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpBufferReader header file ]                                       #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-22                                                   #
#                                                                              #
** ************************************************************************* **/

# ifndef   __HTTPBUFFERREADER_HPP__
# define    __HTTPBUFFERREADER_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IReader.hpp"

# include "HttpException.hpp"

/*******************************************************************************
    * Class Reader :
*******************************************************************************/
namespace http
{
    class BufferReader : public IReader
    {
        public:
            BufferReader(int aContentLength, int aMaxBodySize);
            virtual ~BufferReader(void);

            void            build(string& ReceivedData);
            void            cleanReader(void);
            bool            isComplete(void) const;
            const string&   getFilePath(void) const;

        private: 
            int         mContentLength;
            int         mMaxBodySize;
            string      mPath;
            fd_t        mFile;
    };
}
# endif /* __HTTPBUFFERREADER_HPP__ */
