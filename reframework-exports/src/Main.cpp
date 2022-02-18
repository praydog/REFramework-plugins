#include <cstdint>
#include <vector>
#include <mutex>

#include <windows.h>
#include <reframework/API.hpp>

using namespace reframework; // For API class

extern "C" __declspec(dllexport) const REFrameworkPluginInitializeParam* g_param = nullptr;

extern "C" __declspec(dllexport) const REFrameworkPluginInitializeParam* get_g_param() {
    return g_param;
}

using OnParamInitialize = void (*)(const REFrameworkPluginInitializeParam*);

std::vector<OnParamInitialize> g_on_param_initialize_list{};
std::mutex g_on_param_initialize_list_mutex{};

extern "C" __declspec(dllexport) void on_g_param(OnParamInitialize func) {
    if (func == nullptr) {
        return;
    }

    std::scoped_lock _{g_on_param_initialize_list_mutex};
    g_on_param_initialize_list.push_back(func);

    if (g_param != nullptr) {
        func(g_param);
    }
}

extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version) {
    version->major = REFRAMEWORK_PLUGIN_VERSION_MAJOR;
    version->minor = REFRAMEWORK_PLUGIN_VERSION_MINOR;
    version->patch = REFRAMEWORK_PLUGIN_VERSION_PATCH;
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    std::scoped_lock _{g_on_param_initialize_list_mutex};

    API::initialize(param);
    g_param = param;
    
    for (const auto& func : g_on_param_initialize_list) {
        func(param);
    }

    return true;
}