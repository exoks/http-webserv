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

#include "HttpRawResponse.hpp"

/*******************************************************************************
	* Construction :
*******************************************************************************/

//===[ Constructor: RawResponse ]==============================================
http::RawResponse::RawResponse(void)
	: mBody(""),
	  mRawMessage(""),
	  misConverted(false),
	  misTemplate(false)
{
}

//===[ Destructor: RawResponse ]===============================================
http::RawResponse::~RawResponse(void) {}

/*******************************************************************************
	* Public Methods :
*******************************************************************************/

//===[ Methode : set Body ]====================================================
void		http::RawResponse::setBody(const string& aBody)
{
	mBody = aBody;

	if (misTemplate == true)
	{
		str::replace(mBody, "$(MESSAGE)", mStatusline);
		str::replace(mBody, "$(SERVER)", getHeader("Server"));
	}
	setHeader("Content-Length", str::to_string(mBody.size()));
}

//===[ Methode : toRaw ]=======================================================
std::string http::RawResponse::toRaw(void) 
{
	if (misConverted == false)
	{
		mRawMessage = AResponse::toRaw();
		mRawMessage += mBody;
		misConverted = true;
	}
	return (mRawMessage);
}

//===[ Methode : removeBytesSent ]=============================================
void http::RawResponse::removeBytesSent(size_t aBytesSent)
{
	mRawMessage.erase(0, aBytesSent);
}

//===[ Methode : eof ]=========================================================
bool http::RawResponse::eof(void) const
{
	return (misConverted && mRawMessage.empty());
}

//===[ Methode : setTemplateOn ]===============================================
void http::RawResponse::setTemplateOn(void)
{
	misTemplate = true;
}

//===[ Methode : display ]=====================================================
void http::RawResponse::display(void) const
{
	AResponse::display();
	std::cout << "Body: \n" << mBody << std::endl;
	std::cout << "=================" << std::endl;
}
