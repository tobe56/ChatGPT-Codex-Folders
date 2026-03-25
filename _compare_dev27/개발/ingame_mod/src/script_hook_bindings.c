#include "script_hook_bindings.h"

typedef void(__cdecl *ScriptRegisterFn)(HMODULE module, FrontierScriptMainFn script_main);
typedef void(__cdecl *ScriptUnregisterModuleFn)(HMODULE module);
typedef void(__cdecl *KeyboardHandlerRegisterFn)(FrontierKeyboardHandlerFn handler);
typedef void(__cdecl *KeyboardHandlerUnregisterFn)(FrontierKeyboardHandlerFn handler);
typedef void(__cdecl *ScriptWaitFn)(DWORD milliseconds);
typedef void(__cdecl *NativeInitFn)(uint64_t hash);
typedef void(__cdecl *NativePush64Fn)(uint64_t value);
typedef uint64_t *(__cdecl *NativeCallFn)(void);

static struct {
    HMODULE module;
    ScriptRegisterFn script_register;
    ScriptUnregisterModuleFn script_unregister_module;
    KeyboardHandlerRegisterFn keyboard_register;
    KeyboardHandlerUnregisterFn keyboard_unregister;
    ScriptWaitFn script_wait;
    NativeInitFn native_init;
    NativePush64Fn native_push64;
    NativeCallFn native_call;
    int ready;
} g_script_hook;

static FARPROC frontier_get_export(HMODULE module, const char *name)
{
    if (module == NULL) {
        return NULL;
    }
    return GetProcAddress(module, name);
}

int FrontierScriptHook_Bind(void)
{
    if (g_script_hook.ready) {
        return 1;
    }

    g_script_hook.module = GetModuleHandleA("ScriptHookRDR2.dll");
    if (g_script_hook.module == NULL) {
        return 0;
    }

    g_script_hook.script_register = (ScriptRegisterFn)frontier_get_export(
        g_script_hook.module,
        "?scriptRegister@@YAXPEAUHINSTANCE__@@P6AXXZ@Z"
    );
    g_script_hook.script_unregister_module = (ScriptUnregisterModuleFn)frontier_get_export(
        g_script_hook.module,
        "?scriptUnregister@@YAXPEAUHINSTANCE__@@@Z"
    );
    g_script_hook.keyboard_register = (KeyboardHandlerRegisterFn)frontier_get_export(
        g_script_hook.module,
        "?keyboardHandlerRegister@@YAXP6AXKGEHHHH@Z@Z"
    );
    g_script_hook.keyboard_unregister = (KeyboardHandlerUnregisterFn)frontier_get_export(
        g_script_hook.module,
        "?keyboardHandlerUnregister@@YAXP6AXKGEHHHH@Z@Z"
    );
    g_script_hook.script_wait = (ScriptWaitFn)frontier_get_export(
        g_script_hook.module,
        "?scriptWait@@YAXK@Z"
    );
    g_script_hook.native_init = (NativeInitFn)frontier_get_export(
        g_script_hook.module,
        "?nativeInit@@YAX_K@Z"
    );
    g_script_hook.native_push64 = (NativePush64Fn)frontier_get_export(
        g_script_hook.module,
        "?nativePush64@@YAX_K@Z"
    );
    g_script_hook.native_call = (NativeCallFn)frontier_get_export(
        g_script_hook.module,
        "?nativeCall@@YAPEA_KXZ"
    );

    g_script_hook.ready =
        g_script_hook.script_register != NULL &&
        g_script_hook.script_unregister_module != NULL &&
        g_script_hook.keyboard_register != NULL &&
        g_script_hook.keyboard_unregister != NULL &&
        g_script_hook.script_wait != NULL &&
        g_script_hook.native_init != NULL &&
        g_script_hook.native_push64 != NULL &&
        g_script_hook.native_call != NULL;
    return g_script_hook.ready;
}

int FrontierScriptHook_IsReady(void)
{
    return g_script_hook.ready;
}

void FrontierScriptHook_RegisterScript(HMODULE module, FrontierScriptMainFn script_main)
{
    if (g_script_hook.ready && g_script_hook.script_register != NULL) {
        g_script_hook.script_register(module, script_main);
    }
}

void FrontierScriptHook_UnregisterModule(HMODULE module)
{
    if (g_script_hook.ready && g_script_hook.script_unregister_module != NULL) {
        g_script_hook.script_unregister_module(module);
    }
}

void FrontierScriptHook_RegisterKeyboard(FrontierKeyboardHandlerFn handler)
{
    if (g_script_hook.ready && g_script_hook.keyboard_register != NULL) {
        g_script_hook.keyboard_register(handler);
    }
}

void FrontierScriptHook_UnregisterKeyboard(FrontierKeyboardHandlerFn handler)
{
    if (g_script_hook.ready && g_script_hook.keyboard_unregister != NULL) {
        g_script_hook.keyboard_unregister(handler);
    }
}

void FrontierScriptHook_Wait(DWORD milliseconds)
{
    if (g_script_hook.ready && g_script_hook.script_wait != NULL) {
        g_script_hook.script_wait(milliseconds);
    }
}

uint64_t *FrontierNative_InvokeRaw(uint64_t hash, const uint64_t *args, int arg_count)
{
    int index = 0;

    if (!g_script_hook.ready || g_script_hook.native_init == NULL || g_script_hook.native_push64 == NULL || g_script_hook.native_call == NULL) {
        return NULL;
    }

    g_script_hook.native_init(hash);
    for (index = 0; index < arg_count; ++index) {
        g_script_hook.native_push64(args[index]);
    }
    return g_script_hook.native_call();
}
