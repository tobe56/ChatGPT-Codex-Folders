static int frontier_runtime_file_has_content(const char *path)
{
    HANDLE handle;
    LARGE_INTEGER size;

    if (path == NULL || path[0] == '\0') {
        return 0;
    }

    handle = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    size.QuadPart = 0;
    if (!GetFileSizeEx(handle, &size)) {
        CloseHandle(handle);
        return 0;
    }

    CloseHandle(handle);
    return size.QuadPart > 0;
}

static int frontier_write_ini_string_retry(
    const char *file_name,
    const char *section,
    const char *key,
    const char *value,
    char *path,
    size_t path_size
)
{
    frontier_ensure_runtime_ini_path(path, path_size, file_name);
    if (path == NULL || path[0] == '\0') {
        return 0;
    }
    frontier_touch_runtime_file(path);
    if (WritePrivateProfileStringA(section, key, value, path)) {
        return 1;
    }
    frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
    frontier_touch_runtime_file(path);
    return WritePrivateProfileStringA(section, key, value, path) != 0;
}

static int frontier_flush_ini_retry(const char *file_name, char *path, size_t path_size)
{
    int flush_ok = 0;

    frontier_ensure_runtime_ini_path(path, path_size, file_name);
    if (path == NULL || path[0] == '\0') {
        return 0;
    }
    frontier_touch_runtime_file(path);
    if (WritePrivateProfileStringA(NULL, NULL, NULL, path)) {
        flush_ok = 1;
    } else {
        frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
        frontier_touch_runtime_file(path);
        flush_ok = WritePrivateProfileStringA(NULL, NULL, NULL, path) != 0;
    }
    if (flush_ok) {
        return 1;
    }
    return frontier_runtime_file_has_content(path);
}

