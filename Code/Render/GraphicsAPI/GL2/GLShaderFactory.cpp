#include "GLShaderFactory.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "GLShader.h"

int GLShaderFactory::readFragmentShaderSourceFromFile(const char* Filename, char *readBuffer, unsigned int maxReadLength)
{
    printf("Reading fragment shader source\n");

    FILE* fin = fopen(Filename, "r");

    if (fin == NULL) return -1;

    unsigned int cPtrPosition = 0;

    while (true)
    {
        int readChar = fgetc(fin);

        if (readChar == EOF) break;

        readBuffer[cPtrPosition] = readChar;
        cPtrPosition++;
    }

    readBuffer[cPtrPosition] = '\0';

    return cPtrPosition;
}

int GLShaderFactory::readVertexShaderSourceFromFile(const char* Filename, char *readBuffer, unsigned int maxReadLength)
{

    printf("Reading vertex shader source\n");

    FILE* fin = fopen(Filename, "r");

    if (fin == NULL) return false;

    unsigned int cPtrPosition = 0;

    while (true)
    {
        int readChar = fgetc(fin);

        if (readChar == EOF) break;

        readBuffer[cPtrPosition] = readChar;
        cPtrPosition++;
    }

    readBuffer[cPtrPosition] = '\0';

    return cPtrPosition;
}

void skipWhitespace(const char* inputData, size_t* cPtr)
{
    while (true)
    {

        if ( inputData[*cPtr] == '\0')
        {

            return;

        }
        else if ( isspace( inputData[*cPtr]) == false)
        {

            return;

        }
        else
        {

            *cPtr = *cPtr + 1;

        }

    }

}

bool nextWord(const char* inputData, size_t* cPtr, char* wordBuffer)
{
    skipWhitespace(inputData, cPtr);

    inputData = inputData + *cPtr;

    if (*inputData == '"')
    {

        //TODO: QUOTES MAY BE USED IN SHADERS AT SOME POINT, NEED TO MAKE SURE ITS THE END SHADER "; found! Room for improvement here

        //Increment 1 and discard the opening "
        *cPtr += 1;
        inputData = inputData + 1;

        //Find the end of the quote
        const char* end = strstr(inputData, "\";");

        size_t difference = end - inputData;

        strncpy(wordBuffer, inputData, difference);
        wordBuffer[difference] = '\0';

        //Add two onto the difference for the ";
        *cPtr += difference + 2;

        return true;
    }
    else
    {

        const char* nextSpace = strchr(inputData, ' ');

        if (nextSpace == 0)
        {

            return false;

        }
        else
        {

            size_t difference = nextSpace - inputData;

            strncpy(wordBuffer, inputData, difference);
            wordBuffer[difference] = '\0';

            *cPtr += difference + 1;

            return true;
        }

    }

}

Shader* GLShaderFactory::loadShader(File* fileNode)
{
    if (fileNode == 0) return 0;
    if (fileNode->getType() != fileId) return 0;

    DataInputStream* strm = fileNode->createInputStream();
    if (strm == 0) return 0;

    //If the open fails
    if (strm->open(false) == false)
    {
        //Delete the stream and return 0
        delete strm;
        return 0;
    }

    char* dataBuffer = new char[ fileNode->length() ];

    if (strm->read(dataBuffer, fileNode->length()) == false)
    {

        delete[] dataBuffer;
        delete strm;

        return 0;
    }

    size_t currentPtr = 0;
    char wordBuffer[2048];

    char* apiBuffer = 0;
    char* vertexBuffer = 0;
    char* fragmentBuffer = 0;

    while ( nextWord(dataBuffer, &currentPtr, wordBuffer) && currentPtr < fileNode->length())
    {

        if (strcmp(wordBuffer, "api") == 0)
        {

            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Unexpected end of file\n");
                break;
            }

            //Test =
            if (strcmp(wordBuffer, "=") != 0)
            {
                printf("ERR Expecting = recieved %s\n", wordBuffer);
                break;
            }

            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Read apiBuffer\n");
                break;
            }

            apiBuffer = new char[strlen(wordBuffer) + 1];
            strncpy(apiBuffer, wordBuffer, strlen(wordBuffer) + 1);

        }
        else if (strcmp(wordBuffer, "vertex") == 0)
        {

            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Unexpected end of file\n");
                break;
            }

            //Test =
            if (strcmp(wordBuffer, "=") != 0)
            {
                printf("ERR Expecting = recieved %s\n", wordBuffer);
                break;
            }


            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Read apiBuffer\n");
                break;
            }

            vertexBuffer = new char[strlen(wordBuffer) + 1];
            strncpy(vertexBuffer, wordBuffer, strlen(wordBuffer) + 1);

        }
        else if (strcmp(wordBuffer, "fragment") == 0)
        {

            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Unexpected end of file\n");
                break;
            }

            //Test =
            if (strcmp(wordBuffer, "=") != 0)
            {
                printf("ERR Expecting = recieved %s\n", wordBuffer);
                break;
            }

            if ((nextWord(dataBuffer, &currentPtr, wordBuffer) == false) && ((currentPtr < fileNode->length()) == false))
            {
                printf("ERR Read apiBuffer\n");
                break;
            }

            fragmentBuffer = new char[strlen(wordBuffer) + 1];
            strncpy(fragmentBuffer, wordBuffer, strlen(wordBuffer) + 1);

        }
        else
        {
            printf("ERROR: Word %s in the wrong place\n", wordBuffer);
        }

    }

    delete[] dataBuffer;


    printf("Graphics API: %s\nVertex Shader: %s\nFragment Shader %s\n",apiBuffer, vertexBuffer, fragmentBuffer);

    if (vertexBuffer == 0 || apiBuffer == 0 || fragmentBuffer == 0)
    {

        if (vertexBuffer != 0)
            delete[] vertexBuffer;

        if (apiBuffer != 0)
            delete[] apiBuffer;

        if (fragmentBuffer != 0)
            delete[] fragmentBuffer;

        return 0;
    }

    GLShader* returnShader = 0;

    //TODO: API tests here

    GLShader* newShader = new GLShader();

    if (newShader->loadShader(vertexBuffer, fragmentBuffer) == false)
    {

        delete newShader;

    }
    else
    {

        returnShader = newShader;

    }

    if (vertexBuffer != 0)
        delete[] vertexBuffer;

    if (apiBuffer != 0)
        delete[] apiBuffer;

    if (fragmentBuffer != 0)
        delete[] fragmentBuffer;

    return returnShader;
}
