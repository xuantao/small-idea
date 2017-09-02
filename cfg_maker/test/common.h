#pragma once
#include <string>

namespace module
{
    class IReader
    {
    public:
        virtual ~IReader() {}
    public:
        virtual bool StructBegin(int hashID) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Read(int& n) = 0;
        virtual bool Read(float& f) = 0;
        virtual bool Read(bool& b) = 0;
        virtual bool Read(std::string& s) = 0;
    };

    class IWriter
    {
    public:
        virtual ~IWriter() {}
    public:
        virtual bool StructBegin(int hashID) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Write(int n) = 0;
        virtual bool Write(float f) = 0;
        virtual bool Write(bool b) = 0;
        virtual bool Write(const std::string& s) = 0;
    };

    class Exchanger
    {
    public:
        virtual ~Exchanger() {}

    public:
        virtual void Call(int module, int protocol) = 0;
    };

    class ICrossCaller
    {
    public:
        virtual ~ICrossCaller() {}
    public:
        virtual IWriter* BeginCall(int module) = 0;
        virtual IReader* EndCall() = 0;
    };

    struct TestStruct
    {
        static const int HASH_ID = 0;

        int id;
        int level;
        std::string name;
    };

    bool Read(IReader* reader, bool& n)
    {
        return reader->Read(n);
    }

    bool Read(IReader* reader, int& n)
    {
        return reader->Read(n);
    }

    bool Read(IReader* reader, float& n)
    {
        return reader->Read(n);
    }

    bool Read(IReader* reader, std::string& n)
    {
        return reader->Read(n);
    }

    template <class Ty>
    bool Read(IReader* reader, std::vector<Ty>& vec)
    {
        int size = 0;
        if (!Read(reader, size)) return false;
        if (size < 0) return false;

        vec.resize(size);

        for (int i = 0; i < size; ++i)
            if (!Read(reader, vec[i]))
                return false;

        return true;
    }

    template <class Ty, size_t N>
    bool Read(IReader* reader, std::array<Ty, N>& arr)
    {
        for (size_t i = 0; i < N; ++i)
            if (!Read(reader, arr[i]))
                return false;
        return true;
    }

    template <class Ky, class Ty>
    bool Read(IReader* reader, std::map<Ky, Ty>& map)
    {
        int size = 0;
        if (!Read(reader, size)) return false;
        if (size < 0) return false;

        for (size_t i = 0; i < N; ++i)
        {
            std::pair<Ky, Ty> pair;
            if (!Read(reader, pair.first)) return false;
            if (!Read(reader, pair.second)) return false;

            map.insert(std::move(pair));
        }
        return true;
    }

    bool Read(IReader* reader, TestStruct& data)
    {
        if (!reader->StructBegin(TestStruct::HASH_ID))
            return false;

        if (!Read(reader, data.id)) return false;
        if (!Read(reader, data.id)) return false;
        if (!Read(reader, data.id)) return false;
        if (!Read(reader, data.id)) return false;

        return reader->StructEnd();
    }

    class IProcessor
    {
    public:
        virtual ~IProcessor() {}

    public:
        virtual int GetModuleID() const = 0;
        virtual void Process(IReader* reader) = 0;
    };

    class Module
    {
    public:

        static const int MODULE_ID = 0;

        enum class Message
        {
            Invalid,
            Call_A,
            Call_B,
        };


        class IExecutor
        {
        public:
            virtual ~IExecutor() {}
        public:
            virtual void Call_A(int a, int b) = 0;
            virtual int Call_B(std::string&& str) = 0;
        };

        class Invoker
        {
        public:
            Invoker(ICrossCaller* caller);
        public:
            void Call_A(int a, int b)
            {
                IWriter* writer = _caller->BeginCall(MODULE_ID);
                writer->Write((int)Message::Call_A);
                writer->Write(a);
                writer->Write(b);

                _caller->EndCall();
            }

            int Call_B(const std::string& str)
            {
                int ret = 0;
                IWriter* writer = _caller->BeginCall(1);
                writer->Write((int)Message::Call_B);
                writer->Write(str);

                IReader* reader = _caller->EndCall();
                Read(reader, ret);
                // todo: need error check
                return ret;
            }

        protected:
            ICrossCaller* _caller;
        };

        class Processor : public IProcessor
        {
        public:
            Processor(IExecutor* executor) : _executor(executor)
            {
            }

        public:
            virtual int GetModuleID() const { return MODULE_ID; }

            virtual void Process(IReader* reader, IWriter* writer)
            {
                Message msg = Message::Invalid;
                if (!reader->Read((int&)msg))
                    return;

                switch (msg)
                {
                case Message::Call_A: Call_A(reader, writer); break;
                case Message::Call_B: Call_B(reader, writer); break;
                default:
                    break;
                }
            }
        protected:
            void Call_A(IReader* reader, IWriter* writer)
            {
                int a = 0;
                int b = 0;
                Read(reader, a);
                Read(reader, b);

                _executor->Call_A(a, b);
            }

            void Call_B(IReader* reader, IWriter* writer)
            {
                std::string str;
                Read(reader, str);

                //Write(writer, _executor->Call_B(std::move(str)));
            }
        protected:
            IExecutor* _executor = nullptr;
        };
    };
}
