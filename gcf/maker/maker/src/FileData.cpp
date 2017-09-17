#include "FileData.h"
#include "utility/Utility.h"

GCF_NAMESPACE_BEGIN

namespace detail
{
    enum class BlockType
    {
        Type,
        Var,
        Module,
        Include,
        NamespaceBegin,
        NamespaceEnd,
    };

    class FileBlock
    {
    public:
        FileBlock(BlockType type)
            : _type(type), _data(nullptr)
        {
        }

        FileBlock(BlockType type, const void* data)
            : _type(type), _data(data)
        {
        }

        FileBlock(BlockType type, const std::string& str)
            : _type(type), _data(nullptr), _str(str)
        {
        }

        ~FileBlock() {}

    public:
        BlockCategory Category() const { return BlockCategory::Var; }

    public:
        BlockType _type;
        const void* _data;
        const std::string _str;
    };
}

FileData::FileData(const std::string& file)
    : _file(file)
{
    utility::SplitPath(file, &_path);
}

FileData::~FileData()
{
    std::for_each(_blocks.begin(), _blocks.end(),
        [ ](detail::FileBlock* block) { delete block; });
    _blocks.clear();
}

void FileData::Export(IExporter* visitor, bool merge) const
{
    for (size_t i = 0; i < _blocks.size(); ++i)
    {
        detail::FileBlock* block = _blocks[i];
        switch (block->_type)
        {
        case detail::BlockType::Var:
            visitor->OnVariate(static_cast<const IVariate*>(block->_data));
            break;
        case detail::BlockType::Type:
            visitor->OnType(static_cast<const IType*>(block->_data));
            break;
        case detail::BlockType::Module:
            visitor->OnCrossCall(static_cast<const ICrossCall*>(block->_data));
            break;
        case detail::BlockType::Include:
            if (!merge)
                visitor->OnInclude(block->_str);
            break;
        case detail::BlockType::NamespaceBegin:
            visitor->OnNsBegin(block->_str);
            break;
        case detail::BlockType::NamespaceEnd:
            visitor->OnNsEnd();
            break;
        default:
            break;
        }
    }
}

void FileData::Add(const IType* type)
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::Type, type));
}

void FileData::Add(const IVariate* var)
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::Var, var));
}

void FileData::Add(const ICrossCall* var)
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::Module, var));
}

void FileData::Inlcude(const std::string& file)
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::Include, file));
}

void FileData::NsBegin(const std::string& name)
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::NamespaceBegin, name));
}

void FileData::NsEnd()
{
    _blocks.push_back(new detail::FileBlock(detail::BlockType::NamespaceEnd));
}
GCF_NAMESPACE_END
