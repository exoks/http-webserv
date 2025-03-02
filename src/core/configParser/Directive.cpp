/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Directive Implementation file ]                                      #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-25                                                   #
#                                                                              #
** ************************************************************************* **/

#include "Directive.hpp"

/*******************************************************************************
    * Static Variables :
*******************************************************************************/
int Directive::sCurrentLevel = 0;

/*******************************************************************************
    * Construction:
*******************************************************************************/

//===[ Constructor: DirectivePart ]=============================================
DirectivePart::DirectivePart(const string& aLine,
                             const string& aFile,
                             int aLineNumber,
                             int aLevel)
{
    size_t colonPos = str::strtrim(aLine).find(':');
    if (colonPos == 0 || colonPos == string::npos) {
        throw (Directive::Exception("Syntax_Error",
                                    aFile,
                                    aLineNumber));
    }
    StringPair	keyValue = str::lineToPair(aLine, ':');
    mType   = keyValue.second.empty() ? NonTerminal : Terminal;
    mFile   = aFile;
    mKey    = keyValue.first;
    mRest   = keyValue.second;
    mNline  = aLineNumber;
    mLevel  = aLevel;
}

//===[ Constructor: Directive Default ] ========================================
Directive::Directive(const string& aHostName) : mHostName(aHostName) {}

//===[ Constructor: Directive ] ================================================
Directive::Directive(const string& aName,
                     DirPartVectIt& aDirectiveIter,
                     DirPartVectIt aDirectiveEnd)
    : mHostName(aName)
{
    while (aDirectiveIter != aDirectiveEnd || sCurrentLevel != 0)
    {	
        if (aDirectiveIter == aDirectiveEnd
            || aDirectiveIter->mLevel < sCurrentLevel)
            return ;
        if (aDirectiveIter->mLevel != sCurrentLevel) {
            throw (Directive::Exception("Line_Not_Aligned",
                                        aDirectiveIter->mFile,
                                        aDirectiveIter->mNline));
        }
        if (aDirectiveIter->mType == DirectivePart::NonTerminal)
            _processNonTerminalDirective(aDirectiveIter, aDirectiveEnd);
        else if (aDirectiveIter->mType == DirectivePart::Terminal)
            _processTerminalDirective(aDirectiveIter);
    }
}

//===[ Destructor: ] ===========================================================
Directive::~Directive(void){}

/*******************************************************************************
    * Public methods :
*******************************************************************************/

//===[ Method: create & push to current directive ]=============================
void Directive::push(DirPartVectIt &aDirectiveIter, DirPartVectIt aDirectiveEnd)
{
    if (aDirectiveIter->mType == DirectivePart::NonTerminal)
        _processNonTerminalDirective(aDirectiveIter, aDirectiveEnd);
    else if (aDirectiveIter->mType == DirectivePart::Terminal)
        _processTerminalDirective(aDirectiveIter);
}

//===[ Method: get terminal directive ]========================================
StringVector
Directive::getTerminal(const string& directiveKey) const
{
    std::map<string, StringVector >::const_iterator it;

    it = mTerminal.find(directiveKey);
    if (it == mTerminal.end())
        return (std::vector<string>());
    return (it->second);
}

//===[ Method: get non terminal directive ]====================================
Directive::DirPtrVector
Directive::getNonTerminal(const string& directiveKey) const
{
    std::map<string, DirPtrVector >::const_iterator it;
    it = mNonTerminal.find(directiveKey);
    if (it == mNonTerminal.end())
        return (DirPtrVector());
    return (it->second);
}

//===[ Method: copy matching attributes ]=======================================
void Directive::copyMatchingAttributes(Directive::SharedPtr httpDir)
{
    NonTerminalMap::iterator nonTermIt = httpDir->mNonTerminal.begin();
    for (;nonTermIt != httpDir->mNonTerminal.end(); ++nonTermIt)
    {
        if (Dictionary::find(mHostName, nonTermIt->first) == Invalid)
            continue ;
        if (mNonTerminal.find(nonTermIt->first) != mNonTerminal.end())
            continue ;
        mNonTerminal[nonTermIt->first] = nonTermIt->second;
    }

    TerminalMap::iterator termIt = httpDir->mTerminal.begin();
    for (;termIt != httpDir->mTerminal.end(); ++termIt)
    {
        if (Dictionary::find(mHostName, termIt->first) == Invalid)
            continue ;
        if (mTerminal.find(termIt->first) != mTerminal.end())
            continue ;
        mTerminal[termIt->first] = termIt->second;
    }
}

/*******************************************************************************
    * Private methods :
*******************************************************************************/

//===[ Method: process a non terminal directive ]==============================
void Directive::_processNonTerminalDirective(DirPartVectIt& aDirectiveIter,
                                             DirPartVectIt aDirectiveEnd)
{
    ++sCurrentLevel;
    const string& directiveKey = aDirectiveIter->mKey;

    if (Dictionary::find(mHostName, directiveKey) != Block)
    {
        throw Directive::Exception("Invalid_Directive",
                                   aDirectiveIter->mFile,
                                   aDirectiveIter->mNline);
    }
    if ((aDirectiveIter + 1) == aDirectiveEnd ||
        (aDirectiveIter + 1)->mLevel < sCurrentLevel)
    {
        throw Directive::Exception("Empty_Directive",
                                  aDirectiveIter->mFile,
                                  aDirectiveIter->mNline);
    }
    mNonTerminal[directiveKey].push_back(
             Directive::SharedPtr(new Directive(directiveKey, ++aDirectiveIter,
                aDirectiveEnd))
            );
    --sCurrentLevel;
}

//===[ _processNonTerminalDirective ] =========================================
void Directive::_processTerminalDirective(DirPartVectIt& aDirectiveIter)
{
    string directiveKey = aDirectiveIter->mKey;
    DirType type = Dictionary::find(mHostName, directiveKey);

    if (type != Inline && type != List)
    {
        throw Directive::Exception("Invalid_Directive",
                                    aDirectiveIter->mFile,
                                    aDirectiveIter->mNline);
    }

    if (type == Inline && mTerminal.count(directiveKey) != 0)
    {
        throw Directive::Exception("Duplicate_Simple_Directive",
                                    aDirectiveIter->mFile,
                                    aDirectiveIter->mNline);
    }

    if (type == List) 
    {
        std::vector<string> values = str::split(aDirectiveIter->mRest, ',');
        mTerminal[directiveKey].insert(mTerminal[directiveKey].end(),
                                       values.begin(),
                                       values.end());
    }
    else 
    {
        mTerminal[directiveKey].push_back(aDirectiveIter->mRest);
    }

    ++aDirectiveIter;
}

/*******************************************************************************
    * Directive Exception :
*******************************************************************************/

//===[ Constructor: Directive::Exception ]======================================
Directive::Exception::Exception(const string& aMessage,
                                const string& aFile,
                                int aNline)
{
    char mNline[16];
    sprintf(mNline, "%d", aNline);
    mMessage =  "\e[1m\e[4;31m" + aMessage 	+ ": "
                "\e[0m\e[4;3m"	+ aFile		+ 
                " ["			+ mNline 	+ "]\n";
}

//===[ Destructor: Directive::Exception ]=======================================
Directive::Exception::~Exception(void) throw(){}

//===[ What: Directive::Exception ]=============================================
const char* Directive::Exception::what() const throw()
{
    return (mMessage.c_str());
}
