//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀           𓐓  Initiator.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/07 08:05:02 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/19 22:54:03 by oezzaou
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
}

//====| getConfigFile : get config file path >==================================
std::string	Initiator::getConfigFilePath(void) const
{
	return (_mConfigFilePath);
}

//====| setConfigFile : set config file path >==================================
void	Initiator::setConfigFilePath(const ConfigFile aConfigFilePath)
{
	this->_mConfigFilePath = aConfigFilePath;
}

//====| addToGlobalHandlers : add protocol handlers to global handlers >========
bool	Initiator::_addToGlobalHandlers(const Handlers aHandlers)
{
	(void) aHandlers;
	return (true);
}

//====| init : server >=========================================================
std::map<ISocket *, IHandler *>	Initiator::init(void)
{
	Directive				g_directive;
	NonTerminals			nTerms;
	Handlers				protocolHandlers;

	_mConfigParser->openFile(this->_mConfigFilePath);
	g_directive = _mConfigParser->parse();
	nTerms = g_directive.mNonTerminal;
	if (nTerms.empty() == true)
		return (std::map<ISocket *, IHandler *>());
	for (NonTermsIter iter = nTerms.begin(); iter != nTerms.end(); ++iter)
	{
		if (iter->first == "http")
			protocolHandlers = http::Cluster(*iter->second.begin()).createHandlers();
		if (iter->first == "dns")
			; // dns::Cluster(*iter->second.begin()).createHandlers();
		_addToGlobalHandlers(protocolHandlers);
	}
	return (_mGlobalHandlers);
}
