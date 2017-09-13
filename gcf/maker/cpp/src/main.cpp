#include "CppExporter.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)
//extern "C"

GCF_NAMESPACE_USING;

CppExporter cppExp;

DLL_EXPORT IExporter* GetExporter()
{
    return &cppExp;
}
