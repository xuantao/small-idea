/*
 * to_strings
 * xuantao, 2018-04-19
 * ���ܽ�to_string����������ط����������ռ���
 * ģ�庯������ʱ����Ԫ�������ֲ��ҿ���������
*/
#pragma once

#include "common.h"
#include <string>
#include <string.h>

inline int to_string(char* buff, size_t size, const char* val)
{
    return snprintf(buff, size, "%s", (const char*)(val ? val : "nullptr"));
}

inline int to_string(char* buff, size_t size, const std::string& val)
{
    return snprintf(buff, size, "%s", val.c_str());
}

inline int to_string(char* buff, size_t size, bool val)
{
    return to_string(buff, size, (const char*)(val ? "true" : "false"));
}

inline int to_string(char* buff, size_t size, char val)
{
    return snprintf(buff, size, "%c", val);
}

inline int to_string(char* buff, size_t size, unsigned char val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, short val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, unsigned short val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, int val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, unsigned int val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, long val)
{
    return snprintf(buff, size, "%ld", val);
}

inline int to_string(char* buff, size_t size, unsigned long val)
{
    return snprintf(buff, size, "%ld", val);
}

inline int to_string(char* buff, size_t size, long long val)
{
    return snprintf(buff, size, "%lld", val);
}

inline int to_string(char* buff, size_t size, unsigned long long val)
{
    return snprintf(buff, size, "%lld", val);
}

inline int to_string(char* buff, size_t size, float val)
{
    return snprintf(buff, size, "%f", val);
}

inline int to_string(char* buff, size_t size, double val)
{
    return snprintf(buff, size, "%f", val);
}

inline int to_string(char* buff, size_t size, const void* val)
{
    return snprintf(buff, size, "%p", val);
}

inline int to_string(char* buff, size_t size, std::nullptr_t)
{
    return snprintf(buff, size, "nullptr");
}
