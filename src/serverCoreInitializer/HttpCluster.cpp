//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀         𓐓  HttpCluster.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 19:48:21 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/22 20:06:43 by oussama
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

# include "HttpCluster.hpp"

//====| Cluster : default constructor >=========================================
http::Cluster::Cluster(void)
{
}

//====| Cluster : default constructor >=========================================
http::Cluster::Cluster(Directive httpDirective)
{
	this->_mHttpDirective = httpDirective;
}

//====| Cluster : destructor >==================================================
http::Cluster::~Cluster(void)
{
}

//====| getNecessaryTerms : Wich terms must be used dirTerms or usedTerms >=====
std::vector<std::string> http::Cluster::_getNecessaryTerms(std::string aKey, Terminals dirTerms, Terminals usedTerms)
{
	TermsIter		iter;

	iter = dirTerms.lower_bound(aKey);
	if (iter != dirTerms.end())
		return (iter->second);
	iter = usedTerms.lower_bound(aKey);
	if (iter != usedTerms.end())
		return (iter->second);
	return (std::vector<std::string>());
}

//====| createServer : create server instance from Directive >==================
IServer *http::Cluster::_createServer(Directive servDir, Terminals usedTerms)
{
	(void) usedTerms;
	(void) servDir;

	// start with terminals
	
	return (NULL);
}

//====| createSockets : create sockets instances used by servers >==============
std::vector<ISocket *> http::Cluster::_createSockets(Terminals dirTerms, Terminals usedTerms)
{
	std::vector<std::string>		listen;
	Sockets							sockets;
	ISocket							*tmpSocket;

	// This part must be optimized 
	listen = _getNecessaryTerms("listen", dirTerms, usedTerms);
	for (ListenIter iter = listen.begin(); iter != listen.end(); ++iter) {
		tmpSocket = http::ProtocolFactory::createSocket(*iter);
		if (_findHandler(tmpSocket) == _mHttpHandlers.end()) {
			if (tmpSocket->bindSocket() == true)
				sockets.push_back(tmpSocket);
		}
		else {
			sockets.push_back(_findHandler(tmpSocket)->first);
			delete tmpSocket;
		}
	}
	return (sockets);
}

// use wrapped lower_bound or find
//====| _findHandler : search for handler using socket >========================
http::Cluster::HandlerIter http::Cluster::_findHandler(ISocket *aSocket)
{
	HandlerIter	iter;

	iter = _mHttpHandlers.begin();
	while (iter != _mHttpHandlers.end() && *iter->first != *aSocket)
		++iter;
	return (iter);
}

//====| _addServerToHandler : add Server to handler >===========================
void	http::Cluster::_addServerToHandler(ISocket *aSocket, IServer *aServer)
{
	IHandler					*acceptHandler;
	HandlerIter					handlerIter;

	handlerIter = _findHandler(aSocket);
	if (handlerIter != _mHttpHandlers.end()) {
		handlerIter->second->addServer(aServer);
		return ;
	}
	acceptHandler = http::ProtocolFactory::createAcceptHandler();
	acceptHandler->addServer(aServer);
	_mHttpHandlers.insert(HandlerPair(aSocket, acceptHandler));
}

//====| createAcceptHandler : create accept handlers based on sockets >=========
void http::Cluster::_createAcceptHandlers(std::string key, Directives servDirs)
{
	Terminals					usedTerms;
	Sockets						sockets;

	usedTerms = _mFilteredTerms.lower_bound(key)->second;
	for (DirIter dir = servDirs.begin(); dir != servDirs.end(); ++dir)
	{
		sockets = _createSockets(dir->mTerminal, usedTerms);
		for (SockIter sock = sockets.begin(); sock != sockets.end(); ++sock)
			_addServerToHandler(*sock, _createServer(*dir, usedTerms));
	}
}

//====| filterTerminals : filter terminals based on nonTerminal key >===========
void	http::Cluster::_filterTerminals(const std::string key)
{
	Terminals			gTerms, usedTerms;

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

//====| createHandlers : create http handlers used by reactor >=================
std::map<ISocket *, IHandler *> http::Cluster::createHandlers(void)
{
	NonTerminals				nTerms;

	nTerms = _mHttpDirective.mNonTerminal;
	for (NonTermsIter iter = nTerms.begin(); iter != nTerms.end(); ++iter)
	{
		_filterTerminals(iter->first);
		if (iter->first == "server")
			_createAcceptHandlers(iter->first, iter->second);
	}
	return (_mHttpHandlers);
}
