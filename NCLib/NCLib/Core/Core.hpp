#pragma once
#include <cstdint>
#include <heapapi.h>
#include "Utils/Basic.hpp"
using namespace Basic;
#define INVALID_INDEX -1

namespace NonCRT {
	namespace String {
		class IString {
		private:
			size_t capacity;
			char* str;
		public:
			static const size_t npos = static_cast<size_t>(-1);
			size_t leng;

			//CONSTR
			IString() : str((char*)HeapAlloc(GetProcessHeap(), 0, sizeof(char))), leng(0), capacity(1) {
				if (str) {
					str[0] = '\0';
				}
			}

			IString(const char* s) : str(nullptr), leng(0), capacity(0) {
				size_t len = 0;
				while (s[len] != '\0') {
					len++;
				}
				str = (char*)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(char));
				if (str) {
					size_t i = 0;
					while (s[i] != '\0') {
						str[i] = s[i];
						i++;
					}
					str[i] = '\0';
					leng = len;
					capacity = len + 1;
				}
			}

			~IString() {
				HeapFree(GetProcessHeap(), 0, str);
			}

			IString(const IString& other) : str(nullptr), leng(other.leng), capacity(other.capacity) {
				str = (char*)HeapAlloc(GetProcessHeap(), 0, capacity * sizeof(char));
				if (str) {
					size_t i = 0;
					while (other.str[i] != '\0' && i < leng) {
						str[i] = other.str[i];
						i++;
					}
					str[i] = '\0';
				}
			}
			//CONSTR

			//OPERATORS
			IString& operator=(const IString& other) {
				if (this != &other) {
					size_t len = strLeng(other.str);
					char* newStr = (char*)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(char));
					if (newStr) {
						for (size_t i = 0; i < len; i++) {
							newStr[i] = other.str[i];
						}
						newStr[len] = '\0';
						HeapFree(GetProcessHeap(), 0, str);
						str = newStr;
						leng = len;
						capacity = len + 1;
					}
				}
				return *this;
			}

			IString operator+(const IString& other) const {
				size_t len1 = strLeng(str);
				size_t len2 = strLeng(other.str);
				char* newStr = (char*)HeapAlloc(GetProcessHeap(), 0, (len1 + len2 + 1) * sizeof(char));
				for (size_t i = 0; i < len1; i++) {
					newStr[i] = str[i];
				}
				for (size_t i = 0; i < len2; i++) {
					newStr[len1 + i] = other.str[i];
				}
				newStr[len1 + len2] = '\0';
				IString result(newStr);
				HeapFree(GetProcessHeap(), 0, newStr);
				return result;
			}

			//OPERATORS

			//METHODS
			operator const void* () const {
				return static_cast<const void*>(str);
			}

			void reserve(size_t n) {
				if (n > capacity) {
					char* newStr = (char*)HeapAlloc(GetProcessHeap(), 0, (n + 1) * sizeof(char));
					if (newStr) {
						for (size_t i = 0; i < leng; i++) {
							newStr[i] = str[i];
						}
						newStr[leng] = '\0';
						HeapFree(GetProcessHeap(), 0, str);
						str = newStr;
						capacity = n;
					}
				}
			}

			void pushBack(char c) {
				if (leng == capacity) {
					reserve((capacity == 0) ? 1 : capacity * 2);
				}
				str[leng++] = c;
				str[leng] = '\0';
			}

			void iErase(size_t pos, size_t count = npos) {
				size_t len = strLeng(str);
				count = (count == npos) ? (len - pos) : (count);
				if (pos >= len || count == 0) {
					return;
				}
				if (pos + count >= len) {
					str[pos] = '\0';
					leng = pos;
				}
				else {
					for (size_t i = pos; i < len - count; ++i) {
						str[i] = str[i + count];
					}
					str[len - count] = '\0';
					leng = len - count;
				}
			}

			void iInsert(size_t pos, const char* sub) {
				if (sub == nullptr || pos > leng) {
					return;
				}
				size_t subLength = strLeng(sub);
				size_t newLength = leng + subLength;
				if (newLength > capacity) {
					reserve(newLength * 2);
				}
				for (size_t i = leng; i >= pos; --i) {
					str[i + subLength] = str[i];
				}
				for (size_t i = 0; i < subLength; ++i) {
					str[pos + i] = sub[i];
				}
				leng = newLength;
			}

			/*IString& iReplace(const char* oldStr, const char* newStr) {
				size_t oldStrLength = strLeng(oldStr);
				size_t newStrLength = strLeng(newStr);
				size_t startIndex = 0;
				size_t pos = 0;
				IString result;

				while ((pos = iFind(oldStr, startIndex)) != INVALID_INDEX) {
					result += iSubstr(startIndex, pos - startIndex);
					result += newStr;
					startIndex = pos + oldStrLength;
				}

				result += iSubstr(startIndex, length() - startIndex);
				str = result.cStr();

				return *this;
			}*/



			size_t iFind(const char* sub) const {
				if (sub == nullptr) {
					return npos;
				}
				size_t subLength = strLeng(sub);
				if (subLength == 0) {
					return 0;
				}
				size_t i = 0;
				while (i < leng) {
					i = iFindCh(sub[0], i);
					if (i == npos || i + subLength > leng) {
						return npos;
					}
					bool found = true;
					for (size_t j = 1; j < subLength; j++) {
						if (str[i + j] != sub[j]) {
							found = false;
							break;
						}
					}
					if (found) {
						return i;
					}
					i++;
				}
				return npos;
			}

			size_t iFindCh(char ch, size_t startIndex = 0) const {
				size_t strLength = strLeng(str);
				for (size_t i = startIndex; i < strLength; ++i) {
					if (str[i] == ch) {
						return i;
					}
				}
				return strLength;
			}

			IString iSubstr(size_t pos, size_t len) const {
				if (pos > leng) {
					return IString();
				}
				if (len == npos || pos + len > leng) {
					len = leng - pos;
				}
				char* newStr = (char*)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(char));
				for (size_t i = 0; i < len; i++) {
					newStr[i] = str[pos + i];
				}
				newStr[len] = '\0';
				IString result(newStr);
				HeapFree(GetProcessHeap(), 0, newStr);
				return result;
			}

			size_t length() const {
				size_t len = 0;
				while (str[len] != '\0') {
					++len;
				}
				return len;
			}

			const char* cStr() const {
				char* cstr = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, leng + 1);
				if (cstr) {
					for (int i = 0; i < leng; i++) {
						cstr[i] = str[i];
					}
					cstr[leng] = '\0';
				}
				return cstr;
			}
			//METHODS

		};
	}//String
}
