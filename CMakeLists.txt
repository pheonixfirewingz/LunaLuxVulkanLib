cmake_minimum_required(VERSION 3.16)
project(LunaLuxVulkanLib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(includes STATIC)
include_directories(C:\\VulkanSDK\\1.2.162.0\\Include STATIC)
add_library(LunaLuxVulkanLib STATIC
            Src/VulkanLib.cpp
            Src/VulkanLib.h
            Src/Context/Interface/Platform.h
            Src/Context/ContextInterface.cpp
            Src/Context/ContextInterface.h
            Src/Common/Utils.h
        Src/Context/Debug.cpp Src/Context/Debug.h Src/VulkanLibRenderComands.h Src/VulkanLibRenderComands.cpp Src/Context/Componants/Instance.cpp Src/Context/Componants/Instance.h Src/Context/Componants/Device.cpp Src/Context/Componants/Device.h Src/Context/Componants/Surface.cpp Src/Context/Componants/Surface.h Src/Context/Componants/SwapChain.cpp Src/Context/Componants/SwapChain.h Src/Context/Componants/DepthHandler.cpp Src/Context/Componants/DepthHandler.h Src/Context/Componants/FrameBuffer.cpp Src/Context/Componants/FrameBuffer.h)

target_link_libraries(LunaLuxVulkanLib vulkan-1.lib)
link_directories(C:\\VulkanSDK\\1.2.162.0\\Lib)
include_directories(STATIC Src)
add_executable(Test Main.cpp)
target_link_libraries(Test LunaLuxVulkanLib ${CMAKE_SOURCE_DIR}/libs/LunaLuxWindowLib_Debug.lib)