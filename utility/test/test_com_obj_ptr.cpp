#include "utility/com_obj_ptr.h"

namespace
{
    struct IFile
    {
        void Release() { printf("IFile::Release count=%d\n", --count); }

        int count = 1;
    };

    struct IObj
    {
        void AddRef() { printf("IObj::AddRef  count=%d\n", ++count); }
        void Release() { printf("IObj::Release count=%d\n", --count); }

        int count = 1;
    };
}

void test_com_obj_ptr()
{
    IObj obj;
    IFile file;

    ComObjPtr<IObj> pObj0 = &obj;
    ComObjPtr<IObj> pObj1(pObj0);
    ComObjPtr<IObj> pObj2(std::move(pObj0));

    ComObjPtr<IObj> pObj3;
    pObj3 = &obj;
    pObj3 = std::move(pObj1);
    pObj3 = pObj2;

    UniqueObjPtr<IFile> pFile0 = &file;
    UniqueObjPtr<IFile> pFile1;
    pFile1 = &file;

    //pFile1 = pFile0;
    pFile1 = std::move(pFile0);

    //UniqueComObjPtr<IFile> pFile2 = pFile0;
    UniqueObjPtr<IFile> pFile3 = std::move(pFile0);

    if (pObj0 == nullptr) {}
    if (nullptr == pObj0) {}
    if (pObj0) {}
    if (pObj0 == pObj1) {}
    if (pObj0 != pObj1) {}

    if (pFile0 == nullptr) {}
    if (nullptr == pFile0) {}
    if (pFile0) {}
    if (pFile0 == pFile1) {}
    if (pFile0 != pFile1) {}

}
