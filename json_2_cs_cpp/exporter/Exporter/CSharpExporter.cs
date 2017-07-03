using System;
using System.IO;
using System.Text;

public class CSharpExporter
{
    string _name;
    TextWriter _writer;
    IDeclear _root;
    IDeclear[] _manuals;

    CSharpExporter(TextWriter writer, string name, JsonReport report)
    {
        _writer = writer;
        _name = name;
        _root = report.Root;
        _manuals = report.Manuals;
        if (_manuals == null)
            _manuals = new IDeclear[0];
    }

    void WriteTab(int tab)
    {
        _writer.Write(new string(' ', tab * 4));
    }

    void WriteMembers(IDeclear declear, int tab)
    {
        foreach (IDeclear mem in declear.Members)
        {
            WriteTab(tab);
            _writer.WriteLine("public {0} {1} = {2};", mem.Type, mem.Key, mem.Def);
        }
    }

    void WriteCsharpType(IDeclear declear, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("public class {0}", declear.Type);
        WriteTab(tab);
        _writer.WriteLine("{");

        if (declear.MemberCount > 0)
        {
            WriteMembers(declear, tab + 1);
            _writer.WriteLine();
        }

        WriteLoader(declear, tab + 1);

        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteLoadBasic(IDeclear member, string jd, int tab, string name = null)
    {
        if (string.IsNullOrEmpty(name))
            name = member.Key;

        WriteTab(tab);
        if (member.Type == "bool")
            _writer.WriteLine("if ({0}.IsBoolean)", jd);
        else if (member.Type == "int")
            _writer.WriteLine("if ({0}.IsInt)", jd);
        else if (member.Type == "long")
            _writer.WriteLine("if ({0}.IsLong)", jd);
        else if (member.Type == "float")
            _writer.WriteLine("if ({0}.IsDouble)", jd);
        else if (member.Type == "string")
            _writer.WriteLine("if ({0}.IsString)", jd);

        WriteTab(tab + 1);
        if (member.Type == "float")
            _writer.WriteLine("{0} = ({1})(double){2};", name, member.Type, jd);
        else
            _writer.WriteLine("{0} = ({1}){2};", name, member.Type, jd);
    }

    void WriteLoadArray(IDeclear member, string jd, int tab, int depth, string name = null)
    {
        if (string.IsNullOrEmpty(name))
            name = member.Key;

        string idx = "i";
        string jdL = "jdE";
        if (depth > 0)
        {
            idx = string.Format("i_{0}", depth);
            jdL = string.Format("jdE_{0}", depth);
        }

        string realType = member.Type.Substring(0, member.Type.IndexOf('['));
        string leftKung = member.Type.Substring(member.Type.IndexOf(']') + 1);
        string naL = string.Format("{0}[{1}]", name, idx);

        WriteTab(tab);
        _writer.WriteLine("if ({0}.IsArray)", jd);
        WriteTab(tab);
        _writer.WriteLine("{");

        WriteTab(tab + 1);
        _writer.WriteLine("{0} = new {1}[{2}.Count]{3};", name, realType, jd, leftKung);

        WriteTab(tab + 1);
        _writer.WriteLine("for (int {0} = 0; {1} < {2}.Count; ++{3})", idx, idx, jd, idx);
        WriteTab(tab + 1);
        _writer.WriteLine("{");

        WriteTab(tab + 2);
        _writer.WriteLine("JsonData {0} = {1}[{2}];", jdL, jd, idx);
        foreach (var mem in member.Members)
        {
            if (mem.DeclearType == DeclearType.Basic)
            {
                WriteLoadBasic(mem, jdL, tab + 2, naL);
            }
            else if (mem.DeclearType == DeclearType.Array)
            {
                WriteLoadArray(mem, jdL, tab + 2, depth + 1, naL);
            }
            else if (mem.DeclearType == DeclearType.Object)
            {
                WriteTab(tab + 2);
                _writer.WriteLine("{0} = new {1}();", naL, mem.Type);
                WriteLoadObj(mem, jdL, tab + 2, naL);
            }
        }

        WriteTab(tab + 1);
        _writer.WriteLine("}");
        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteLoadObj(IDeclear member, string jd, int tab, string name = null)
    {
        if (string.IsNullOrEmpty(name))
            name = member.Key;

        WriteTab(tab);
        _writer.WriteLine("if ({0}.IsObject)", jd);

        WriteTab(tab + 1);
        _writer.WriteLine("{0}.Load({1});", name, jd);
    }

    void WriteLoaderImpl(IDeclear declear, int tab)
    {
        if (declear.MemberCount == 0)
            return;

        WriteTab(tab);
        _writer.WriteLine("IDictionary dicJD = jsData as IDictionary;");

        foreach (var member in declear.Members)
        {
            WriteTab(tab);
            _writer.WriteLine("// load member {0}", member.Key);
            WriteTab(tab);
            _writer.WriteLine("if (dicJD.Contains(\"{0}\"))", member.Key);
            WriteTab(tab);
            _writer.WriteLine("{");
            WriteTab(tab + 1);
            _writer.WriteLine("JsonData jd = jsData[\"{0}\"];", member.Key);

            if (member.DeclearType == DeclearType.Basic)
                WriteLoadBasic(member, "jd", tab + 1);
            else if (member.DeclearType == DeclearType.Array)
                WriteLoadArray(member, "jd", tab + 1, 0);
            else if (member.DeclearType == DeclearType.Object)
                WriteLoadObj(member, "jd", tab + 1);

            WriteTab(tab);
            _writer.WriteLine("}");
        }
    }

    void WriteLoader(IDeclear declear, int tab)
    {
        WriteTab(tab);
        _writer.WriteLine("public bool Load(JsonData jsData)");
        WriteTab(tab);
        _writer.WriteLine("{");

        WriteTab(tab + 1);
        _writer.WriteLine("if (jsData == null)");
        WriteTab(tab + 2);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteLoaderImpl(declear, tab + 1);

        WriteTab(tab + 1);
        _writer.WriteLine("return true;");
        WriteTab(tab);
        _writer.WriteLine("}");
    }

    void WriteLoader()
    {
        WriteTab(1);
        _writer.WriteLine("public bool LoadJson(string context)");

        WriteTab(1);
        _writer.WriteLine("{");

        WriteTab(2);
        _writer.WriteLine("JsonData jsData = JsonMapper.ToObject(context);");
        WriteTab(2);
        _writer.WriteLine("if (jsData == null)");
        WriteTab(3);
        _writer.WriteLine("return false;");
        _writer.WriteLine();

        WriteLoaderImpl(_root, 2);
        _writer.WriteLine();

        WriteTab(2);
        _writer.WriteLine("return true;");

        WriteTab(1);
        _writer.WriteLine("}");
    }

    bool Export()
    {
        _writer.WriteLine("/*");
        _writer.WriteLine(" * this file is auto generated, please do not edit it manual!");
        _writer.WriteLine(" * {0}", System.DateTime.Now);
        _writer.WriteLine("*/");
        _writer.WriteLine();

        _writer.WriteLine("using LitJson;");
        _writer.WriteLine("using System;");
        _writer.WriteLine("using System.Collections;");
        _writer.WriteLine("using System.Collections.Generic;");
        _writer.WriteLine();

        _writer.WriteLine("public class {0}", _name);
        _writer.WriteLine("{");

        for (int i = 0; i < _manuals.Length; ++i)
        {
            WriteCsharpType(_manuals[i], 1);
            if (i + 1 != _manuals.Length)
                _writer.WriteLine();
        }

        if (_root.MemberCount > 0)
            _writer.WriteLine();
        WriteMembers(_root, 1);
        if (_root.MemberCount > 0)
            _writer.WriteLine();

        WriteLoader();

        _writer.WriteLine("}");
        _writer.WriteLine();

        return true;
    }

    public static bool Export(TextWriter writer, string name, JsonReport report)
    {
        if (report == null || report.Root == null)
            return false;

        CSharpExporter exportor = new CSharpExporter(writer, name, report);
        return exportor.Export();
    }
}