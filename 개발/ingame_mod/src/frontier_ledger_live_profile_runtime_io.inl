static void frontier_ensure_runtime_ini_path(char *path, size_t path_size, const char *file_name)
{
    if (path == NULL || path_size == 0U || file_name == NULL || file_name[0] == '\0') {
        return;
    }
    if (path[0] == '\0' && g_runtime.module_path[0] != '\0') {
        frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
    }
}

static void frontier_touch_runtime_file(const char *path)
{
    HANDLE handle;

    if (path == NULL || path[0] == '\0') {
        return;
    }

    handle = CreateFileA(
        path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
    }
}
