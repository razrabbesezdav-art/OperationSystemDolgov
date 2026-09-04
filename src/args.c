#include "../include/args.h"
#include <stdio.h>
#include <stdlib.h>

int
CheckPID (char *CharPID)
{
    if (CharPID == NULL)
        return -1;
    char* EndChar = NULL;
    long IntPID = strtol(CharPID, &EndChar, 10);
    
     if (EndChar == CharPID || IntPID <= 0 || 
        (*EndChar != '\0' && *EndChar != '\n')) {
        return -1;
    }
    
    return (int)IntPID;
}
