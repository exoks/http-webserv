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
Dictionary::GrammarMap
Dictionary::sGrammar = Dictionary::_initializeGrammar(CONFIG_FILE_PATH);

/*******************************************************************************
     * public Methods :
*******************************************************************************/

//===[ Method: getGrammar ]=====================================================
DirType  Dictionary::find(const string& aDir, const string& aSubDir)
{
    NonTerminalDirVector grammar = sGrammar[aDir];

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

//===[ Method: initGrammar ]====================================================
Dictionary::GrammarMap Dictionary::_initializeGrammar(const string aFilePath)
{
    GrammarMap      grammar;
    std::ifstream   configFile(aFilePath.c_str());

    if (!configFile.is_open())
    {
        Logger::log("error  ", "\e[1;31m Grammar_File_Failed_To_Open: \e[0m"
                + aFilePath + "\n", 2);
        std::exit(EXIT_FAILURE);
    }

    string currentKey, line;
    while (std::getline(configFile, line))
    {
        line = str::strtrim(line);
        if (line.empty())
            continue;

        if (_isValidKey(line))
            currentKey = line.substr(0, line.size() - 1);
        else if (!currentKey.empty())
            _processValue(line, currentKey, grammar);
    }

    configFile.close();
    return (grammar);
}

//===[ Method: isValidKey ]=====================================================
bool Dictionary::_isValidKey(const string& line)
{
    size_t len = line.size();
    if (len > 1 && line[len - 1] == ':')
        return (true);
    return (false);
}

//===[ Method: processValue ]===================================================
void Dictionary::_processValue(const string& aLine,
                               const string& aKey,
                               GrammarMap& aGrammar)
{
    StringPair KeyValue = str::lineToPair(aLine, '=');
    if (KeyValue.first.empty())
        return;
    KeyValue.second = str::toLower(KeyValue.second);
    DirType terminalType = KeyValue.second == "list" ? List :
        (KeyValue.second == "block" ? Block : Inline);
    aGrammar[aKey].push_back(std::make_pair(KeyValue.first, terminalType));
}
