#include "utility/weak_obj_array.h"

UTILITY_NAMESPACE_BEGIN

WeakObjIndex::~WeakObjIndex()
{
}

namespace weak_obj_internal
{

    bool WeakObjBase::IsValid() const
    {
        return true;
    }

    void* WeakObjBase::Get() const
    {
        return nullptr;
    }

    void WeakObjBase::Set(WeakObjIndex& obj_index, void* obj)
    {

    }
} // weak_obj_internal

UTILITY_NAMESPACE_END
