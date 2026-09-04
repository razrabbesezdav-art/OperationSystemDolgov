#include "../include/args.h"
#include <stdio.h>
#include <stdlib.h>

int
CheckPID (char *CharPID)
{
    if (CharPID == NULL)
        return -1;
    char* EndChar = NULL;
    int IntPID = strtol(CharPID, &EndChar, 10);
    
    if(IntPID < 0 || (*EndChar != (NULL || "\n"))){
        return -1;
    }
    else{
        return IntPID;
    }
}
