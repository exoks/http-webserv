//  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣦⣴⣶⣾⣿⣶⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀                                              
//  ⠀⠀⠀⠀⠀⠀⠀⢠⡶⠻⠛⠟⠋⠉⠀⠈⠤⠴⠶⠶⢾⣿⣿⣿⣷⣦⠄⠀⠀⠀          𓐓  Utils.cpp 𓐔           
//  ⠀⠀⠀⠀⠀⢀⠔⠋⠀⠀⠤⠒⠒⢲⠀⠀⠀⢀⣠⣤⣤⣬⣽⣿⣿⣿⣷⣄⠀⠀                                              
//  ⠀⠀⠀⣀⣎⢤⣶⣾⠅⠀⠀⢀⡤⠏⠀⠀⠀⠠⣄⣈⡙⠻⢿⣿⣿⣿⣿⣿⣦⠀  Student: oezzaou <oezzaou@student.1337.ma> 
//  ⢀⠔⠉⠀⠊⠿⠿⣿⠂⠠⠢⣤⠤⣤⣼⣿⣶⣶⣤⣝⣻⣷⣦⣍⡻⣿⣿⣿⣿⡀                                              
//  ⢾⣾⣆⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇                                              
//  ⠀⠈⢋⢹⠋⠉⠙⢦⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇     Created: 2024/05/15 11:58:00 by oezzaou
//  ⠀⠀⠀⠑⠀⠀⠀⠈⡇⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇     Updated: 2024/05/21 14:05:50 by oezzaou
//  ⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢀⣾⣿⣿⠿⠟⠛⠋⠛⢿⣿⣿⠻⣿⣿⣿⣿⡿⠀                                             
//  ⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⢠⣿⣟⣭⣤⣶⣦⣄⡀⠀⠀⠈⠻⠀⠘⣿⣿⣿⠇⠀                                            
//  ⠀⠀⠀⠀⠀⠱⠤⠊⠀⢀⣿⡿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠘⣿⠏⠀⠀                             𓆩♕𓆪      
//  ⠀⠀⠀⠀⠀⡄⠀⠀⠀⠘⢧⡀⠀⠀⠸⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠐⠋⠀⠀⠀                        𓄂 oussama ezzaou𓆃
//  ⠀⠀⠀⠀⠀⠘⠄⣀⡀⠸⠓⠀⠀⠀⠠⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                              

#include "Utils.hpp"

/*******************************************************************************
    * String Utils :
*******************************************************************************/

//===[ strtrim : ]==============================================================
string str::strtrim(const string& input)
{
    size_t start = input.find_first_not_of(' ');
    size_t end   = input.find_last_not_of(' ');

    if (string::npos == start)
    {
        return (EmptyString);
    }
    return (input.substr(start, end - start + 1));
}

//===[ lineToPair : ]===========================================================
StringPair str::lineToPair(const string& line, const char sep)
{
    StringPair          pair;
    std::stringstream   ss(line);
    getline(ss, pair.first, sep);
    getline(ss, pair.second);
    return (StringPair(strtrim(pair.first), strtrim(pair.second)));
}

//===[ split : ]================================================================
std::vector<string> str::split(const string str, const char sep)
{
    std::vector<string>    arr;
    std::stringstream           ss(str);
    string                 buff;

    while (getline(ss, buff, sep))
    {
        if (buff.empty() == false)
        {
            arr.push_back(buff);
        }
    }
    return (arr);
}

//===[ stirngTolower : ]========================================================
string str::toLower(const string &str)
{
    string result = str;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return (result);
}

//===[ address to string : ]====================================================
string str::addrtoString(const in_addr_t& addr, const socklen_t& addrLen)
{
    char* buffer = new char[addrLen];
    unsigned char* ip = (unsigned char*)&addr;
    
    snprintf(buffer, addrLen, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    string result(buffer);
    delete[] buffer;

    return (result);
}

//===[ replace : ]==============================================================
string& str::replace(string& str,
                          const string& from,
                          const string& to)
{
    if (from.empty())
        return (str);
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.size(), to);
        start_pos += to.size();
    }
    return (str);
}

//===[ strToInt : ]=============================================================
int str::strToInt(const string& str)
{
    if (str.empty() == true)
        return (0);

    for (size_t index = 0; index < str.size(); ++index)
    {
        if (std::isdigit(str[index]) == false)
            return (0);
    }

    return (std::atoi(str.c_str()));
}

//===[ toArray : ]============================================================
char**  str::to_Array(const StringVector& strVector)
{
    char** arr = new char*[strVector.size() + 1];
    for (size_t index = 0; index < strVector.size(); ++index)
    {
        arr[index] = new char[strVector[index].size() + 1];
        ::strcpy(arr[index], strVector[index].c_str());
    }
    arr[strVector.size()] = NULL;
    return (arr);
}


/*******************************************************************************
    * Logger Utils :
*******************************************************************************/

//=== [ print error in file ] ==================================================
void	Logger::log(const string& level, const string& message, int fd)
{
    string time = getcurrentTime("%Y/%m/%d %H:%M:%S");

    fd = (fd >= 0) ? fd : 1;
    string logMsg = time + " - [" + level + "] " + message + "\n";
    ::write(fd, logMsg.c_str(), logMsg.size());
}

//=== [ get current time ] =====================================================
string		Logger::getcurrentTime(const char* format)
{
    time_t  now      = time(0);
    tm*     timeinfo = localtime(&now);
    char    timestamp[50];

    strftime(timestamp, sizeof(timestamp), format, timeinfo);
    return (timestamp);
}

/*******************************************************************************
    * File Utils :
*******************************************************************************/

//=== [ file exists ] ==========================================================
bool      file::fileExists(const string& path)
{
    return (access(path.c_str(), F_OK) == 0);
}

//=== [ generate temp file name ] ==============================================
string    file::generateTempFileName(const string& dir)
{
    std::srand(static_cast<u_int>(std::time(0)));
    std::string tempFileName;

    do {
		tempFileName.clear();
        tempFileName = dir + "/";
		tempFileName += "tempfile_" + std::to_string(std::time(0));
        tempFileName += "_" + str::to_string(std::rand()) + ".tmp";
	} while (fileExists(tempFileName));

    return (tempFileName);
}

//=== [ create file ] ==========================================================
fd_t      file::createFile(const string& path)
{
    if (!fileExists(path) && errno != ENOENT)
        return (-1);
    return (::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666));
}

//=== [ get file size ] ========================================================
ssize_t    file::getFileSize(fd_t fd)
{
    struct stat st;

    if (fd == -1 || ::fstat(fd, &st) == -1) {
        return (-1);
    }
    return (st.st_size);
}

//=== [ close file ] ===========================================================
void      file::closeFile(fd_t fd)
{
    if (fd != -1) {
        ::close(fd);
    }
}

//=== [ remove file ] ==========================================================
void      file::removeFile(const string& path)
{
    std::remove(path.c_str());
}

//=== [ get extension ] ========================================================
std::string     file::getExtension(const string& path)
{
    std::size_t position = path.find_last_of('.');
    if (position == std::string::npos)
        return ("");
    return (path.substr(position + 1));
}

//=== [ is directory ] =========================================================
bool    file::isDirectory(const string& path)
{    
    struct stat st;
    if (path[path.length() - 1] == '/')
        return (true);
    if (stat(path.c_str(), &st) != 0)
        return (false);
    return (S_ISDIR(st.st_mode));
}

/*******************************************************************************
    * Socket Utils :
*******************************************************************************/

//=== [ create socket ] ========================================================
Handle sock::createSocket(int family, int type, int protocol)
{
    Handle socketHandle = -1;
    socketHandle = ::socket(family, type, protocol);
    if (socketHandle < 0)
        throw std::runtime_error("SOCKET: Failed to create socket");
    return (socketHandle);
}

//=== [ bind ] =================================================================
void    sock::bind(Handle socket, const string& host, const string& port)
{
    struct addrinfo     hints;
    struct addrinfo*    result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0)
    {
        throw std::runtime_error("SOCKET: Failed to resolve host/port");
    }

    if (bind(socket, result->ai_addr, result->ai_addrlen) == -1)
    {
        freeaddrinfo(result);
        throw (std::runtime_error("SOCKET: Failed to bind socket"));
    }

    freeaddrinfo(result);
}

//=== [ start listening ] ======================================================
void    sock::startListening(Handle socket, int backlog)
{
    if (::listen(socket, backlog) == -1)
    {
        throw std::runtime_error("SOCKET: Failed to start listening");
    }
}

//=== [ set reuse address ] ====================================================
void    sock::setReuseAddr(Handle& socket)
{
    int reuse = 1;
    if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR,
            &reuse, sizeof(reuse)) < 0)
    {
        throw std::runtime_error("SOCKET: setReuseAddr setsockopt() failed");
    }
}

//=== [ set non blocking ] =====================================================
void    sock::setNonBlocking(Handle& socket)
{
    if (::fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
    {
        throw std::runtime_error("SOCKET: setNonBlocking fcntl() failed");
    }
}

/*******************************************************************************
    * HTTP Utils
*******************************************************************************/

//=== [ tmp dir ] ==============================================================
std::string http::tmpDir(const std::string& dir)
{
    static string tmpDir = "/tmp";

    if (dir != "")
        tmpDir = dir;

    return (tmpDir);
}

//=== [ isValidHeader ] ========================================================
bool    http::isValidHeader(const char &c)
{
    if (!(::isalnum(c) || c == '-' || c == '_' ))
        return (false);
    return (true);
}

//=== [ is Valid Method : ] ====================================================
bool    http::isValidMethod(const string& method)
{
    if (method == "GET" || method == "POST" || method == "DELETE")
        return (true);
    return (false);
}

//=== [ is Valid Version : ] ===================================================
bool    http::isValidProtocol(const string& protocol)
{
    if (protocol == "HTTP/1.1")
        return (true);
    return (false);
}

//=== [ isSpecialChar ] ========================================================
bool    http::isSpecialChars(const char &c)
{
    if (!(::isalnum(c) || http::SpecialChars(c)))
        return (false);
    return (true);
}

//===[ httpDecoder: ]===========================================================
void    http::decoder(string &str)
{
	std::size_t percentSignPosition;
	while ((percentSignPosition = str.find('%')) != string::npos)
	{
		string hexadecimalValue = str.substr(percentSignPosition + 1, 2);
		char character = static_cast<char>(std::strtol(hexadecimalValue.c_str()
            ,NULL, 16));
		str.replace(percentSignPosition, 3, 1, character);				
	}
}

//===[ getRelativePath ] =======================================================
std::string http::getRelativePath(const string& reqUri, const string& locUri)
{
    if (locUri[locUri.length() - 1] == '/') 
    {
        std::string path = reqUri.substr(locUri.length());
        return (path);
    }
    else {
        std::string path = reqUri.substr(locUri.length());
        path.erase(0, path.find_first_of('/') + 1);
        return (path);
    }
}

//===[ isCgiPath ] =============================================================
bool    http::isCgiPath(const string& path, const StringVector& cgiExt)
{
    string fileExt = "." + file::getExtension(path);

    for (size_t index = 0; index < cgiExt.size() && !fileExt.empty(); ++index)
    {
        if (cgiExt[index] == fileExt)
            return (true);
    }
    return (false);
}
