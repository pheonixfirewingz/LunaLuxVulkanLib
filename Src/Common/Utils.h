//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_UTILS_H
#define GITIGNORE_UTILS_H
#include <stdio.h>
#include <vulkan/vulkan_core.h>
#define debug_wrapper(debug,function) \
if(!debug) function;                  \
else                                  \
{                                     \
    VkResult res;                     \
    if((res = function) != VK_SUCCESS) printf("Error: %i \n",res); \
}
#endif //GITIGNORE_UTILS_H
