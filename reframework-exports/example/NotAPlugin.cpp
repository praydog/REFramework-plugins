#include <string>
#include <chrono>
#include <Windows.h>
#include <reframework/API.hpp>

using namespace reframework;

const REFrameworkPluginInitializeParam* g_param = nullptr;

using OnParamInitialize = void (*)(const REFrameworkPluginInitializeParam*);

void on_g_param(OnParamInitialize func) {
    const auto export_module = []() {
        HMODULE result{nullptr};

        const auto now = std::chrono::steady_clock::now();
        
        while (result == nullptr) {
            result = GetModuleHandleA("reframework-exports.dll");

            if (std::chrono::steady_clock::now() - now > std::chrono::seconds(5)) {
                break;
            }
        }

        return result;
    }();

    if (export_module == nullptr) {
        MessageBoxA(nullptr, "NotAPlugin: Could not find reframework-exports.dll", "Error", MB_OK);
        return;
    }

    const auto on_g_param_func = (void (*)(OnParamInitialize))GetProcAddress(export_module, "on_g_param");

    if (on_g_param_func == nullptr) {
        MessageBoxA(nullptr, "NotAPlugin: Could not find on_g_param()", "Error", MB_OK);
        return;
    }

    on_g_param_func(func);
}

void on_param_initialize(const REFrameworkPluginInitializeParam* param) {
    API::initialize(param);
    API::get()->log_info("NotAPlugin: Param created");

    g_param = param;
}

void set_shadow_quality_to_shit() {
    auto& api = API::get();

    const auto tdb = api->tdb();

    const auto renderer_t = tdb->find_type("via.render.Renderer");
    const auto renderer = api->get_native_singleton("via.render.Renderer");

    const auto get_render_config = renderer_t->find_method("get_RenderConfig");

    if (get_render_config == nullptr) {
        api->log_error("NotAPlugin: Could not find get_RenderConfig()");
        return;
    }

    auto render_config = get_render_config->call<API::ManagedObject*>(api->get_vm_context(), renderer);

    if (render_config == nullptr) {
        api->log_error("NotAPlugin: get_RenderConfig() returned nullptr");
        return;
    }

    render_config->call("set_ShadowQuality", api->get_vm_context(), render_config, 2); // LOWEST
}

void example_thread() {
    on_g_param(on_param_initialize);

    // probably DONT do it like this
    while (g_param == nullptr) {
        std::this_thread::yield();
    }
    
    auto& api = API::get();

    api->log_info("NotAPlugin: Hello from the DLL!");

    set_shadow_quality_to_shit();

    api->log_info("NotAPlugin: Done!");
}

// dllmain
BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)example_thread, nullptr, 0, nullptr);
        break;
    }

    return TRUE;
}