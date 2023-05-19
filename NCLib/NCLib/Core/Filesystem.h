#pragma once
namespace Filesystem {
#include <Windows.h>

    class FileReader {
    public:
        FileReader(const char* filename) : fileHandle_(CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) {}

        ~FileReader() {
            if (fileHandle_ != INVALID_HANDLE_VALUE) {
                CloseHandle(fileHandle_);
            }
        }

        bool ReadLine(char* buffer, size_t bufferSize) {
            if (fileHandle_ == INVALID_HANDLE_VALUE) {
                return false;
            }

            DWORD bytesRead;
            char ch = '\0';
            size_t i = 0;

            while (i < bufferSize - 1 && ch != '\n') {
                if (ReadFile(fileHandle_, &ch, 1, &bytesRead, NULL) == 0 || bytesRead == 0) {
                    break;
                }

                buffer[i++] = ch;
            }

            buffer[i] = '\0';

            return i > 0;
        }

    private:
        HANDLE fileHandle_;
    };
}
