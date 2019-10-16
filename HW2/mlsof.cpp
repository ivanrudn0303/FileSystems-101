#include <dirent.h>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>


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
        std::string sStr = std::string("/proc/") + sFile->d_name + "/fd/";
        DIR* sTmpDir = opendir(sStr.c_str());
        struct dirent* sTmpFile;
        if (sTmpDir)
        {
            while (sTmpFile = readdir(sTmpDir))
            {
                if (!std::regex_match(sTmpFile->d_name, sRegex))
                    continue;
                char buf[256]{};
                std::string sPath = sStr + sTmpFile->d_name;
                if (-1 != readlink(sPath.c_str(), buf, sizeof(buf)))
                    std::cout << sFile->d_name << '\t' << buf << std::endl; 
            }
        }
    }
    closedir(sDir);
    return 0;
}
