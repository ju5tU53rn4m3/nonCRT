#pragma once
namespace Filesystem {
#include <Windows.h>

    class FileReader {
    public:
        FileReader(const char* filename) : file_handle_(CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) {}

        ~FileReader() {
            if (file_handle_ != INVALID_HANDLE_VALUE) {
                CloseHandle(file_handle_);
            }
        }

        bool ReadLine(char* buffer, size_t buffer_size) {
            if (file_handle_ == INVALID_HANDLE_VALUE) {
                return false;
            }

            DWORD bytes_read;
            char ch = '\0';
            size_t i = 0;

            while (i < buffer_size - 1 && ch != '\n') {
                if (ReadFile(file_handle_, &ch, 1, &bytes_read, NULL) == 0 || bytes_read == 0) {
                    break;
                }

                buffer[i++] = ch;
            }

            buffer[i] = '\0';

            return i > 0;
        }

    private:
        HANDLE file_handle_;
    };
}