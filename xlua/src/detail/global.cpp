#include "global.h"
#include "type_desc.h"
#include "../xlua_export.h"
#include <memory>
#include <algorithm>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    static int s_version = 0;
    static NodeBase* s_node_head = nullptr;
    static GlobalVar* s_global = nullptr;
}

ObjIndex::~ObjIndex()
{
    if (index_ != -1 && detail::s_global)
        detail::s_global->FreeObjIndex(index_);
}

namespace detail
{
    static bool StateCmp(const std::pair<lua_State*, xLuaState*>& l, const std::pair<lua_State*, xLuaState*>& r) {
        return l.first < r.first;
    }

    NodeBase::NodeBase(NodeType type) : type_(type)
    {
        next_ = s_node_head;
        s_node_head = this;
    }

    NodeBase::~NodeBase()
    {
        NodeBase* node = s_node_head;
        while (node != this && node->next_ != this)
            node = node->next_;

        if (node == this)
            s_node_head = next_;
        else
            node->next_ = next_;
        next_ = nullptr;
    }

    bool GlobalVar::Startup()
    {
        if (s_global)
            return false;

        s_global = new GlobalVar(++s_version);

        /* 初始化静态数据 */
        NodeBase* node = s_node_head;
        while (node) {
            switch (node->type_) {
            case NodeType::kType:
                static_cast<TypeNode*>(node)->func_();
                break;
            case NodeType::kConst:
                s_global->const_infos_.push_back(static_cast<ConstNode*>(node)->func_());
                break;
            case NodeType::kScript:
                s_global->scripts_.push_back(static_cast<ScriptNode*>(node)->script_);
                break;
            default:
                break;
            }

            node = node->next_;
        }
        return true;
    }

    GlobalVar* GlobalVar::GetInstance()
    {
        return s_global;
    }

    void GlobalVar::Purge()
    {
        delete s_global;
        s_global = nullptr;
    }

    GlobalVar::GlobalVar(int version) : version_(version)
    {
        types_.reserve(256);
        types_.push_back(nullptr);

        external_types_.reserve(256);
        external_types_.push_back(nullptr);
    }

    GlobalVar::~GlobalVar()
    {
    }

    xLuaState* GlobalVar::Create() {
        lua_State* l = luaL_newstate();
        xLuaState* xl = new xLuaState(l, false);
        xl->InitEnv();
        xl->AddTypes(types_);
        xl->AddConsts(const_infos_);
        xl->AddScritps(scripts_);

        states_.push_back(std::make_pair(l, xl));
        std::sort(states_.begin(), states_.end(), &StateCmp);
        return xl;
    }

    xLuaState* GlobalVar::Attach(lua_State* l) {
        return nullptr;
    }

    void GlobalVar::Destory(xLuaState* l) {

    }

    xLuaState* GlobalVar::GetState(lua_State* l) const {
        auto first = states_.cbegin();
        auto last = states_.cend();
        first = std::lower_bound(first, last, std::make_pair(l, (xLuaState*)nullptr), &StateCmp);
        if (first == last || first->first != l)
            return nullptr;
        return first->second;
    }

    ITypeDesc* GlobalVar::AllocType(TypeCategory category,
        bool is_weak_obj,
        const char* name,
        const TypeInfo* super) {
        return new TypeDesc(s_global,category, is_weak_obj , name, super);
    }

    const TypeInfo* GlobalVar::GetExternalTypeInfo(int index) const {
        assert(index < (int)types_.size());
        if (index >= (int)types_.size())
            return nullptr;
        return types_[index];
    }

    int GlobalVar::AllocObjIndex(ObjIndex& obj_index, void* obj, const TypeInfo* info) {
        if (obj_index.index_ != -1) {
            // 总是存储子类指针
            auto& ary_obj = obj_array_[obj_index.index_];
            if (!IsBaseOf(info, ary_obj.info_)) {
                ary_obj.obj_ = obj;
                ary_obj.info_ = info;
            }
            return obj_index.index_;
        } else {
            // 无可用元素，增加数组容量
            if (free_index_.empty()) {
                size_t old_size = obj_array_.size();
                size_t new_size = obj_array_.size() + 1024;
                obj_array_.resize(new_size, ArrayObj{0, nullptr, nullptr});

                if (free_index_.capacity() < 1024)
                    free_index_.reserve(1024);

                for (size_t i = new_size; i > old_size; --i)
                    free_index_.push_back((int)i - 1);
            }

            auto index = free_index_.back();
            auto& ary_obj = obj_array_[index];
            free_index_.pop_back();

            ary_obj.info_ = info;
            ary_obj.obj_ = obj;
            ary_obj.serial_num_ = ++serial_num_gener_;

            obj_index.index_ = index;
            return index;
        }
    }

    ArrayObj* GlobalVar::AllocLuaObj(ObjIndex& obj_index, void* obj, const TypeInfo* info)
    {
        int idx = AllocObjIndex(obj_index, obj, info);
        return &obj_array_[idx];
    }

    ArrayObj* GlobalVar::GetArrayObj(int index) {
        if (index < 0 || index >= (int)obj_array_.size())
            return nullptr;
        return &obj_array_[index];
    }

    void GlobalVar::FreeObjIndex(int index)
    {
        auto& ary_obj = obj_array_[index];
        ary_obj.info_ = nullptr;
        ary_obj.obj_ = nullptr;
        ary_obj.serial_num_ = 0;

        if ((free_index_.capacity() - free_index_.size()) == 0)
            free_index_.reserve(free_index_.size() + 1024);
        free_index_.push_back(index);
    }

    void GlobalVar::AddTypeInfo(TypeInfo* info)
    {
        assert(types_.size() < 0xff);
        info->index = (int)types_.size();
        types_.push_back(info);

        if (info->category == TypeCategory::kExternal) {
            info->external_type_index = (int8_t)external_types_.size();
            external_types_.push_back(info);
        }
    }

    void* GlobalVar::SerialAlloc(size_t size)
    {
        //TODO: 需要优化
        return new int8_t[size];
    }
} // namespace detail

XLUA_NAMESPACE_END
