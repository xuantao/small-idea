#include "Serialize.h"

namespace serialize
{
    namespace utility
    {
        /*
         * read functions
        */
        bool Read(IReader* reader, bool& val)
        { return reader->Read(val); }
        bool Read(IReader* reader, int32_t& val)
        { return reader->Read(val); }
        bool Read(IReader* reader, uint32_t& val)
        { return reader->Read(val); }
        bool Read(IReader* reader, float& val)
        { return reader->Read(val); }
        bool Read(IReader* reader, std::string& val)
        { return reader->Read(val); }

        /*
         * write functions
        */
        bool Write(IWriter* writer, bool val)
        { return writer->Write(val); }
        bool Write(IWriter* writer, int32_t val)
        { return writer->Write(val); }
        bool Write(IWriter* writer, uint32_t val)
        { return writer->Write(val); }
        bool Write(IWriter* writer, float val)
        { return writer->Write(val); }
        bool Write(IWriter* writer, const std::string& val)
        { return writer->Write(val); }
    }
}