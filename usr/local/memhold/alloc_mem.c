
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

#include "clog.h"

#define ALLOC_MEGA_B 1*1024*1024
#define ALLOC_SYS_MB 110
/* MEM_HOLD_MB=200 */

int main()
{
    int * buf = NULL;
    int count = 0;
    char * strIntEnv = NULL;
    int iMemMax = ALLOC_SYS_MB;
    int iSecStep = 10;
    int iRet = -1;

    CLogger_t logger = {
        .fileCnt = 4,
        .maxSize = 64 * 1024,
    };
    iRet = CLogInitLogger(&logger, "/var/log/memhold.log");

    strIntEnv = getenv("MEM_HOLD_MB");
    if (NULL == strIntEnv)
    {
        iMemMax = ALLOC_SYS_MB;
    }
    else
    {
        iMemMax = atoi(strIntEnv);
    }
    strIntEnv = NULL;
    strIntEnv = getenv("SEC_STEP_MB");
    if (NULL != strIntEnv)
    {
        iSecStep = atoi(strIntEnv);
    }

    for(count = 1; count <= iMemMax; count++)
    {
        buf = malloc(ALLOC_MEGA_B);
        if (NULL == buf)
        {
            printf("Failed to malloc memory of %d bytes.\n", ALLOC_MEGA_B);
	    return -1;
        }
	memset(buf, 0x00, ALLOC_MEGA_B);
        if ((count < 100) && (0 == count%20))
        {
            CLog(&logger, "Allocate %d MB Memory.\n", count);
        }
        if ((count >= 100) && (0 == count%5))
	{
            CLog(&logger, "Allocate %d MB Memory.\n", count);
	}
        if ((count >= 120) && (0 == count%2))
	{
            CLog(&logger, "Allocate %d MB Memory.\n", count);
	}
	printf("Allocate %d MB Memory.\n", count);

	if (count % iSecStep == 0)
	{
            sleep(1);
	}
    }
    printf("Successfully running 600 secondes, and keep %d MB memory allocated.\n", iMemMax);
    CLog(&logger, "Successfully hold mem of %d MB and keep running .\n", iMemMax);
    
    while (1)
    {
        sleep(30);
	CLog(&logger, "Successfully hold mem of %d MB and keep running .\n", iMemMax);
    }

    CLogUninitLogger(&logger);
    
    return 0;
}

