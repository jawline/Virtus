#include "ConfigurationInstance.h"
#include "ConfigurationReader.h"
#include "ConfigurationWriter.h"
#include "ConfigurationFolder.h"
#include <string.h>
#include <stdio.h>

ConfigurationInstance::ConfigurationInstance(File* fileToLoad)
{
    m_instanceFile = fileToLoad;

    loadFromPath();

}

ConfigurationInstance::~ConfigurationInstance()
{

    saveInstance();
    unloadAndFreeAll();

}

void ConfigurationInstance::writeToPath()
{
    //Check the instance file exists
    if (m_instanceFile == 0) return;

    //Attempt to create a new output stream
    DataOutputStream* oStream = m_instanceFile->createOutputStream();

    //Check oStream is valid
    if (oStream == 0) return;

    //If unable to open the stream
    if (oStream->open(true) == false)
    {
        //Delete the new oStream and return
        delete oStream;

        return;
    }
    else
    {

        for (unsigned int i = 0; i < m_Nodes.size(); ++i)
        {
            ConfigurationWriter::outputNode(m_Nodes[i], 0, oStream);
        }

    }

    delete oStream;

    return;

}

void ConfigurationInstance::loadFromPath()
{

    //Free all existing loaded data first
    unloadAndFreeAll();

    //If the file to load from is invalid exit
    if (m_instanceFile == 0) return;

    DataInputStream* inputStream = m_instanceFile->createInputStream();

    //If the input stream is invalid exit
    if (inputStream == 0)
    {
        printf("Configuration: Unable to gain a valid input stream\n");
        return;
    }

    //If unable to open the input stream in text read only mode
    if (inputStream->open(true) == false)
    {
        printf("Configuration: Unable to open the input stream!\n");

        //delete the stream
        delete inputStream;

        //Exit
        return;

    }

    //Read in until the Reader returns a null (Error or exit)
    ConfigurationNode* nextNode = 0;

    while (true)
    {
        printf("Configuration: Attempt to read next node\n");

        nextNode = ConfigurationReader::getNextNode(inputStream);

        //If nothing read then exit out
        if (nextNode == 0)
        {

            printf("Configuration: Next node is null\n");

            break;
        }
        else
        {
            printf("Configuration: Next node valid, adding\n");
        }

        //Otherwise, add the child and loop
        m_Nodes.push_back(nextNode);

    }

    delete inputStream;
}

void ConfigurationInstance::unloadAndFreeAll()
{

    for (unsigned int i = 0; i < m_Nodes.size(); i++)
    {
        delete m_Nodes[i];
    }

    m_Nodes.clear();

}

void ConfigurationInstance::saveInstance()
{
    writeToPath();
}

unsigned int ConfigurationInstance::getNumberOfNodes()
{
    return m_Nodes.size();
}

ConfigurationNode* ConfigurationInstance::getNode(unsigned int i)
{
    return m_Nodes[i];
}

ConfigurationNode* ConfigurationInstance::internalSearchNode(const char* Path, ConfigurationNode* cNode)
{
    const char* splitSearch = strchr(Path, '/');

    //If there is no / left
    if (splitSearch == 0)
    {

        //if the current node is 0 search at the base level
        if (cNode == 0)
        {

            //Search through each base node
            ConfigurationNode* testNode = 0;
            for (unsigned int i = 0; i < getNumberOfNodes(); ++i)
            {

                testNode = getNode(i);

                //If it matches
                if (strcmp(testNode->getName(), Path) == 0)
                {

                    //Return it
                    return testNode;
                }
            }


        }
        else
        {

            //Check the cNode is a folder
            if (cNode->getType() != configurationFolder)
            {
                printf("ERR Cannot search node, not a folder. malformed search at for %s\n", Path);
                return 0;
            }

            //Cast and create a current test node ptr
            ConfigurationFolder* folder = static_cast<ConfigurationFolder*> (cNode);
            ConfigurationNode* testNode = 0;

            //Scan through
            for (unsigned int i = 0; i < folder->numberOfChildren(); ++i)
            {

                testNode = folder->getChild(i);

                //Check if the strings compare
                if (strcmp(testNode->getName(), Path) == 0)
                {

                    //If they do then recurse with that node as the search node
                    return testNode;

                }


        }

        //Search the current node for the path, and if it does not exist return 0
        return 0;

        }
    }
    else
    {
        //If the current node is 0 search at base level
        if (cNode == 0)
        {

            ConfigurationNode* testNode = 0;

            for (unsigned int i = 0; i < getNumberOfNodes(); ++i)
            {

                //Get the node
                testNode = getNode(i);

                //Compare between the left side of the Path and the name to see if its the right one
                if (strncmp(testNode->getName(), Path, splitSearch - Path) == 0)
                {
                    return internalSearchNode(splitSearch + 1, testNode);
                }
            }

            //If no match return 0
            return 0;

        }
        else
        {
            //Check the cNode is a folder
            if (cNode->getType() != configurationFolder)
            {
                printf("ERR Cannot search node, not a folder. malformed search at for %s\n", Path);
                return 0;
            }

            //Cast and create a current test node ptr
            ConfigurationFolder* folder = static_cast<ConfigurationFolder*> (cNode);
            ConfigurationNode* testNode = 0;

            //Scan through
            for (unsigned int i = 0; i < folder->numberOfChildren(); ++i)
            {

                testNode = folder->getChild(i);

                //Check if the strings compare
                if (strncmp(testNode->getName(), Path, splitSearch - Path) == 0)
                {

                    //If they do then recurse with that node as the search node
                    return internalSearchNode(splitSearch + 1, testNode);

                }


            }

            //If no match return 0
            return 0;

        }
    }

    return 0;
}

ConfigurationNode* ConfigurationInstance::getNode(const char* Path)
{
    return internalSearchNode(Path, 0);
}
