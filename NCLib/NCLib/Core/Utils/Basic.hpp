#pragma once
#include <windows.h>

namespace Basic {
	class EncodingDetector {
	private:
		const char* str;
	public:
		EncodingDetector(const char* s) : str(s) {}

		bool isASCII() const {
			const unsigned char* ustr = reinterpret_cast<const unsigned char*>(str);
			while (*ustr) {
				if (*ustr & 0x80) return false;
				ustr++;
			}
			return true;
		}

		bool isUTF8() const {
			const unsigned char* ustr = reinterpret_cast<const unsigned char*>(str);
			while (*ustr) {
				if (*ustr <= 0x7F) {  // ASCII character
					ustr++;
				}
				else if ((*ustr & 0xE0) == 0xC0) {  // 2-byte character
					if ((ustr[1] & 0xC0) != 0x80) return false;
					ustr += 2;
				}
				else if ((*ustr & 0xF0) == 0xE0) {  // 3-byte character
					if ((ustr[1] & 0xC0) != 0x80 || (ustr[2] & 0xC0) != 0x80) return false;
					ustr += 3;
				}
				else if ((*ustr & 0xF8) == 0xF0) {  // 4-byte character
					if ((ustr[1] & 0xC0) != 0x80 || (ustr[2] & 0xC0) != 0x80 || (ustr[3] & 0xC0) != 0x80) return false;
					ustr += 4;
				}
				else {
					return false;
				}
			}
			return true;
		}

		void convertASCIIToUTF8(const char* asciiStr, char* utf8Str) {
			int i = 0, j = 0;
			while (asciiStr[i]) {
				unsigned char ch = asciiStr[i];
				if (ch <= 0x7F) {
					utf8Str[j++] = ch;
				}
				else {
					utf8Str[j++] = 0xC0 | (ch >> 6);
					utf8Str[j++] = 0x80 | (ch & 0x3F);
				}
				i++;
			}
			utf8Str[j] = '\0';
		}

		void convertUTF8ToASCII(const char* utf8Str, char* asciiStr) {
			int i = 0, j = 0;
			while (utf8Str[i]) {
				unsigned char ch = utf8Str[i];
				if (ch <= 0x7F) {
					asciiStr[j++] = ch;
				}
				else if ((ch & 0xE0) == 0xC0) {
					asciiStr[j++] = ((ch & 0x1F) << 6) | (utf8Str[i + 1] & 0x3F);
					i++;
				}
				else {
					// Invalid UTF-8 sequence
					break;
				}
				i++;
			}
			asciiStr[j] = '\0';
		}
	};

	const int MAX_DIGITS = 16;

	size_t strLeng(const char* str) {
		if (str == nullptr) {
			return 0;
		}
		size_t leng = 0;
		while (str[leng] != '\0') {
			++leng;
		}
		return leng;
	}

	char* intToString(int value) {
		static char result[MAX_DIGITS + 1];
		int sign = (value < 0) ? -1 : 1;
		int numDigits = 0;
		if (value == 0) {
			result[numDigits++] = '0';
		}
		else {
			value *= sign;
			while (value != 0 && numDigits < MAX_DIGITS) {
				result[numDigits++] = '0' + value % 10;
				value /= 10;
			}
		}
		if (sign == -1 && numDigits < MAX_DIGITS) {
			result[numDigits++] = '-';
		}
		for (int i = 0; i < numDigits / 2; i++) {
			char temp = result[i];
			result[i] = result[numDigits - i - 1];
			result[numDigits - i - 1] = temp;
		}
		result[numDigits] = '\0';
		return result;
	}

	//Prints
	static void iPrint(const void* data) {
		DWORD written;
		const char* newline = "\r\n";
		const char* str = reinterpret_cast<const char*>(data);
		const int strLength = strLeng(str);

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < strLength; i++) {
			WriteFile(hOut, &str[i], 1, &written, NULL);
		}
		WriteFile(hOut, newline, strLeng(newline), &written, NULL);
		return;
	}

	void encPrint(const char* str) {
		EncodingDetector detector(str);
		if (detector.isASCII()) {
			iPrint(str);
		}
		else if (detector.isUTF8()) {
			char utf8Str[1024];
			detector.convertUTF8ToASCII(str, utf8Str);
			iPrint(utf8Str);
		}
	}

	void pause() {
		INPUT_RECORD ir;
		DWORD count;
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

		while (1) {
			ReadConsoleInput(hStdIn, &ir, 1, &count);

			if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
				break;
			}
		}
	}
}
