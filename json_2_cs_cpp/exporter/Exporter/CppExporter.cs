using System.Collections.Generic;
using System.IO;

public class CppExporter
{
    class ArrayType
    {
        public IDeclear Declear;
        public string Type;

        public ArrayType(IDeclear declear, string type)
        {
            Declear = declear;
            Type = type;
        }
    }

    TextWriter _header;
    TextWriter _cpp;
    string _name;
    IDeclear _root;
    IDeclear[] _manuals;

    TextWriter _writer;

    Dictionary<string, string> _dicVector = new Dictionary<string, string>();
    
    List<ArrayType> _cppArrays = new List<ArrayType>();

    CppExporter(TextWriter header, TextWriter cpp, string name, JsonReport report)
    {
        _header = header;
        _cpp = cpp;
        _name = name;
        _root = report.Root;
        _manuals = report.Manuals;

        if (_manuals == null)
            _manuals = new IDeclear[0];
    }

    void ExportHeader()
    {
        _writer.WriteLine("/*");
        _writer.WriteLine(" * this file is auto generated, please do not edit it manual!");
        _writer.WriteLine(" * {0}", System.DateTime.Now);
        _writer.WriteLine("*/");
        _writer.WriteLine();

        _writer.WriteLine("#pragma once");
        _writer.WriteLine("#include <vector>");
        _writer.WriteLine("#include <string>");
        _writer.WriteLine();

        _writer.WriteLine("class {0}", _name);
        _writer.WriteLine("{");

        if (_manuals.Length > 0)
        {
            _writer.WriteLine("public:");

            foreach (var declear in _manuals)
            {
                WriteTab(1);
                _writer.WriteLine("struct {0}", declear.Type);
                WriteTab(1);
                _writer.WriteLine("{");

                if (declear.Members.Length > 0)
                {
                    WriteTab(2);
                    _writer.WriteLine("{0}()", declear.Type);
                    WriteConstructor(declear, 3);
                    WriteTab(2);
                    _writer.WriteLine("{}");
                    _writer.WriteLine();

                    DefineMember(declear, 2);
                }

                WriteTab(1);
                _writer.WriteLine("};");
                _writer.WriteLine();
            }
            //_writer.WriteLine();
        }

        // define constructor
        _writer.WriteLine("public:");
        WriteTab(1);
        _writer.WriteLine("{0}();", _name);

        // define loader member function
        _writer.WriteLine();
        _writer.WriteLine("public:");
        WriteTab(1);
        _writer.WriteLine("bool LoadJson(const char* lpJson, int nSize);");

        // define member data
        if (_root.Members.Length > 0)
        {
            _writer.WriteLine();
            _writer.WriteLine("public:");

            DefineMember(_root, 1);
        }

        _writer.WriteLine("};");
    }

    void DefineMember(IDeclear declear, int tab)
    {
        foreach (var mem in declear.Members)
        {
            WriteTab(tab);
            _writer.WriteLine("{0} {1};", GetTypeName(mem), mem.Key);
        }
    }

    void ExportCpp()
    {
        _writer.WriteLine("#include \"{0}.h\"", _name);
        _writer.WriteLine("#include <json/reader.h>");
        _writer.WriteLine();

        foreach (var declear in _manuals)
        {
            _writer.WriteLine("static bool sLoad(const Json::Value& node, {0}::{1}& ret);", _name, declear.Type);
        }

        foreach (var pair in _cppArrays)
        {
            _writer.WriteLine("static bool sLoad(const Json::Value& node, {0}& ret);", pair.Type);
        }

        foreach (var declear in _manuals)
        {
            _writer.WriteLine();
            WriteObjLoader(declear, 0);
        }

        foreach (var pair in _cppArrays)
        {
            _writer.WriteLine();
            WriteArrayLoader(pair.Declear, pair.Type, 0);
        }
        _writer.WriteLine();

        WriteLoader();
    }

    void WriteLoader()
    {
        _writer.WriteLine("{0}::{0}()", _name);
        WriteConstructor(_root, 1);
        _writer.WriteLine("{}");
        _writer.WriteLine();

        _writer.WriteLine("bool {0}::LoadJson(const char* lpJson, int nSize)", _name);
        _writer.WriteLine("{");

        WriteTab(1);
        _writer.WriteLine("if (lpJson == nullptr)");
        WriteTab(2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteTab(1);
        _writer.WriteLine("Json::Value root;");
        WriteTab(1);
        _writer.WriteLine("Json::Reader reader;");

        WriteTab(1);
        _writer.WriteLine("if (!reader.parse(std::string(lpJson, nSize), root, false))");
        WriteTab(2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteObjLoaderImpl(_root, "root", "", 1);

        WriteTab(1);
        _writer.WriteLine("return true;");
        _writer.WriteLine("}");
    }

    void WriteObjLoader(IDeclear declear, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("static bool sLoad(const Json::Value& node, {0}::{1}& ret)", _name, declear.Type);

        WriteTab(tab);
        _writer.WriteLine("{");
        if (declear.Members.Length == 0)
        {
            WriteTab(tab + 1);
            _writer.WriteLine("return true;");

            WriteTab(tab);
            _writer.WriteLine("}");
            return;
        }

        WriteObjLoaderImpl(declear, "node", "ret", tab + 1);

        _writer.WriteLine();
        WriteTab(tab + 1);
        _writer.WriteLine("return true;");

        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteObjLoaderImpl(IDeclear declear, string node, string ret, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("if ({0}.type() != Json::ValueType::objectValue)", node);
        WriteTab(tab + 1);
        _writer.WriteLine("return false;");

        if (!string.IsNullOrEmpty(ret))
            ret += ".";

        foreach (var mem in declear.Members)
        {
            _writer.WriteLine();
            WriteTab(tab);
            _writer.WriteLine("if ({0}.isMember(\"{1}\"))", node, mem.Key);
            WriteTab(tab);
            _writer.WriteLine("{");

            WriteTab(tab + 1);
            _writer.WriteLine("const Json::Value& mem = {0}[\"{1}\"];", node, mem.Key);

            if (mem.DeclearType == DeclearType.Basic)
                LoadBasic(mem, tab + 1, "mem", string.Format("{0}{1}", ret, mem.Key));
            else
                LoadObj(mem, tab + 1, "mem", string.Format("{0}{1}", ret, mem.Key));
            WriteTab(tab);

            _writer.WriteLine("}");
        }
    }

    void WriteArrayLoader(IDeclear declear, string type, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("static bool sLoad(const Json::Value& node, {0}& ret)", type);

        WriteTab(tab);
        _writer.WriteLine("{");

        WriteTab(tab + 1);
        _writer.WriteLine("if (node.type() != Json::ValueType::arrayValue)");
        WriteTab(tab + 2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteTab(tab + 1);
        _writer.WriteLine("ret.resize(node.size());");
        WriteTab(tab + 1);
        _writer.WriteLine("for (int i = 0; i < (int)node.size(); ++i)");
        WriteTab(tab + 1);
        _writer.WriteLine("{");

        IDeclear real = declear.Members[0];
        if (real.DeclearType == DeclearType.Basic)
        {
            WriteTab(tab + 2);
            _writer.WriteLine("const Json::Value& child = node[i];");
            LoadBasic(real, tab + 2, "child", "ret[i]");
        }
        else
        {
            LoadObj(real, tab + 2, "node[i]", "ret[i]");
        }

        WriteTab(tab + 1);
        _writer.WriteLine("}");

        _writer.WriteLine();
        WriteTab(tab + 1);
        _writer.WriteLine("return true;");

        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteConstructor(IDeclear declear, int tab)
    {
        bool bFirst = true;
        List<IDeclear> basicList = new List<IDeclear>();
        foreach (var mem in declear.Members)
        {
            if (mem.DeclearType != DeclearType.Basic || mem.Type == "string")
                continue;

            WriteTab(tab);
            if (bFirst)
            {
                bFirst = false;
                _writer.Write(": ");
            }
            else
            {
                _writer.Write(", ");
            }

            _writer.Write("{0}(", mem.Key);
            if (mem.Type == "int" || mem.Type == "long")
                _writer.Write("0");
            else if (mem.Type == "bool")
                _writer.Write("false");
            else if (mem.Type == "float")
                _writer.Write("0.0f");
            _writer.WriteLine(")");
        }
    }

    void LoadBasic(IDeclear declear, int tab, string node, string ret)
    {
        if (declear.Type == "int" || declear.Type == "long")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == Json::ValueType::intValue || {0}.type() == Json::ValueType::uintValue)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.asInt();", ret, node);
        }
        else if (declear.Type == "bool")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == Json::ValueType::booleanValue)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.asBool();", ret, node);
        }
        else if (declear.Type == "float")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == Json::ValueType::realValue)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = (float){1}.asDouble();", ret, node);
        }
        else if (declear.Type == "string")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == Json::ValueType::stringValue)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.asString();", ret, node);
        }
    }

    void LoadObj(IDeclear declear, int tab, string node, string ret)
    {
        WriteTab(tab);
        _writer.WriteLine("sLoad({0}, {1});", node, ret);
    }

    void AnalysisCppTypes()
    {
        foreach (var type in _manuals)
        {
            foreach (var mem in type.Members)
            {
                if (mem.DeclearType != DeclearType.Array)
                    continue;

                GenArrayTypes(mem);
            }
        }

        foreach (var mem in _root.Members)
        {
            if (mem.DeclearType != DeclearType.Array)
                continue;

            GenArrayTypes(mem);
        }

        _dicVector.Add("string", "std::string");
    }

    string GetTypeName(IDeclear declear)
    {
        string name = string.Empty;
        if (_dicVector.TryGetValue(declear.Type, out name))
            return name;
        return declear.Type;
    }

    bool IsBasic(IDeclear declear)
    {
        foreach (var mem in declear.Members)
        {
            if (mem.DeclearType == DeclearType.Object)
                return false;
        }
        return true;
    }

    void WriteTab(int tab)
    {
        _writer.Write(new string(' ', tab * 4));
    }

    void GenArrayTypes(IDeclear declear)
    {
        if (_dicVector.ContainsKey(declear.Type))
            return;

        string real = declear.Type.Substring(0, declear.Type.Length - 2);
        if (real.EndsWith("[]"))
        {
            GenArrayTypes(declear.Members[0]);
            _dicVector.Add(declear.Type, string.Format("std::vector<{0} >", _dicVector[real]));
            _cppArrays.Add(new ArrayType(declear, string.Format("std::vector<{0} >", _dicVector[real])));
        }
        else
        {
            
            _dicVector.Add(declear.Type, string.Format("std::vector<{0}>", real));
            if (declear.Members[0].DeclearType == DeclearType.Basic)
            {
                if (declear.Members[0].Type == "string")
                    _cppArrays.Add(new ArrayType(declear, string.Format("std::vector<std::string>")));
                else
                    _cppArrays.Add(new ArrayType(declear, string.Format("std::vector<{0}>",real)));
            }
            else
            {
                _cppArrays.Add(new ArrayType(declear, string.Format("std::vector<{0}::{1}>", _name, real)));
            }
        }
    }

    bool Export()
    {
        AnalysisCppTypes();

        _writer = _header;
        ExportHeader();

        _writer = _cpp;
        ExportCpp();
        return true;
    }

    public static bool Export(TextWriter header, TextWriter cpp, string className, JsonReport report)
    {
        if (report == null || report.Root == null)
            return false;

        CppExporter exportor = new CppExporter(header, cpp, className, report);
        return exportor.Export();
    }
}
