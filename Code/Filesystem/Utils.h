#ifndef _FILESYSTEM_UTILITY_FUNCTIONS_
#define _FILESYSTEM_UTILITY_FUNCTIONS_
#include "File.h"

class FsUtils
{
public:

    static bool writeLineToStream(DataOutputStream* out, const char* str);

};

#endif //_FILESYSTEM_UTILITY_FUNCTIONS
