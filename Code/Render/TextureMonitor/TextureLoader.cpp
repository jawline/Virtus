#include "TextureLoader.h"
#include <stdint.h>

bool TextureLoader::load(Texture * Tex, DataInputStream* inputStream)
{
    tga_header head;

    if (inputStream == 0 || (inputStream->isOpen() == false))
    {
        printf("Bad input stream\n");
        return false;
    }

    if(inputStream->read(&head, sizeof(head)) != true)
    {
        printf("Header read fail\n");
        return false;				// Return False If It Fails
    }

    printf("Header size: %i\n", (int) sizeof(head));

    printf("TGA IMAGE DATA\nWidth: %i\nHeight: %i\nImage Type: %i\n", head.width, head.height, head.imageType);

    printf("StartX: %i StartY: %i\n", head.xStart, head.yStart);

    if (head.imageType == 2)
    {

        printf("Uncompressed TGA\n");

        if ( !loadUncompressedTGA(Tex, inputStream, &head) )
        {

            return false;

        }

    }
    else if (head.imageType == 10)
    {

        printf("Compressed TGA\n");

        if ( !loadCompressedTGA(Tex, inputStream, &head) )
        {

            return false;

        }

    }
    else
    {

        printf("Unloadable TGA\n");

        printf("Error, This TGA loader is only capable of loading RLE and uncompressed RGB or RGBA based TGA images.\n");

        return false;
    }

    printf("Loaded\n");

    return true;
}

//This function swaps the Y rows so that the origin of the image is in the top left format
void swapYRows(uint8_t* Data, tga_header* head)
{
    uint8_t bytesPerPixel = head->bits / 8;

    printf("Bytes per pixel: %i", bytesPerPixel);

    unsigned int lineSize = bytesPerPixel * head->width;
    unsigned int halfHeight = head->height / 2;
    unsigned int maxLine = head->height - 1;

    //Line buffer used when swapping
    uint8_t* tempLine = new uint8_t[lineSize];

    //For each line of the image (Only go to half because your swapping top for bottom)
    for (unsigned int currentLine = 0; currentLine < halfHeight;  ++currentLine)
    {
        //Copy the line into the line buffer
        memcpy(tempLine, Data + (currentLine * lineSize), lineSize);

        //Replace the lines data with the data from the respective line in the other half
        memcpy(Data + (currentLine * lineSize), Data + (maxLine * lineSize) - (currentLine * lineSize), lineSize);

        //Copy the stored data to the respective line in the other half
        memcpy(Data + (maxLine * lineSize) - (currentLine * lineSize), tempLine, lineSize);
    }

    delete[] tempLine;
}

bool TextureLoader::loadCompressedTGA(Texture* Tex, DataInputStream* stream, tga_header* header)
{

    if((header->bits != 24) && (header->bits !=32))
    {
        printf("Invalid/Corrupt Header\n");
        return false;
    }

    if(header->bits == 24)  				// Is It A 24bpp Image?
    {
        printf("24 BPP\n");
    }
    else  						// If It's Not 24, It Must Be 32
    {
        printf("32 BPP\n");
    }

    unsigned int bytesPP = header->bits / 8;

    printf("Bytes per pixel %i\n", bytesPP);

    unsigned int iSize = bytesPP * header->width * header->height;

    uint8_t* iDat = new uint8_t[iSize];

    if(iDat == 0)			// If Memory Can Not Be Allocated...
    {
        printf("Unable to allocate space for image\n");
        return false;				// Return False
    }

    unsigned int pixelcount = header->height * header->width;	// Number Of Pixels In The Image
    unsigned int currentpixel	= 0;			// Current Pixel We Are Reading From Data
    unsigned int currentbyte	= 0;			// Current Byte We Are Writing Into Imagedata

    // Storage For 1 Pixel
    uint8_t* colorbuffer = new uint8_t[bytesPP];

    if (colorbuffer == 0)
    {
        printf("Color buffer unable to allocate\n");
        delete[] iDat;
        return false;
    }

    do
    {
        uint8_t chunkheader = 0;											/* Storage for "chunk" header */

        if(stream->read(&chunkheader, sizeof(uint8_t)) == false)				/* Read in the 1 byte header */
        {
            printf("Error could not read RLE header");						/*Display Error */

            if(iDat != NULL)									/* If there is stored image data */
            {
                delete[] iDat;								/* Delete image data */
            }

            if (colorbuffer != NULL)
                delete[] colorbuffer;

            return false;													/* Return failed */
        }

        if(chunkheader < 128)												/* If the ehader is < 128, it means the that is the number of RAW color packets minus 1 */
        {

            short counter;													/* that follow the header */
            chunkheader++;													/* add 1 to get number of following color values */

            for(counter = 0; counter < chunkheader; counter++)				/* Read RAW color values */
            {

                if(stream->read(colorbuffer, bytesPP) != true) /* Try to read 1 pixel */
                {

                    printf("Error could not read image data");				/* IF we cant, display an error */

                    if(colorbuffer != NULL)									/* See if colorbuffer has data in it */
                    {
                        printf("Deleting color buffer\n");
                        delete[] colorbuffer;								/* If so, delete it */
                    }

                    if(iDat != NULL)										/* See if there is stored Image data */
                    {
                        printf("Deleting image data\n");
                        delete[] iDat;								/* If so, delete it too */
                    }

                    return false;														/* Return failed */
                }

                /* write to memory */
                iDat[currentbyte] = colorbuffer[2];				    /* Flip R and B vcolor values around in the process */
                iDat[currentbyte + 1] = colorbuffer[1];
                iDat[currentbyte + 2] = colorbuffer[0];

                if(bytesPP == 4)												/* if its a 32 bpp image */
                {
                    iDat[currentbyte + 3] = colorbuffer[3];				/* copy the 4th byte */
                }

                currentbyte += bytesPP;										/* Increase thecurrent byte by the number of bytes per pixel */
                currentpixel++;															/* Increase current pixel by 1 */

                if(currentpixel > pixelcount)											/* Make sure we havent read too many pixels */
                {
                    printf("Error too many pixels read");								/* if there is too many... Display an error! */

                    if(colorbuffer != NULL)												/* If there is data in colorbuffer */
                    {
                        delete[] colorbuffer;												/* Delete it */
                    }

                    if(iDat != NULL)										/* If there is Image data */
                    {
                        delete[] iDat;							/* delete it */
                    }

                    return false;														/* Return failed */
                }
            }
        }
        else																			/* chunkheader > 128 RLE data, next color  reapeated chunkheader - 127 times */
        {

            short counter;

            chunkheader -= 127;															/* Subteact 127 to get rid of the ID bit */

            if(stream->read(colorbuffer, bytesPP) != true)		/* Attempt to read following color values */
            {
                printf("Error could not read from file");			/* If attempt fails.. Display error (again) */

                if(colorbuffer != NULL)													/* If there is data in the colorbuffer */
                {
                    delete[] colorbuffer;													/* delete it */
                }

                if(iDat != NULL)											/* If thereis image data */
                {
                    delete[] iDat;										/* delete it */
                }

                return false;															/* return failed */
            }

            for(counter = 0; counter < chunkheader; counter++)					/* copy the color into the image data as many times as dictated */
            {
                /* by the header */
                iDat[currentbyte		] = colorbuffer[2];					/* switch R and B bytes areound while copying */
                iDat[currentbyte + 1	] = colorbuffer[1];
                iDat[currentbyte + 2	] = colorbuffer[0];

                if(bytesPP == 4)												/* If TGA images is 32 bpp */
                {
                    iDat[currentbyte + 3] = colorbuffer[3];				/* Copy 4th byte */
                }

                currentbyte += bytesPP;										/* Increase current byte by the number of bytes per pixel */
                currentpixel++;															/* Increase pixel count by 1 */

                if(currentpixel > pixelcount)											/* Make sure we havent written too many pixels */
                {
                    printf("Error too many pixels read");								/* if there is too many... Display an error! */

                    if(colorbuffer != NULL)												/* If there is data in colorbuffer */
                    {
                        printf("Deleting color buffer\n");
                        delete[] colorbuffer;												/* Delete it */
                    }

                    if(iDat != NULL)										/* If there is Image data */
                    {
                        printf("Deleting image data\n");
                        delete[] iDat;										/* delete it */
                    }

                    return false;														/* Return failed */
                }
            }
        }
    }
    while(currentpixel < pixelcount);

    delete[] colorbuffer;

    if (header->yStart == 0)
    {
        swapYRows(iDat, header);
    }

    if (header->bits == 24)
    {
        Tex->loadTextureData(input_data_byte_rgb, header->width, header->height, true, iDat);
    }
    else if (header->bits == 32)
    {
        Tex->loadTextureData(input_data_byte_rgba, header->width, header->height, true, iDat);
    }

    delete[] iDat;

    return true;
}

bool TextureLoader::loadUncompressedTGA(Texture* Tex, DataInputStream* stream, tga_header* header)
{

    if((header->bits != 24) && (header->bits !=32))
    {
        printf("Invalid/Corrupt Header\n");
        return false;
    }

    if(header->bits == 24)  				// Is It A 24bpp Image?
    {
        printf("RGB\n");
    }
    else  				// If It's Not 24, It Must Be 32
    {
        printf("RGBA\n");
    }

    //Bytes per pixel
    unsigned int bytesPP = header->bits / 8;

    //Image data
    uint8_t* iDat = new uint8_t[bytesPP * header->width * header->height];

    if (iDat == 0)
    {
        printf("Unable to allocate image data for uncompressed TGA image\n");
        return false;
    }

    if(stream->read(iDat, bytesPP * header->width * header->height) != true)
    {
        printf("Error: FREAD returned fail trying to read uncompressed TGA file\n");
        delete[] iDat;
        return false;				// If We Cant, Return False
    }

    //uint8_t redByte = 0;
    //uint8_t blueByte = 0;

    for(unsigned int cswap = 0; cswap <  bytesPP * header->width * header->height; cswap += bytesPP)
    {

        /*
        The OLD slower way of doing it with intermediates
        blueByte = iDat[cswap];
        redByte = iDat[cswap + 2];

        iDat[cswap] = redByte;
        iDat[cswap + 2] = blueByte;
        */

        //The new way is to use a XOR trick to swap the two values

        //Set the value of the R position to the XORed value of the B
        iDat[cswap] ^= iDat[cswap + 2];

        //Set the value of the B to the XORed value of the R (This leaves the correct B value)
        iDat[cswap + 2] ^= iDat[cswap];

        //Set the value of the R to the XOred value of the new B (Leaving the resultant as the correct R value)
        iDat[cswap] ^= iDat[cswap + 2];

    }

    if (header->yStart != 0)
    {
        swapYRows(iDat, header);
    }

    if (header->bits == 24)
    {
        Tex->loadTextureData(input_data_byte_rgb, header->width, header->height, true, iDat);
    }
    else if (header->bits == 32)
    {
        Tex->loadTextureData(input_data_byte_rgba, header->width, header->height, true, iDat);
    }

    free(iDat);

    return true;
}
