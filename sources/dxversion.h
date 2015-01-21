#ifndef versionH
#define versionH

#include "time.h"
#include "sys/time.h"

struct TdxVERSION{ int build; time_t time; char name[128]; }
VERSION __attribute__((section("VERSION_SECTION"))) = 
{ 10, 1421762126, "0.0" };

#endif
