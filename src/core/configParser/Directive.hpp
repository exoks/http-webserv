/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Directive header file ]                                              #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-25                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef  __DIRECTIVE_HPP__
# define  __DIRECTIVE_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"
# include "Utils.hpp"

# include "Dictionary.hpp"

/*******************************************************************************
    * DirectivePart struct :
*******************************************************************************/ 
class	DirectivePart
{
public:
    typedef enum { Terminal = 0, NonTerminal=1 }    Type;

    DirectivePart(const string& aLine,
                  const string& aFile,
                  int aNline,
                  int aLevel);
    
    Type        mType;
    string      mFile;
    string      mKey;
    string      mRest;
    int         mLevel;
    int         mNline;
};

/*******************************************************************************
    * Directive Class :
*******************************************************************************/ 
class	Directive
{
public:
    class Exception;

    typedef mem::shared_ptr<Directive>              SharedPtr;
    typedef std::vector<SharedPtr>                  DirPtrVector;
    typedef std::vector<DirectivePart>              DirPartVect;
    typedef std::vector<DirectivePart>::iterator    DirPartVectIt;
    typedef std::map<string, DirPtrVector >         NonTerminalMap;
    typedef std::map<string, StringVector >         TerminalMap;

    Directive(const string& aHostName = "global");
    Directive(const string& aName, DirPartVectIt& aBegin, DirPartVectIt aEnd);
    ~Directive(void);

    void             copyMatchingAttributes(const SharedPtr httpDir);
    void             push(DirPartVectIt& aDirIter, DirPartVectIt aDirEnd);
    StringVector     getTerminal(const string& aDirectiveKey) const;
    DirPtrVector     getNonTerminal(const string& aDirectiveKey) const;

private:
    // Static members :
    static int          sCurrentLevel;

    // Members Variables:
    string              mHostName;
    NonTerminalMap      mNonTerminal;
    TerminalMap         mTerminal;

    // Private methods :
    void    _processTerminalDirective(DirPartVectIt& aDirectiveIter);
    void    _processNonTerminalDirective(DirPartVectIt& aDirectiveIter,
                                         DirPartVectIt aDirectiveEnd);
};

/*******************************************************************************
    * Exception Class :
*******************************************************************************/ 
class Directive::Exception : public std::exception
{
public:
    Exception(const string& aMessage, const string& aFile, int aNline);
    virtual ~Exception(void) throw();

    const char* what(void) const throw();
private:
    string     mMessage;
};

#endif /* __DIRECTIVE_HPP__ */
