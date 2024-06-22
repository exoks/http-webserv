//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀          𓐓  HttpSocket.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oussama <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/18 17:09:56 by oussama
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/21 21:43:00 by oezzaou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

# include "HttpSocket.hpp"

//====| constructor : >=========================================================
http::Socket::Socket(void)
{
}

//====| destructor : close socket fd >==========================================
http::Socket::~Socket(void)
{
	close(this->_mFd);
}

//====| constructor : >=========================================================
http::Socket::Socket(std::string aListen)
{
	std::pair<std::string, std::string>	hostPort;

	hostPort = prs::lineToPair(aListen, ':');
	this->_mHostName = hostPort.first;
	this->_mPort = hostPort.second;
}

//====| operator == : >=========================================================
bool	http::Socket::operator !=(const ISocket & aSocket)
{
	return (_mHostName != aSocket.getHostName() || _mPort != aSocket.getPort());
}

//====| getFd : fd getter >=====================================================
int	http::Socket::getFd(void) const
{
	return (this->_mFd);
}

//====| getPort : port getter >=================================================
std::string	http::Socket::getPort(void) const
{
	return (this->_mPort);
}

//====| getHostName : host getter >=============================================
std::string	http::Socket::getHostName(void) const
{
	return (this->_mHostName);
}

//====| createSocket : create http socket (Tcp Socket) >========================
bool	http::Socket::bindSocket(void)
{
	struct addrinfo			hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
//	addFlags && set Socket to non-blocking mode	
	if (getaddrinfo(_mHostName.c_str(), _mPort.c_str(), &hints, &res)) {
		std::cerr << "Socket : " << gai_strerror(errno) << std::endl;
		return (false);
	}
    this->_mFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_mFd <= -1 || bind(_mFd, res->ai_addr, res->ai_addrlen) != 0) {
		std::cerr << _mHostName + ": " << std::strerror(errno) << std::endl;
		return (freeaddrinfo(res), false);
	}
	return (freeaddrinfo(res), true);
}


/*for (addrinfo *node = res; node != NULL; node = node->ai_next)
{
	sockaddr_in *sockaddr = (sockaddr_in *) node->ai_addr;
	std::cout << "Port : " << sockaddr->sin_port << std::endl;
	std::cout << "HostName : " << sockaddr->sin_addr.s_addr << std::endl;
	std::cout << "Type : " << node->ai_socktype << std::endl;
	std::cout << "========================" << std::endl;
}*/
