#include "ConfigurationReader.h"
#include "ConfigurationFolder.h"
#include "ConfigurationString.h"
#include "ConfigurationNumber.h"
#include <ctype.h>

void ConfigurationReader::skipWhitespace(DataInputStream* node)
{

    //Create a buffer character to use while skipping any whitespace
    int bufferChar = 0;

    while (true) //Enter a loop
    {

        //Read the next character from the file
        if (node->read(&bufferChar, 1) == false)
        {
            return;
        }

        //If the character isn't a space (isspace returns 0)
        if (isspace(bufferChar) == 0)
        {

            //Rewind 1 byte
            node->rewind(1);

            return;
        }


    }

}

//Keep reading until file ends or a " is found and add each character read onto the output buffer. If end of the file return fail
bool ConfigurationReader::getString(DataInputStream* stream, char* outputBuffer, unsigned int mlen)
{
    //The buffer character used in the reading of the string
    int bufferChar = 0;

    //The pointer to the current index in the output buffer
    unsigned int cPtr = 0;

    //Start looping
    while (true)
    {
        //If unable to get the next character
        if (stream->read(&bufferChar, 1) == false)
        {

            //Exit with an error
            printf("Error end of file before string ends\n");
            return false;

        }

        //If its a terminating character append a null terminator to the end of the string and exit with a success value
        if ((bufferChar == L'"'))
        {
            //Check not outside the output bounds
            if (cPtr >= mlen) return false;

            //Append the null terminator
            outputBuffer[cPtr] = L'\0';

            //Return true
            return true;

        }

        //Check not outside the bounds
        if (cPtr >= mlen) return false;

        //Add the read character to the current point on the output buffer
        outputBuffer[cPtr] = bufferChar;

        //Increment the current pointer
        cPtr++;

    }

    //Should never get here
    return false;

}

//WORDS IN CONFIGURATION, Any space seperated value E.g. dog, anything in a set of quotes is interpretted as a single word IE \"Hello World\" and the symbols {, } and ; are interpretted as single words
bool ConfigurationReader::getNextWord(DataInputStream* instream, char* outputBuffer, unsigned int mlen)
{

    //Buffer character used when reading the next word
    int bufferChar = 0;

    //Pointer to the current location in the outputBuffer
    unsigned int cPtr = 0;

    //Skip all the whitespace before the next word
    skipWhitespace(instream);

    //Enter a loop
    while (true)
    {

        //If the read fails or the character is a space then exit appending a null terminator to the string

        bool resultOfRead = instream->read(&bufferChar, 1);

        if ( (resultOfRead == false) || (isspace(bufferChar) != 0))
        {

            //Check we can still add to the buffer
            if (cPtr >= mlen) return false;

            //Append a null
            outputBuffer[cPtr] = L'\0';

            //Return success
            return true;

        }

        //Check if its a " and its the start of a word (No characters beforehand, If its not it will be picked up later and the world will unget the character then kick out (As its the start of the next word probably, just a badly written file)
        if (bufferChar == L'"')
        {
            if (cPtr == 0)
                //If at the start, pass to a readString
                return getString(instream, outputBuffer, mlen);
        }

        //These tests come after adding the character to the buffer (AS they wouldn't be added to the word otherwise)
        if ((bufferChar == L'{') || (bufferChar == L'}') || (bufferChar == L';') || (bufferChar == L'"'))
        {

            //If this isn't the first character read then its a badly written Cfg file with no whitespace between the words. unget the character and exit out
            if (cPtr != 0)
            {

                //Unget the character so the next getNextWord functions correctly (Push it back onto the file stream)
                instream->rewind(1);

                //Check we can still add to the buffer
                if (cPtr >= mlen) return false;

                //Append a null terminator onto the end
                outputBuffer[cPtr] = L'\0';

                //Return success
                return true;

            }
            //If it is the first character however, then it is the entirety of the word and thus just append the character plus a null terminator to the output buffer and exit out
            else
            {

                //Check we can still add to the buffer (mlen - 1 as adding two characters)
                if (cPtr >= mlen - 1) return false;

                //Append the character
                outputBuffer[cPtr] = bufferChar;

                //Append a null terminator
                outputBuffer[cPtr + 1] = L'\0';

                //Return success
                return true;
            }

        }

        //If its none of those things, then just stick the character onto the end of the output buffer and restart the loop!

        //Check can still fit more on the buffer
        if (cPtr >= mlen)
        {
            return false;
        }

        //Append it onto the end of the output buffer
        outputBuffer[cPtr] = bufferChar;

        //Increment the current pointer
        cPtr++;
    }

    //Shouldn't ever reach here!
    return false;
}

ConfigurationNode* ConfigurationReader::readFolderNode(DataInputStream* node)
{

    //Create a buffer for the name of the folder (And the subsequent read to check its formatted properly with a {
    char nameBuffer[512] = "";

    //Attempt to read the name
    if (getNextWord(node, nameBuffer, 512) == false)
    {
        printf("Unable to read folder name\n");
        return 0;
    }

    //Create the new folder with the name that was read
    ConfigurationFolder* newFolder = new ConfigurationFolder(nameBuffer);

    //Try and read the {
    if (getNextWord(node, nameBuffer, 512) == false)
    {
        printf("Error in config file\n");

        delete newFolder;

        return 0;
    }

    //Check its a { otherwise return a error
    if (strcmp("{", nameBuffer) != 0)
    {
        printf("Error, expected { after name in configuration file");
        delete newFolder;
        return 0;
    }

    //Pointer to the next childNode when reading the children
    ConfigurationNode* childNode = 0;

    //Here's a little bit of trickery. Loop calling getNextNode until it returns 0. this works because the folder is ended with } which will cause getNextNode to return 0 when it reads it as the next word.
    while (1)
    {

        //Attempt to get the next node
        childNode = getNextNode(node);

        //If its 0, theres either been a error or a } has been found. either way break out and return
        if (childNode == 0)
        {
            break;
        }
        else
        {

            //Otherwise, just add it as a child node
            newFolder->addChild(childNode);

        }

    }

    //Return the generated folder
    return newFolder;
}

ConfigurationNode* ConfigurationReader::readStringNode(DataInputStream* node)
{
    char nameBuffer[512] = "";
    char stringBuffer[2048] = "";

    //Attempt to read the string name
    if (getNextWord(node, nameBuffer, 512) == false)
    {
        printf("Unable to read string name\n");
        return 0;
    }

    //Read the next word into the string buffer, if it fails return 0
    if (getNextWord(node, stringBuffer, 2028) == false)
    {

        printf("Bad configuration file\n");
        return 0;

    }

    //check its a =, it should be a equals, if not have a bit of a shout and return 0
    if (strcmp(stringBuffer, "=") != 0)
    {

        printf("Expecting equals after name in string definition.\n");
        return 0;

    }

    //Attempt to read the string name, (Comes after the equals, duh) if it fails return 0
    if (getNextWord(node, stringBuffer, 2048) == false)
    {

        printf("Unable to read string name\n");
        return 0;

    }

    //Create the actual node, a new string
    ConfigurationString* newString = new ConfigurationString(nameBuffer, stringBuffer);

    //Attempt to read the next word, if it fails then theres no ; so delete the nice new node we just created and return nil after complaining
    if (getNextWord(node, stringBuffer, 2048) == false)
    {

        printf("Bad cfg file\n");

        delete newString;

        return 0;

    }

    //Check its a ;, if it isn't complain a bit then delete the nice new node and return 0;
    if (strcmp(stringBuffer, ";") != 0)
    {

        printf("Expecting ; terminator after string value recieved %s instead\n", nameBuffer);
        delete newString;
        return 0;

    }

    //Finally if nothing goes wrong return the string
    return newString;
}

ConfigurationNode* ConfigurationReader::readNumberNode(DataInputStream* node)
{

    //Create a value float and a buffer for the node name
    char nameBuffer[512] = "";

    //Small because once you've got 256 digits in a float I doubt its gohna work anyways
    char numberBuffer[256] = "";

    float value = 0;

    //If unable to read the number node's name exit out with a unhappy face
    if (getNextWord(node, nameBuffer, 512) == false)
    {
        printf("Unable to read number name\n");
        return 0;
    }

    //If unable to read the next word (Should be a =) exit out with a grumble
    if (getNextWord(node, numberBuffer, 256) == false)
    {

        printf("Bad configuration file\n");

        return 0;

    }

    //If the next word wasn't in fact a = then have a even bigger rant and exit
    if (strcmp(numberBuffer, "=") != 0)
    {

        printf("Expecting equals after name in number definition.\n");
        return 0;

    }

    //Attempt to read the actual number as a string
    if (getNextWord(node, numberBuffer, 256) == false)
    {
        printf("Unable to read number name\n");
    }

    //Attempt to scan it from the string using swscanf
    if (sscanf(numberBuffer, "%f", &value) != 1)
    {
        printf("Unable to evaluate to a number value\n");
        return 0;
    }

    //Create the new number using the namebuffer and the value
    ConfigurationNumber* newNumber = new ConfigurationNumber(nameBuffer, value);

    //If unable to read the next word (Should be a ;) throw a hissy fit and return 0
    if (getNextWord(node, nameBuffer, 256) == false)
    {

        printf("Bad cfg file\n");

        delete newNumber;

        return 0;

    }

    //If the next word isn't a ; then complain and return 0
    if (strcmp(nameBuffer, ";") != 0)
    {

        printf("Expecting ; terminator after number value\n");

        delete newNumber;

        return 0;

    }

    //If nothing went wrong then return the new number!
    return newNumber;

}

ConfigurationNode* ConfigurationReader::getNextNode(DataInputStream* node)
{
    //Create a buffer to use when reading the words from the file
    char cfgBuffer[1024] = "";

    //If the next word fails to read, just return null (Failure)
    if (getNextWord(node, cfgBuffer, 1024) == false)
    {
        printf("ConfigurationReader: Unable to read next word\n");
        return 0;
    }

    //If its a folder this is how its parsedCiao
    if (strcmp(cfgBuffer, "folder") == 0)
    {
        printf("ConfigurationReader: Attempting to read a folder node\n");
        //Pass it on to the specific read function
        return readFolderNode(node);

    }
    //If its a string this is how its parsed
    else if (strcmp(cfgBuffer, "string") == 0)
    {
        printf("ConfigurationReader: Attempting to read a string node\n");

        //Pass it on to the specific read function
        return readStringNode(node);
    }
    else if (strcmp(cfgBuffer, "number") == 0)
    {
        printf("ConfigurationReader: Attempting to read a number\n");

        //Pass it on to the specific number read function
        return readNumberNode(node);
    }
    else
    {
        printf("ConfigurationReader: Unknown node %s\n", cfgBuffer);
    }

    return 0;
}
