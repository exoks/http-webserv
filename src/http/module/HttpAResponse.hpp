/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpAResponse header file ]                                          #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-18                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __HttpAResponse_HPP__
# define   __HttpAResponse_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "IResponse.hpp"

# include <iostream>

/*******************************************************************************
    * HttpAResponse Class :
*******************************************************************************/
namespace http
{
    class AResponse : public IResponse
    {
    public:   
        AResponse(void);
        virtual	~AResponse(void);

        void    	   setVersion(const std::string& aVersion);
        void    	   setStatusCode(u_int aStatusCode);
        void    	   setHeader(const std::string& aHeader, const string& aValue);
        void           setSendTimeout(time_t aSendTimeout);
        void           setToErrorMode(void);

        const string&  getHeader(const string& aHeader) const;
        time_t         getSendTimeout(void) const;

        std::string    toRaw(void);
        void           log(const string& aClientInfo) const;
        void           display(void) const;

    protected:
        int            mMode;
        std::string    mVersion;
        u_int          mStatusCode;
        std::string    mStatusline;
        StringMap      mHeaders;
        time_t         mSendTimeout;
    
    public:
        static const UintStringMap    sStatusMessage;
        static const UintStringMap    _sinitStatusMessage(void);
    };
}

#endif	/* __HttpAResponse_HPP__ */