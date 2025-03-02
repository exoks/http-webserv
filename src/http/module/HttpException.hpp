/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ HttpException header file ]                                          #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-12                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __HTTPEXCEPTION_HPP__
# define   __HTTPEXCEPTION_HPP__

/*******************************************************************************
	* Includes :
*******************************************************************************/
# include "typedefs.hpp"
#include "Utils.hpp"

# include <exception>
# include <string>

/*******************************************************************************
	* HttpException Class :
*******************************************************************************/
namespace http
{
	class Exception : public std::exception
	{
	public:
		Exception(const string& aMsg, StatusCode aStatusCode);
		virtual ~Exception(void) throw();

		virtual const 	char* what(void) const throw();
		StatusCode		getStatusCode(void) const;
		void			setClientInfo(const string& aClientInfo);

	private:
		string mMsg;
		StatusCode mStatusCode;
	};
}

#endif /* __HTTPEXCEPTION_HPP__ */
