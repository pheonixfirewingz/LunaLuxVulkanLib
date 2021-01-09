//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_UTILS_H
#define GITIGNORE_UTILS_H
#include <cassert>
#define debug_wrapper(debug,function) \
if(!debug) function;                  \
else                                  \
{                                     \
    VkResult res;                     \
    res = function;                   \
    assert(res == VK_SUCCESS);        \
}
#endif //GITIGNORE_UTILS_H
