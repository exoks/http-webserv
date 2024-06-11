/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Main Program File ]                                                 #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-21                                                   #
#                                                                              #
** ************************************************************************* **/

# include "ConfigParser.hpp"
# include "Initiator.hpp"

class	IHandler;

int		main(void)
{
	Initiator	initiator(new ConfigParser());
//	Reactor		reactor(new PollMultiplexer);
	
	try
	{
		initiator.init();
//		reactor.registerHandler(queue);
//		reactor.waitEvent(2000);	
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
