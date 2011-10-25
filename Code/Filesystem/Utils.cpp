#include "Utils.h"
#include <string.h>

bool FilesystemUtils::writeLineToStream(DataOutputStream* outstrm, const char* str)
{

    return outstrm->write(str, strlen(str));

}
