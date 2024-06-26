//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀          𓐓  HttpServer.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 20:40:54 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/26 20:05:59 by oezzaou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

# include "HttpServer.hpp"

//====| HttpServer : default constructor >======================================
http::Server::Server(void)
{
}

//====| ~HttpServer : destructor >==============================================
http::Server::~Server(void)
{
}

//====| _addHostName : add HostName to the existing host names >================
bool	http::Server::addHostName(const String aHostName)
{
	this->_mHostNames.push_back(aHostName);
	return (true);
}

//====| addIndex : add Index member locaiton >==================================
void	http::Server::Location::addRoot(String aRoot)
{
	if (this->_mRoot.empty() != false && this->_mRoot != aRoot)
		this->_mRoot = aRoot;
}

//====| addIndex : add Index member locaiton >==================================
void	http::Server::Location::addIndex(String aIndex)
{
	if (std::find(_mIndex.begin(), _mIndex.end(), aIndex) == _mIndex.end())
		this->_mIndex.push_back(aIndex);
}	

//====| addMember : add locations >=============================================
http::Server::Location	http::Server
	::Location::addMember(String aKey, std::vector<String> aVal)
{
	String					member[2] = {"root", "index"};
	void					(http::Server::Location::*fun[2])(String) = {
															&Location::addRoot,
															&Location::addIndex
															};

	for (unsigned int memberIdx = 0; memberIdx < 2; ++memberIdx)
	{
		unsigned int valIdx = -1;
		while (++valIdx < aVal.size() && member[memberIdx] == aKey)
			(this->*fun[memberIdx])(aVal[valIdx]);
	}
	return (*this);
}

//====| addLocation : add Location >============================================
void	http::Server::addLocation(Terminals aLocation, Terminals aServer, Terminals aHttp)
{
	Terminals					*data[3] = {&aLocation, &aServer, &aHttp};
	Server::Location			location;
	Terminals					*tmp;

	for (unsigned int index = 0; index < 3; ++index)
	{
		tmp = data[index];
		for (TermsIter iter = tmp->begin(); iter != tmp->end(); ++iter)
			location.addMember(iter->first, iter->second);
	}
	this->_mLocations.push_back(location);
}
