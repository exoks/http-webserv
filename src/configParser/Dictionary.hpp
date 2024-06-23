/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Dictionary header file ]                                             #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-21                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __DICTIONARY_HPP__
# define   __DICTIONARY_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "ParserUtils.hpp"

# include <iostream>
# include <fstream>
# include <cstdlib>
# include <string>
# include <vector>
# include <map>

/*******************************************************************************
    * Dictionary Static Class :
*******************************************************************************/
enum  DirType    { Simple = 0, List, Complex, Invalid };

#define CONFIG_FILE_PATH  "src/configParser/config/parser.conf"

class Dictionary
{
public:
    typedef std::string                         String;
    typedef std::pair<String, DirType>          PairNameType;
    typedef std::vector<PairNameType>           VectorNameType;
    typedef std::map<String, VectorNameType>    MapGrammar;

    static DirType  find(const std::string& aDir, const std::string& aSubDir);

private:
    static MapGrammar sGrammar;

    Dictionary(void);

    static MapGrammar _initializeGrammar(const std::string aConfigFilePath);
    static void _processValue(const std::string&aLine, const std::string& aKey,
                                                          MapGrammar& aGrammar);
    static bool _isValidKey(const std::string& line);
};

#endif /* __DICTIONARY_HPP__ */
