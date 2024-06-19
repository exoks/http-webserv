//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀         𓐓  HttpCluster.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 19:48:21 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/18 23:28:13 by oussama
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

# include "HttpCluster.hpp"

//====| Cluster : default constructor >==========================================
http::Cluster::Cluster(void)
{
}

//====| Cluster : default constructor >==========================================
http::Cluster::Cluster(Directive httpDirective)
{
	this->_mHttpDirective = httpDirective;
}

//====| Cluster : destructor >===================================================
http::Cluster::~Cluster(void)
{
}

//====| createServer : create server instance from Directive >===================
IServer *http::Cluster::_createServer(Directive server, Terminals usedTerms)
{
	(void) server;
	(void) usedTerms;
	return (http::ProtocolFactory::createServer());
}

//====| createSockets : create sockets instances used by servers >===============
std::vector<ISocket *>	http::Cluster::_createSockets(Terminals dirTerms, Terminals usedTerms)
{
	std::vector<std::string>	listen;
	Sockets				socket;
	ISocket				*tmp;
	
	if (usedTerms.lower_bound("listen") != usedTerms.end())
		listen = usedTerms.lower_bound("listen")->second;
	if (dirTerms.lower_bound("listen") != dirTerms.end())
		listen = dirTerms.lower_bound("listen")->second;
	for (unsigned int index = 0; index < listen.size(); index++)
	{
		tmp = http::ProtocolFactory::createSocket(listen[index]); 
		if (prs::find(_mHandlers, tmp) == _mHandlers.end())
		{
			socket.push_back(tmp);
			continue ;
		}	
		delete tmp;
	}
	return (socket);
}

void	http::Cluster::_addServerToHandler(ISocket *aSocket, IServer *aServer)
{
	(void) aSocket;
	(void) aServer;
	if (prs::find(_mHandlers, aSocket) != _mHandlers.end())
		; // add to handler
	else
		; // create new handler
}

//====| createAcceptHandler : create accept handlers based on sockets >========
IHandler *http::Cluster::_createAcceptHandlers(std::string key, Directives servDirs)
{
	Terminals			usedTerms;
	Sockets				socket;
	IServer				*server;

	usedTerms = _mFilteredTerms.lower_bound(key)->second;
	for (DirIter dir = servDirs.begin(); dir != servDirs.end(); ++dir)
	{
		socket = _createSockets(dir->mTerminal, usedTerms);
		for (SockIter it = socket.begin(); it != socket.end(); ++it)
		{
			server = _createServer(*dir, usedTerms);
			_addServerToHandler(*it, server);
		}
	}
	return (NULL);
}

//====| filterTerminals : filter terminals based on nonTerminal key >===========
void	http::Cluster::_filterTerminals(const std::string key)
{
	Terminals		gTerms, usedTerms;

	gTerms = _mHttpDirective.mTerminal;
	if (_mFilteredTerms.lower_bound(key) != _mFilteredTerms.end())
		return ;
	for (TermsIter iter = gTerms.begin(); iter != gTerms.end(); ++iter)
	{
		if (Dictionary::find(key, iter->first) <= (Simple | List))
			usedTerms.insert(*iter);
	}
	_mFilteredTerms.insert(std::pair<std::string, Terminals>(key, usedTerms));
};

//====| createHandlers : create http handlers used by reactor >==================
std::map<ISocket *, IHandler *> http::Cluster::createHandlers(void)
{
	NonTerminals		nTerms;

	nTerms = _mHttpDirective.mNonTerminal;
	for (NonTermsIter iter = nTerms.begin(); iter != nTerms.end(); ++iter)
	{
		_filterTerminals(iter->first);
		if (iter->first == "server")
			_createAcceptHandlers(iter->first, iter->second);
	}
	return (_mHandlers);
}
