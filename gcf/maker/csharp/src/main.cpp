#include "CsExporter.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)
//extern "C"

GCF_NAMESPACE_USING;

CsExporter cppExp;

DLL_EXPORT IExporter* CreateExporter()
{
    return &cppExp;
}
