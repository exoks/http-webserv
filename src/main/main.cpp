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

# include "Initiator.hpp"

int		main(void)
{
	Initiator		initiator(new ConfigParser());
//	Reactor			reactor(new PollMultiplexer);
	Initiator::Handlers	handlers;

	try
	{
		handlers = initiator.init();
		std::cout << "==========================================================" << std::endl;
		for (Initiator::HandlerIter iter = handlers.begin(); iter != handlers.end(); ++iter)
		{
			std::cout << "handler Socket : " << iter->first->getFd() << std::endl;
		}
		std::cout << "GlobalHandlers_Size : " << handlers.size() << std::endl;
		std::cout << "==========================================================" << std::endl;
//		reactor.registerHandler(queue);
//		reactor.waitEvent(2000);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
