//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_UTILS_H
#define GITIGNORE_UTILS_H
#include <cassert>
#include <stdio.h>
#define debug_wrapper(debug,function) \
if(!debug) function;                  \
else                                  \
{                                     \
    VkResult res;                     \
    res = function;                   \
    if(res != 0)                      \
    {                                 \
        printf("Error: %i /n",res);    \
    }                                 \
    assert(res == VK_SUCCESS);        \
}
#endif //GITIGNORE_UTILS_H
