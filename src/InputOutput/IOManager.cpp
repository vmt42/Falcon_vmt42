#include "IOManager.h"
#include <fstream>
namespace Falcon
{
    bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail())
        {
            perror(filePath.c_str());
            return false;
        }

        // Seek to the end
        file.seekg(0, std::ios::end);

        // Get file size
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reduce the file by header size
        fileSize -= file.tellg();

        buffer.resize((unsigned long) fileSize);
        file.read((char *) &(buffer[0]), fileSize);
        file.close();
        return true;

    }

    bool IOManager::readFileToBuffer(std::string filePath, std::string &buffer)
    {

        std::ifstream file(filePath, std::ios::binary);
        if (file.fail())
        {
            perror(filePath.c_str());
            return false;
        }

        // Seek to the end
        file.seekg(0, std::ios::end);

        // Get file size
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reduce the file by header size
        fileSize -= file.tellg();

        buffer.resize((unsigned long) fileSize);
        file.read((char*) &(buffer[0]), fileSize);
        file.close();
        return true;
    }
}