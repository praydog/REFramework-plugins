#include <cstdint>

#include <windows.h>
#include <reframework/API.hpp>

using namespace reframework; // For API class

void* g_set_cursor_pos{nullptr};

void on_frame() {
    if (g_set_cursor_pos == nullptr) {
        return;
    }

    // patch it.
    if (*(uint8_t*)g_set_cursor_pos != 0xC3) {
        DWORD old{};
        VirtualProtect(g_set_cursor_pos, 1, PAGE_EXECUTE_READWRITE, &old);
        *(uint8_t*)g_set_cursor_pos = 0xC3;
        VirtualProtect(g_set_cursor_pos, 1, old, &old);

        API::get()->log_info("Patched SetCursorPos");
    }
}

extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion* version) {
    version->major = REFRAMEWORK_PLUGIN_VERSION_MAJOR;
    version->minor = REFRAMEWORK_PLUGIN_VERSION_MINOR;
    version->patch = REFRAMEWORK_PLUGIN_VERSION_PATCH;
    version->game_name = "RE7";
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    g_set_cursor_pos = GetProcAddress(GetModuleHandleA("user32.dll"), "SetCursorPos");

    API::initialize(param);

    // Example usage of param functions
    const auto functions = param->functions;
    functions->on_frame(on_frame);

    API::get()->log_info("SetCursorPos: 0x%p", g_set_cursor_pos);

    return true;
}