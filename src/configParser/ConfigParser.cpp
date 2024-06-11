//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀        𓐓  ConfigParser.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀   Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇       Created: 2024/06/06 20:49:38 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇       Updated: 2024/06/09 09:52:34 by oezzaou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                              
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                     𓄂 oussama ezzaou𓆃  
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

#include "ConfigParser.hpp"
#include "Directive.hpp"

/*******************************************************************************
    * Construction :
*******************************************************************************/

//===[ Constructor: ]===========================================================
ConfigParser::ConfigParser(void){}

//===[ Destructor: ]============================================================
ConfigParser::~ConfigParser(void)
{
    while (mFiles.empty() == false) {
        CloseLastOpenFile();
    }
}

/*******************************************************************************
    * Public Methods :
*******************************************************************************/

//===[ Method: open a new file ]================================================
void ConfigParser::openFile(const std::string& aFileName, int aStartColumn)
{
    if (isFileAlreadyOpen(aFileName) == true)
        throw (Exception("File_Already_Open", aFileName));
    
    std::fstream*	stream = new std::fstream(aFileName.c_str(), std::ios::in);
    if (!stream->is_open()) {
        delete (stream);
        throw (Exception("Can't_Open_File", aFileName));
    }
    mFiles.push_back(file(aFileName, stream));
    mStartColumn.push_back(aStartColumn);
    mlineCount.push_back(0);
}

//===[ Method: check if a file is already open ]================================
bool ConfigParser::isFileAlreadyOpen(const std::string& aFileName) const
{
    std::vector<file>::const_iterator it;
    for (it = mFiles.begin(); it != mFiles.end(); ++it) {
        if (it->first == aFileName)
            return (true);
    }
    return (false);
}

//===[ Method: close last open file ]===========================================
void	ConfigParser::CloseLastOpenFile(void)
{
    delete (mFiles.back().second);
    mFiles.pop_back();
    mStartColumn.pop_back();
    mlineCount.pop_back();
}

//===[ Method: parse the configuration ]========================================
Directive	ConfigParser::parse(void)
{
    std::vector<Directive>		directives;
    std::vector<DirectivePart>	dirParts;
    std::string					line;

    while (!mFiles.empty()) {
        while (std::getline(*(mFiles.back().second), line)) {
            mlineCount.back()++;
            _processLine(line, directives, dirParts);
        }
        if (mFiles.size() == 1 && dirParts.empty() == true)
            throw (Exception("Empty_Configuration", mFiles.back().first));
        CloseLastOpenFile();
    }
    dirParts::iterator iter = dirParts.begin();
    directives.push_back(Directive(iter, dirParts.end()));
    // Gather the directives in one global directive
	return (*directives.begin());
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: process a line ]=================================================
void ConfigParser::_processLine(std::string& aLine,
                               directive& aDirs,
                               dirParts& aDirParts)
{
    aLine = aLine.substr(0, aLine.find_first_of('#'));
    if (prs::strtrim(aLine).empty())
        return ;
    int level = getLevel(aLine);
    if (includeDirective(aLine, level) == true)
        return ;
    if (isValidDirective(aDirParts, level) == true) {
        dirParts::iterator iter = aDirParts.begin();
        aDirs.push_back(Directive(iter, aDirParts.end()));
        aDirParts.clear();
    }
    std::string file = mFiles.back().first;
    int nline = mlineCount.back();
    aDirParts.push_back(DirectivePart(aLine, file, nline, level));
}

//===[ Method: get level ]======================================================
int	ConfigParser::getLevel(const std::string& aLine)
{
    std::vector<int>::iterator	iter;
    int							column;

    column = aLine.find_first_not_of(' ') + mStartColumn.back();
    iter = find(mLevels.begin(), mLevels.end(), column);
    if (mLevels.empty() == false && iter != mLevels.end())
        return (iter - mLevels.begin());
    mLevels.push_back(column); 
    return (static_cast<int>(mLevels.size()) - 1);
}

//===[ Method: is a complete Parts of a directive ]=============================
bool ConfigParser::isValidDirective(const dirParts &aDirParts, int aLevel) const
{
    if (aDirParts.size() == 1 && aDirParts[0].mType == Terminal)
    {
        if (aDirParts[0].mLevel > 0)
        return (true);
    }
    return (!aDirParts.empty() && mLevels[aLevel] <= mLevels[0]);
}

//===[ Method: include directive ]==============================================
bool ConfigParser::includeDirective(const std::string& aLine, int aLevel)
{
    if (aLine.find("include") == std::string::npos)
        return (false);
    prs::keyValuePair kv = prs::lineToPair(aLine, ':');
    if (kv.first != "include" || kv.second.empty())
        return (false);
    openFile(kv.second, mLevels[aLevel]);
    return (true);
}

/*******************************************************************************
    * ConfigParser Exception :
*******************************************************************************/

//===[ Constructor: Exception ]=================================================
ConfigParser::Exception::Exception(std::string aMessage, std::string aLine)
{
    mMessage =  "\e[1m\e[1;31m" + aMessage 	+ ": " 
                "\e[0m\e[3m"	+ aLine;
}

//===[ Destructor: Exception ]==================================================
ConfigParser::Exception::~Exception() throw() {};

//===[ Method: what ]===========================================================
const char* ConfigParser::Exception::what() const throw()
{
    return (mMessage.c_str());
}
