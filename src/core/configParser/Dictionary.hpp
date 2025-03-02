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
# include "typedefs.hpp"
# include "Utils.hpp"

# include <iostream>
# include <fstream>

/*******************************************************************************
    * defines :
*******************************************************************************/
#define CONFIG_FILE_PATH  "src/core/configParser/Dictionary.conf"

/*******************************************************************************
    * Dictionary Static Class :
*******************************************************************************/

enum DirType { Inline = 0, List, Block, Invalid };

class Dictionary
{
public:
    typedef std::pair<string, DirType>                TerminalDirPair;
    typedef std::vector<TerminalDirPair>              NonTerminalDirVector;
    typedef std::map<string, NonTerminalDirVector>    GrammarMap;

    static DirType  find(const string& aDir, const string& aSubDir);

private:
    static GrammarMap sGrammar;

    Dictionary(void);

    static GrammarMap   _initializeGrammar(const string aFilePath);
    static bool         _isValidKey(const string& aLine);
    static void         _processValue(const string& aLine, const string& aKey,
                                      GrammarMap& aGrammar);
};

#endif /* __DICTIONARY_HPP__ */
