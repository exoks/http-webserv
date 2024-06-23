//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀         𓐓  HttpCluster.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 19:48:21 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/23 19:25:20 by oezzaou
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

//====| getNecessaryTerms : Wich terms must be used (dirTerms or gDirTerms) >===
std::vector<std::string> http::Cluster::_getTerms(String aKey, Terminals dirTerms, Terminals gDirTerms)
{
	TermsIter				termsIter;

	termsIter = dirTerms.lower_bound(aKey);
	if (termsIter != dirTerms.end())
		return (termsIter->second);
	termsIter = gDirTerms.lower_bound(aKey);
	if (termsIter != gDirTerms.end())
		return (termsIter->second);
	return (std::vector<String>());
}

//====| createServer : create server instance from Directive >==================
IServer *http::Cluster::_createServer(Directive srvDir, Terminals gSrvDirTerms)
{
	(void) gSrvDirTerms;
	(void) srvDir;

	std::map<std::string, std::vector<std::string> >	dirlocations;

	NonTerminals	subDir = srvDir.mNonTerminal;
	for (NonTermsIter iter = subDir.begin(); iter != subDir.end(); ++iter)
	{
		if (iter->first == "location")
		{
			std::vector<Directive> locations = iter->second;
			for (DirIter it = locations.begin(); it != locations.end(); ++it)
				; //_addLocations(it->mTerminal);
		}
	}
	return (NULL);
}

//====| createSockets : create sockets instances used by servers >==============
std::vector<ISocket *> http::Cluster::_createSockets(Terminals dirTerms, Terminals gDirTerms)
{
	std::vector<String>						listen;
	Sockets									sockets;
	ISocket									*tmpSocket;

	listen = _getTerms("listen", dirTerms, gDirTerms);
	for (ListenIter iter = listen.begin(); iter != listen.end(); ++iter) {
		tmpSocket = http::ProtocolFactory::createSocket(*iter);
		HandlerIter hIter = alg::find(_mHttpHandlers, tmpSocket);
		if (hIter == this->_mHttpHandlers.end()) {
			if (tmpSocket->bindSocket() == true)
				sockets.push_back(tmpSocket);
		}
		else {
			sockets.push_back(hIter->first);
			delete tmpSocket;
		}
	}
	return (sockets);
}

//====| _addServerToHandler : add Server to handler >===========================
void	http::Cluster::_addServerToHandler(ISocket *aSocket, IServer *aServer)
{
	IHandler					*acceptHandler;
	HandlerIter					handlerIter;

	handlerIter = alg::find(_mHttpHandlers, aSocket);
	if (handlerIter != _mHttpHandlers.end()) {
		handlerIter->second->addServer(aServer);
		return ;
	}
	acceptHandler = http::ProtocolFactory::createAcceptHandler();
	acceptHandler->addServer(aServer);
	this->_mHttpHandlers.insert(HandlerPair(aSocket, acceptHandler));
}

//====| createAcceptHandler : create accept handlers based on sockets >=========
void http::Cluster::_createAcceptHandlers(String key, Directives servDirs)
{
	Terminals					gDirTerms;
	Sockets						sockets;

	gDirTerms = this->_mFilteredTerms.lower_bound(key)->second;
	for (DirIter dir = servDirs.begin(); dir != servDirs.end(); ++dir)
	{
		sockets = _createSockets(dir->mTerminal, gDirTerms);
		for (SockIter sock = sockets.begin(); sock != sockets.end(); ++sock)
			_addServerToHandler(*sock, _createServer(*dir, gDirTerms));
	}
}

//====| filterTerminals : filter terminals based on nonTerminal key >===========
void	http::Cluster::_filterTerminals(const String key)
{
	Terminals			gTerms, gDirTerms;

	gTerms = this->_mHttpDirective.mTerminal;
	if (_mFilteredTerms.lower_bound(key) != _mFilteredTerms.end())
		return ;
	for (TermsIter iter = gTerms.begin(); iter != gTerms.end(); ++iter)
	{
		if (Dictionary::find(key, iter->first) <= (Simple | List))
			gDirTerms.insert(*iter);
	}
	this->_mFilteredTerms.insert(std::pair<String, Terminals>(key, gDirTerms));
};

//====| createHandlers : create http handlers used by reactor >=================
std::map<ISocket *, IHandler *>	http::Cluster::createHandlers(void)
{
	NonTerminals				nTerms;

	nTerms = _mHttpDirective.mNonTerminal;
	for (NonTermsIter iter = nTerms.begin(); iter != nTerms.end(); ++iter)
	{
		_filterTerminals(iter->first);
		if (iter->first == "server")
			_createAcceptHandlers(iter->first, iter->second);
	}
	return (this->_mHttpHandlers);
}
