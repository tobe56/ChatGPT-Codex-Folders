#ifndef SCRIPT_HOOK_BINDINGS_H
#define SCRIPT_HOOK_BINDINGS_H

#include <stdint.h>
#include <windows.h>

typedef void (*FrontierScriptMainFn)(void);
typedef void (*FrontierKeyboardHandlerFn)(DWORD key, WORD repeats, BYTE scan_code, BOOL is_extended, BOOL with_alt, BOOL was_down_before, BOOL is_up_now);

int FrontierScriptHook_Bind(void);
int FrontierScriptHook_IsReady(void);
void FrontierScriptHook_RegisterScript(HMODULE module, FrontierScriptMainFn script_main);
void FrontierScriptHook_UnregisterModule(HMODULE module);
void FrontierScriptHook_RegisterKeyboard(FrontierKeyboardHandlerFn handler);
void FrontierScriptHook_UnregisterKeyboard(FrontierKeyboardHandlerFn handler);
void FrontierScriptHook_Wait(DWORD milliseconds);
uint64_t *FrontierNative_InvokeRaw(uint64_t hash, const uint64_t *args, int arg_count);

static inline uint64_t FrontierPackBool(int value)
{
    return value ? 1ULL : 0ULL;
}

static inline uint64_t FrontierPackInt(int value)
{
    return (uint64_t)(uint32_t)value;
}

static inline uint64_t FrontierPackUInt64(uint64_t value)
{
    return value;
}

static inline uint64_t FrontierPackPointer(const void *value)
{
    return (uint64_t)(uintptr_t)value;
}

static inline uint64_t FrontierPackFloat(float value)
{
    uint64_t raw = 0ULL;
    unsigned char *dst = (unsigned char *)&raw;
    unsigned char *src = (unsigned char *)&value;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    return raw;
}

#endif
