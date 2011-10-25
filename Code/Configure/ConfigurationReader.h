#ifndef _CONFIGURATION_READER_DEF_H_
#define _CONFIGURATION_READER_DEF_H_
#include "ConfigurationNode.h"
#include <Filesystem/File.h>

class ConfigurationReader
{
    private:

        static void skipWhitespace(DataInputStream* node);
        static bool getString(DataInputStream* node, char* outputBuffer, unsigned int mlen);
        static bool getNextWord(DataInputStream* node, char* OutputBuffer, unsigned int mlen);

        static ConfigurationNode* readFolderNode(DataInputStream* node);
        static ConfigurationNode* readStringNode(DataInputStream* node);
        static ConfigurationNode* readNumberNode(DataInputStream* node);

    public:

        static ConfigurationNode* getNextNode(DataInputStream* node);

};

#endif //_CONFIGURATION_READER_DEF_H_
