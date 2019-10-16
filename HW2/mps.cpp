#include <dirent.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>


int main()
{
    DIR *sDir;
    struct dirent* sFile;
    sDir = opendir("/proc");
    const std::regex sRegex("[0-9]+");
    if (!sDir)
        return 1;

    while (sFile = readdir(sDir))
    {
        if (!std::regex_match(sFile->d_name, sRegex))
            continue;
        std::string sStr = std::string("/proc/") + sFile->d_name + "/task/";
        DIR* sTmpDir = opendir(sStr.c_str());
        struct dirent* sTmpFile;
        if (sTmpDir)
        {
            while (sTmpFile = readdir(sTmpDir))
            {
                if (!std::regex_match(sTmpFile->d_name, sRegex))
                    continue;
                std::ifstream sInput(sStr + sTmpFile->d_name + "/cmdline");
                std::string sCmd( (std::istreambuf_iterator<char>(sInput) ),
                       (std::istreambuf_iterator<char>()    ) );
                std::cout << sTmpFile->d_name << '\t' << sCmd << std::endl; 
            }
        }
    }
    closedir(sDir);
    return 0;
}
