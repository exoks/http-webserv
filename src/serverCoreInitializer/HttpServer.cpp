//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀          𓐓  HttpServer.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 20:40:54 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/24 19:44:53 by oezzaou
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
	if (this->_mRoot.empty() == false || this->_mRoot == aRoot)
	{
		std::cerr << "Duplicated Location: Root: " + aRoot << std::endl;
		return;
	}
	this->_mRoot = aRoot;
}

//====| addIndex : add Index member locaiton >==================================
void	http::Server::Location::addIndex(String aIndex)
{
	if (std::find(_mIndex.begin(), _mIndex.end(), aIndex) != _mIndex.end())
	{
		std::cerr << "Duplicated Location: Index: " + aIndex << std::endl;
		return ;
	}
	this->_mIndex.push_back(aIndex);
}	

//====| addMember : add locations >=============================================
void	http::Server::Location::addMember(std::pair<String, std::vector<String> > aMember)
{
	std::vector<String>		memberVals = aMember.second;
	String					memberName = aMember.first;
	String					member[2] = {"root", "index"};
	void					(http::Server::Location::*fun[2])(String) = {
															&Location::addRoot,
															&Location::addIndex
															};

	for (unsigned int memberIdx = 0; memberIdx < 2; ++memberIdx)
	{
		unsigned int valIdx = -1;
		while (++valIdx < memberVals.size() && member[memberIdx] == memberName)
			(this->*fun[memberIdx])(memberVals[valIdx]);
	}
}

//====| addLocation : add Location >============================================
bool	http::Server::addLocation(std::map<String, std::vector<String> > aLocation)
{
	(void) aLocation;
	return (true);
}
