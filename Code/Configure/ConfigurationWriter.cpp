#include "ConfigurationWriter.h"
#include "ConfigurationFolder.h"
#include "ConfigurationString.h"
#include "ConfigurationNumber.h"
#include <Filesystem/Utils.h>


void ConfigurationWriter::startNewLine(unsigned int depth, DataOutputStream* outputStream)
{
    FilesystemUtils::writeLineToStream(outputStream, "\n");

    for (unsigned int i = 0; i < depth; i++)
        FilesystemUtils::writeLineToStream(outputStream, "\n");

}

void ConfigurationWriter::outputNode(ConfigurationNode* Node, unsigned int depth, DataOutputStream* outStrm)
{
    char lineBuffer[1024];

    if (Node->getType() == configurationFolder)
    {

        //Start the writer with a newline at the correct depth (Or at least the specified depth)
        startNewLine(depth,outStrm);

        //Create the line folder "NAME" { and write it to the file
        sprintf(lineBuffer, "folder \"%s\" {", Node->getName());
        FilesystemUtils::writeLineToStream(outStrm, lineBuffer);

        //Loop through each child and write them in as children
        ConfigurationFolder* folder = static_cast<ConfigurationFolder*> (Node);

        //Get the number of children
        unsigned int folderLength = folder->numberOfChildren();

        //Loop through each node
        for (unsigned int i = 0; i < folderLength; i++)
        {
            //Output each node with a depth value of +1 (To make the file format nicely for the possible human reader)
            outputNode(folder->getChild(i), depth + 1, outStrm);
        }

        //Start a new line
        startNewLine(depth, outStrm);

        //Output the } to signify the end of the folder in the file
        FilesystemUtils::writeLineToStream(outStrm, "}");

    }
    else if (Node->getType() == configurationString)
    {

        //Start a new line
        startNewLine(depth, outStrm);

        //Get a pointer to a ConfigurationString* node by typecasting the node given
        ConfigurationString* string = static_cast<ConfigurationString*> (Node);

        //Write the string line using fprintf
        sprintf(lineBuffer, "string \"%s\" = \"%s\";", Node->getName(), string->getStringData());
        FilesystemUtils::writeLineToStream(outStrm, lineBuffer);


    }
    else if (Node->getType() == configurationNumber)
    {

        //Start a new line
        startNewLine(depth, outStrm);

        //Get a pointer to a ConfigurationNumber* node by typecasting the node given
        ConfigurationNumber* Number = static_cast<ConfigurationNumber*> (Node);

        //Write the number line using fprintf
        sprintf(lineBuffer, "number \"%s\" = %f;", Node->getName(), Number->getValue());
        FilesystemUtils::writeLineToStream(outStrm, lineBuffer);

    }
    else
    {
        //If the node isn't handled bitch a bit

        printf("NODE CANNOT BE WRITTEN (Yet)\n");
    }

}
