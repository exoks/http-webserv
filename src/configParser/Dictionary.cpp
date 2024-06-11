/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Dictionary source file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-21                                                   #
#                                                                              #
** ************************************************************************* **/

#include "Dictionary.hpp"

/*******************************************************************************
    * static Variables :
*******************************************************************************/

Dictionary::MapGrammar
Dictionary::sGrammar = Dictionary::_initializeGrammar(CONFIG_FILE_PATH);

/*******************************************************************************
     * public Methods :
*******************************************************************************/

//===[ Method: getGrammar ]====================================================
DirType Dictionary::find(const std::string& aDir, const std::string& aSubDir)
{
     VectorNameType grammar = sGrammar[aDir];

     for (size_t i = 0; i < grammar.size(); ++i)
     {
         if (grammar[i].first == aSubDir)
             return (grammar[i].second);
     }
     return (Invalid);
}

/*******************************************************************************
    * Private Methods :
*******************************************************************************/

//===[ Method: initGrammar ]===================================================
std::map<std::string, Dictionary::VectorNameType>
        Dictionary::_initializeGrammar(const std::string aConfigFilePath)
{
    std::map<std::string, VectorNameType> grammar;
    std::ifstream configFile(aConfigFilePath.c_str());

    if (!configFile.is_open())
    {
        std::cerr << " Error : failed to open: ";
        std::cerr << aConfigFilePath  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string currentKey, line;
    while (std::getline(configFile, line))
    {
        line = prs::strtrim(line);
        if (line.empty())
            continue;

        if (_isValidKey(line))
            currentKey = line.substr(0, line.length() - 1);
        else if (!currentKey.empty())
            _processValue(line, currentKey, grammar);
    }

    configFile.close();
    return (grammar);
}

//===[ Method: isValidKey ]====================================================
bool Dictionary::_isValidKey(const std::string& line)
{
    size_t len = line.length();
    if (len > 1 && line[len - 1] == ':')
        return (true);
    return (false);
}

//===[ Method: processValue ]==================================================
void Dictionary::_processValue(const std::string&aLine,
                               const std::string&aKey,
                               MapGrammar& aGrammar)
{
    prs::keyValuePair kvp = prs::lineToPair(aLine, '=');
    if (kvp.first.empty())
        return;

    DirType terminalType = kvp.second == "List" ? List :
        (kvp.second == "Complex" ? Complex : Simple);

    aGrammar[aKey].push_back(PairNameType(kvp.first, terminalType));
}
