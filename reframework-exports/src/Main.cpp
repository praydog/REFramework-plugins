#include <cstdint>

#include <windows.h>
#include <reframework/API.hpp>

using namespace reframework; // For API class

extern "C" __declspec(dllexport) const REFrameworkPluginInitializeParam* g_param = nullptr;

extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version) {
    version->major = REFRAMEWORK_PLUGIN_VERSION_MAJOR;
    version->minor = REFRAMEWORK_PLUGIN_VERSION_MINOR;
    version->patch = REFRAMEWORK_PLUGIN_VERSION_PATCH;
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    API::initialize(param);

    g_param = param;

    return true;
}