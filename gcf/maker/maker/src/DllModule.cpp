#include "DllModule.h"
#include <string>
#include <Windows.h>

DllModule::DllModule()
{
}

DllModule::~DllModule()
{
    if (_module != nullptr)
        ::FreeLibrary((HMODULE)_module);
    _module = nullptr;
}

bool DllModule::Load(const char* name)
{
    if (name == nullptr)
        return false;

    std::string path(name);
    path += ".dll";

    _module = (void*) ::LoadLibraryA(path.c_str());
    if (_module == nullptr)
        printf("load library failed [%s]\n", path.c_str());

    return _module != nullptr;
}

DllModule::Func DllModule::GetProcImpl(const char* name)
{
    if (_module == nullptr || name == nullptr)
        return nullptr;

    return (Func)::GetProcAddress((HMODULE)_module, name);
}
