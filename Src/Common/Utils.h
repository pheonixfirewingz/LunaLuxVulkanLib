//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_UTILS_H
#define GITIGNORE_UTILS_H
#include <stdio.h>
#define debug_wrapper(debug,function) \
if(!debug) function;                  \
else                                  \
{                                     \
    VkResult res;                     \
    res = function;                   \
    if(res != VK_SUCCESS)             \
    {                                 \
        printf("Error: %i \n",res);    \
    }                                 \
}
#endif //GITIGNORE_UTILS_H
