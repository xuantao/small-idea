#include "SerUtility.h"

namespace serialize
{
    namespace utility
    {
        /*
         * read functions
        */
        bool Read(IReader* reader, bool& val, const char* name/* = nullptr*/)
        {
            return reader->Read(val, name);
        }
        bool Read(IReader* reader, int32_t& val, const char* name/* = nullptr*/)
        {
            return reader->Read(val, name);
        }
        bool Read(IReader* reader, uint32_t& val, const char* name/* = nullptr*/)
        {
            return reader->Read(val, name);
        }
        bool Read(IReader* reader, float& val, const char* name/* = nullptr*/)
        {
            return reader->Read(val, name);
        }
        bool Read(IReader* reader, std::string& val, const char* name/* = nullptr*/)
        {
            return reader->Read(val, name);
        }

        /*
         * write functions
        */
        bool Write(IWriter* writer, bool val, const char* name/* = nullptr*/)
        {
            return writer->Write(val, name);
        }
        bool Write(IWriter* writer, int32_t val, const char* name/* = nullptr*/)
        {
            return writer->Write(val, name);
        }
        bool Write(IWriter* writer, uint32_t val, const char* name/* = nullptr*/)
        {
            return writer->Write(val, name);
        }
        bool Write(IWriter* writer, float val, const char* name/* = nullptr*/)
        {
            return writer->Write(val, name);
        }
        bool Write(IWriter* writer, const std::string& val, const char* name/* = nullptr*/)
        {
            return writer->Write(val, name);
        }
    }
}
