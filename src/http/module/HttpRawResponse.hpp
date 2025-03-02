/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpRawResponse header file ]                                        #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-16                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __HttpRawResponse_HPP__
# define   __HttpRawResponse_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "HttpAResponse.hpp"

# include <iostream>

/*******************************************************************************
    * HttpRawResponse Class :
*******************************************************************************/
namespace http
{
    class RawResponse : public AResponse
    {
    public:
        RawResponse(void);
        virtual ~RawResponse(void);

        void        setBody(const string& aBody);
        string      toRaw(void);
        void        removeBytesSent(size_t aBytesSent);
        bool        eof(void) const;
        void        setTemplateOn(void);
        void        display(void) const;
    
    private:
        string      mBody;
        string      mRawMessage;
        bool        misConverted;
        bool        misTemplate;
    };
}

#endif	/* __HttpRawResponse_HPP__ */