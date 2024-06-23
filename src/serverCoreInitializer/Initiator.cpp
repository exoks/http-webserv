//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀           𓐓  Initiator.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/07 08:05:02 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/22 15:00:04 by oussama
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

# include "Initiator.hpp"

//====| Initiator : default constructor >=======================================
Initiator::Initiator(void)
	: _mConfigFilePath("config/default.conf")
{
}

//====| Initiator : custom constructor >========================================
Initiator::Initiator(ConfigParser *configParser)
	: _mConfigFilePath("config/default.conf")
{
	this->_mConfigParser = configParser;
}

//====| Initiator : destructor >================================================
Initiator::~Initiator(void)
{
	delete _mConfigParser;
	// delete _mGlobalHandlers
}

//====| getConfigFile : get config file path >==================================
std::string	Initiator::getConfigFilePath(void) const
{
	return (this->_mConfigFilePath);
}

//====| setConfigFile : set config file path >==================================
void	Initiator::setConfigFilePath(const ConfigFile aConfigFilePath)
{
	this->_mConfigFilePath = aConfigFilePath;
}

//====| addToGlobalHandlers : add protocol handlers to global handlers >========
bool	Initiator::_addToGlobalHandlers(Handlers aHandlers)
{
	for (HandlerIter iter = aHandlers.begin(); iter != aHandlers.end(); ++iter)
	{
		_mGlobalHandlers.insert(*iter);
	}
	return (true);
}

//====| init : server >=========================================================
std::map<ISocket *, IHandler *>	Initiator::init(void)
{
	Directive					globalDirective;
	NonTerminals				xClusters;
	Handlers					xClusterHandlers;

	_mConfigParser->openFile(this->_mConfigFilePath);
	globalDirective = _mConfigParser->parse();
	xClusters = globalDirective.mNonTerminal;
	if (xClusters.empty() == true)
		std::invalid_argument("Initiator : Empty Cluster");
	for (NonTermsIter clus = xClusters.begin(); clus != xClusters.end(); ++clus)
	{
		Directives directives = clus->second;
		for (DirIter dir = directives.begin(); dir != directives.end(); ++dir) {
			if (clus->first == "http")
				xClusterHandlers = http::Cluster(*dir).createHandlers();
			if (clus->first == "dns")
			;// xClusterHandlers = dns::Cluster(*dir).createHandlers();
			_addToGlobalHandlers(xClusterHandlers);
		}
	}
	return (this->_mGlobalHandlers);
}
