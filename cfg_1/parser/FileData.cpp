#include "FileData.h"

CFG_NAMESPACE_BEGIN

namespace detail
{
    class FileBlock : public IBlock
    {
    public:
        FileBlock(BlockCategory block, const void* data)
            : _block(block), _data(data)
        {
        }
        ~FileBlock() {}

    public:
        BlockCategory Category() const { return _block; }

    public:
        BlockCategory _block;
        const void* _data;
    };
}

FileData::FileData(const std::string& file)
    : _file(file)
{
}

FileData::~FileData()
{
    std::for_each(_blocks.begin(), _blocks.end(),
        [ ](IBlock* block) {delete block; });
    _blocks.clear();
}

IBlock* FileData::Get(int index) const
{
    if (index < 0 || index >= Size())
        return nullptr;
    return _blocks[index];
}

void FileData::Traverse(IFileVisitor* visitor) const
{
    for (size_t i = 0; i < _blocks.size(); ++i)
    {
        detail::FileBlock* block = static_cast<detail::FileBlock*>(_blocks[i]);
        if (block->Category() == BlockCategory::Type)
            visitor->OnType(static_cast<const IType*>(block->_data));
        else if (block->Category() == BlockCategory::Var)
            visitor->OnVariate(static_cast<const IVariate*>(block->_data));
    }
}

void FileData::Add(const IType* type)
{
    _blocks.push_back(new detail::FileBlock(BlockCategory::Type, type));
}

void FileData::Add(const IVariate* var)
{
    _blocks.push_back(new detail::FileBlock(BlockCategory::Var, var));
}

CFG_NAMESPACE_END
