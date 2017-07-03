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

                DefineMember(declear, 2);

                WriteTab(1);
                _writer.WriteLine("};");
                _writer.WriteLine();
            }
            //_writer.WriteLine();
        }

        // define constructer
        _writer.WriteLine("public:");
        WriteTab(1);
        _writer.WriteLine("{0}();", _name);
        //_writer.WriteLine("~{0}() {}", _name);

        // define loader member function
        _writer.WriteLine();
        _writer.WriteLine("public:");
        WriteTab(1);
        _writer.WriteLine("bool LoadJson(const char* lpData, int nSize);");

        // define member data
        if (_root.Members.Length > 0)
        {
            _writer.WriteLine();
            _writer.WriteLine("public:");

            DefineMember(_root, 1);
        }

        _writer.WriteLine("};");
        _writer.WriteLine();
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
        _writer.WriteLine("#include <libjson.h>");
        _writer.WriteLine();

        foreach (var declear in _manuals)
        {
            _writer.WriteLine("static bool sLoad(const JSONNode& node, {0}::{1}& ret);", _name, declear.Type);
        }

        foreach (var pair in _cppArrays)
        {
            _writer.WriteLine("static bool sLoad(const JSONNode& node, {1}& ret);", _name, pair.Type);
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

    }

    void WriteLoader()
    {
        _writer.WriteLine("bool {0}::LoadJson(const char* lpJson, int nSize)", _name);
        _writer.WriteLine("{");

        WriteTab(1);
        _writer.WriteLine("if (lpJson == null)");
        WriteTab(2);
        _writer.WriteLine("return false;");



        WriteTab(1);
        _writer.WriteLine("return true;");
        _writer.WriteLine("}");
    }

    void WriteObjLoader(IDeclear declear, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("static bool sLoad(const JSONNode& node, {0}::{1}& ret)", _name, declear.Type);

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

        WriteTab(tab + 1);
        _writer.WriteLine("if (node.type() != JSON_NODE)");
        WriteTab(tab + 2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteTab(tab + 1);
        _writer.WriteLine("for (JSONNode::const_iterator it = node.begin(); it != node.end(); ++it)");
        WriteTab(tab + 1);
        _writer.WriteLine("{");

        WriteTab(tab + 2);
        _writer.WriteLine("std::string node_name = it->name();");

        bool bFirst = true;
        foreach (var mem in declear.Members)
        {
            WriteTab(tab + 2);
            if (!bFirst)
                _writer.Write("else ");
            _writer.WriteLine("if (node_name == \"{0}\")", mem.Key);
            bFirst = false;

            WriteTab(tab + 2);
            _writer.WriteLine("{");
            if (mem.DeclearType == DeclearType.Basic)
                LoadBasic(mem, tab + 3, "(*it)", string.Format("ret.{0}", mem.Key));
            else
                LoadObj(mem, tab + 3, "(*it)", string.Format("ret.{0}", mem.Key));
            WriteTab(tab + 2);
            _writer.WriteLine("}");
        }

        WriteTab(tab + 1);
        _writer.WriteLine("}");

        _writer.WriteLine();
        WriteTab(tab + 1);
        _writer.WriteLine("return true;");

        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteArrayLoader(IDeclear declear, string type, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("static bool sLoad(const JSONNode& node, {0}& ret)", type);

        WriteTab(tab);
        _writer.WriteLine("{");

        WriteTab(tab + 1);
        _writer.WriteLine("if (node.type() != JSON_ARRAY)");
        WriteTab(tab + 2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteTab(tab + 1);
        _writer.WriteLine("for (int i = 0; i < node.size(); ++i)");
        WriteTab(tab + 1);
        _writer.WriteLine("{");

        IDeclear real = declear.Members[0];
        if (real.DeclearType == DeclearType.Basic)
            LoadBasic(real, tab + 2, "node[i]", "ret[i]");
        else
            LoadObj(real, tab + 2, "node[i]", "ret[i]");

        WriteTab(tab + 1);
        _writer.WriteLine("}");

        _writer.WriteLine();
        WriteTab(tab + 1);
        _writer.WriteLine("return true;");

        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void LoadBasic(IDeclear declear, int tab, string node, string ret)
    {
        if (declear.Type == "int" || declear.Type == "long")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == JSON_NUMBER)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.as_int();", ret, node);
        }
        else if (declear.Type == "bool")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == JSON_BOOL)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.as_bool();", ret, node);
        }
        else if (declear.Type == "float")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == JSON_NUMBER)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.as_float();", ret, node);
        }
        else if (declear.Type == "string")
        {
            WriteTab(tab);
            _writer.WriteLine("if ({0}.type() == JSON_STRING)", node);
            WriteTab(tab + 1);
            _writer.WriteLine("{0} = {1}.as_string();", ret, node);
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
            _cppArrays.Add(new ArrayType(declear, string.Format("std::vector<{0}::{1}>", _name, real)));
        }
    }

    bool Export()
    {
        AnalysisCppTypes();

        _writer = _header;
        //ExportHeader();

        _writer = _cpp;
        ExportCpp();
        return true;
    }

    public static bool Export(TextWriter header, TextWriter cpp, string name, JsonReport report)
    {
        if (report == null || report.Root == null)
            return false;

        CppExporter exportor = new CppExporter(header, cpp, name, report);
        return exportor.Export();
    }
}
