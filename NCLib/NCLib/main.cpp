#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define _AMD64_
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#define _X86_
#elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#define _ARM_
#endif

#include "Core/Core.hpp"
#include <windows.h>
#define _AFXDLL
using namespace NonCRT;
using namespace NonCRT::String;
static const size_t npos = NonCRT::String::IString::npos;


int _CustomEntry() {
    //int в строку
    int myInt(5);
    iPrint(intToString(myInt));

    //Циклы по массиву
	IString it[] = { "str1", "str2", "str3" };
	for (int i = 0; i < 3; i++) {
		iPrint(it[i]);
	}

    // Проверка на пустую строку
    IString s("Hello, world!");
    IString emptySubstr = s.iSubstr(s.leng + 1, 5);
    iPrint(emptySubstr);

    // Проверка на len = npos
    IString nposSubstr = s.iSubstr(7, IString::npos);
    iPrint(nposSubstr);

    // Проверка на len < оставшейся длины строки
    IString lenSubstr = s.iSubstr(7, 3);
    iPrint( lenSubstr );

    // Проверка на len > оставшейся длины строки
    IString remainingSubstr = s.iSubstr(7, 20);
    iPrint(remainingSubstr );

    // Случайная строка и значения pos и len
    IString randomStr("abcdefg");
    size_t pos = 2;
    size_t len = 3;
    IString randomSubstr = randomStr.iSubstr(pos, len);
    iPrint( randomSubstr);
    iPrint(s + randomStr);

    // Первое вхождение символа в строке
    /*IString ptrStr("Hello, World!");    
    iPrint(ptrStr.iSubstr(',', 2));*/


    //Аналог регуляркам
    const char* str = "{{\"textTetxtTEXT:\" \"text1text2text3\"}; {\"wordWordWORD\": \"word1word2word3\"}; {\"textTetxtTEXT:\" \"text4text5text6\"}; {\"myString:\" \"MyString!!!\"}; {\"textTetxtTEXT:\" \"text7text8text9\"};}";
    const char* searchStr = "\"myString:\" ";
    size_t startIndex = IString(str).iFind(searchStr);
    if (startIndex != INVALID_INDEX) {
        startIndex += IString(searchStr).length();
        size_t endIndex = IString(str).iFindCh('}', startIndex);
        if (endIndex == INVALID_INDEX) {
            endIndex = IString(str).length() - 1;
        }
        IString result = IString(str).iSubstr(startIndex, endIndex - startIndex);
        iPrint(result.cStr());
    }
    else {
        iPrint("String not found.");
    }


    iPrint( "\nPress enter to continue...");
    pause();
    return 0;
}


