//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_INSTANCE_H
#define GITIGNORE_INSTANCE_H
/*this class is used to manage the vulkan instance
 *
 * the constructor creates an instance and stores it in the instance variable
 *
 * the de-constructor will automatically destroy/clean up the instance memory
 *
 * there is a getter that is used to access the private memory of the instance variable
 */
#include <vulkan/vulkan_core.h>
namespace LunaLuxVulkanLib
{
    class Instance
    {
    private:
        VkInstance instance = nullptr;
    public:
        Instance(bool debug);
        ~Instance();
        VkInstance get();
    };
};
#endif //GITIGNORE_INSTANCE_H
