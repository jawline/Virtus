#include "Utils.h"
#include <string.h>

bool FsUtils::writeLineToStream(DataOutputStream* outstrm, const char* str)
{

    return outstrm->write(str, strlen(str));

}
