using LitJson;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

public interface IJsonWalker
{
    void OnObjectBegin();
    void OnObjectEnd();

    void OnArrayBegin();
    void OnArrayEnd();

    void OnKey(string key);
    void OnIndex(int index);
    void OnValue(bool value);
    void OnValue(int value);
    void OnValue(long value);
    void OnValue(float value);
    void OnValue(string value);
}

public enum DeclearType
{
    Basic,
    Object,
    Array,
}

public interface IDeclear
{
    string Type { get; }
    string Key { get; }
    object Def { get; }
    DeclearType DeclearType { get; }
    int MemberCount { get; }
    IDeclear[] Members { get; }
}

internal class ObjectDeclear : IDeclear, IEquatable<ObjectDeclear>
{
    string _key;
    List<IDeclear> _members = new List<IDeclear>();

    public ObjectDeclear(string key) { _key = key; }

    public string Type { get; set; }
    public string Key { get { return _key; } }
    public object Def { get { return GetDef(); } }
    public DeclearType DeclearType { get { return DeclearType.Object; } }

    public int MemberCount { get { return _members.Count; } }
    public IDeclear[] Members { get { return _members.ToArray(); } }
    public bool IsBasic()
    {
        foreach (IDeclear mem in _members)
        {
            if (mem.DeclearType == DeclearType.Object)
                return false;
        }
        return true;
    }

    public void ClearEmpty()
    {
        for (int i = 0; i < _members.Count;)
        {
            if (string.IsNullOrEmpty(_members[i].Type))
                _members.RemoveAt(i);   // remove empty
            else
                ++i;
        }
    }

    public void Append(IDeclear type)
    {
        _members.Add(type);
    }

    public bool Equals(ObjectDeclear other)
    {
        if (other == null)
            return false;

        if (_members.Count != other._members.Count)
            return false;

        for (int i = 0; i < _members.Count; ++i)
        {
            if (_members[i].Type != other._members[i].Type)
                return false;
            if (_members[i].Key != other._members[i].Key)
                return false;
        }
        return true;
    }

    string GetDef()
    {
        if (_members.Count == 0)
            return "null";
        return string.Format("new {0}()", Type);
    }
}

internal class ArrayDeclear : IDeclear
{
    string _key;
    IDeclear _real = null;

    public ArrayDeclear(string key) { _key = key; }
    public string Type { get { return GetTypeName(); } }
    public string Key { get { return _key; } }
    public object Def { get { return GetDef(); } }
    public DeclearType DeclearType { get { return DeclearType.Array; } }

    public IDeclear Real { get { return _real; } }
    public int MemberCount { get {return 1; }  }
    public IDeclear[] Members { get { return new IDeclear[1] { _real }; } }

    public void Append(IDeclear type)
    {
        if (_real == null)
            _real = type;
        else if (_real.Type != type.Type || _real.DeclearType != type.DeclearType)
            throw new Exception(string.Format("array:{0} contain multy type:{1} deleartype:{2} elements", _key, type.Type, type.DeclearType));
        else if (_real.DeclearType == DeclearType.Object)
            Merge(_real as ObjectDeclear, type as ObjectDeclear);
    }

    void Merge(ObjectDeclear dst, ObjectDeclear src)
    {
        Dictionary<string, IDeclear> dstMembers = new Dictionary<string, IDeclear>();
        Dictionary<string, IDeclear> srcMembers = new Dictionary<string, IDeclear>();

        foreach (var mem in dst.Members)
            dstMembers.Add(mem.Key, mem);
        foreach (var mem in src.Members)
            srcMembers.Add(mem.Key, mem);

        // recursion merge object type
        foreach (var pair in dstMembers)
        {
            if (pair.Value.DeclearType == DeclearType.Object && srcMembers.ContainsKey(pair.Key))
                Merge(pair.Value as ObjectDeclear, srcMembers[pair.Key] as ObjectDeclear);
        }

        // add new types
        foreach (var pair in srcMembers)
        {
            if (!dstMembers.ContainsKey(pair.Key))
                dst.Append(pair.Value);
        }
    }

    Action<IDeclear> FnGetMembers(Dictionary<string, IDeclear> dic)
    {
        return delegate (IDeclear type) { dic.Add(type.Key, type); };
    }

    string GetTypeName()
    {
        if (_real == null || string.IsNullOrEmpty(_real.Type))
            return string.Empty;

        if (_real.DeclearType != DeclearType.Array)
            return _real.Type + "[]";

        string name = ((ArrayDeclear)_real).GetTypeName();
        if (string.IsNullOrEmpty(name))
            return string.Empty;

        return name + "[]";
    }

    string GetDef()
    {
        return "null";
    }
}

internal class GeneralDeclear : IDeclear
{
    string _type;
    string _key;
    object _def;
    public GeneralDeclear(string type, string key, object def)
    {
        _type = type;
        _key = key;
        _def = def;
    }

    public string Type { get { return _type; } }
    public string Key { get { return _key; } }
    public object Def { get { return _def; } }
    public DeclearType DeclearType { get { return DeclearType.Basic; } }
    public int MemberCount { get { return 0; } }
    public IDeclear[] Members { get { return new IDeclear[0]; } }
}

internal class DeclearWalker : IJsonWalker
{
    Stack<IDeclear> _stack = new Stack<IDeclear>();
    List<ObjectDeclear> _basicObjs = new List<ObjectDeclear>();
    List<ObjectDeclear> _ComplexObjs = new List<ObjectDeclear>();
    ObjectDeclear _dumy = new ObjectDeclear("");
    ObjectDeclear _root = null;
    string _key = "";

    public ObjectDeclear Root { get { return _root; } }

    public ObjectDeclear[] Report()
    {
        List<ObjectDeclear> namedTypes = new List<ObjectDeclear>();
        if (_root == null)
            return namedTypes.ToArray();

        _basicObjs.Sort(delegate (ObjectDeclear l, ObjectDeclear r) { return l.MemberCount.CompareTo(r.MemberCount); });
        _ComplexObjs.Sort(delegate (ObjectDeclear l, ObjectDeclear r) { return l.MemberCount.CompareTo(r.MemberCount); });

        for (int i = 0; i < _basicObjs.Count; ++i)
        {
            ObjectDeclear obj = _basicObjs[i];
            if (!string.IsNullOrEmpty(obj.Type))
                continue;

            obj.Type = string.Format("Type_{0}", namedTypes.Count + 1);
            namedTypes.Add(obj);
            for (int j = i + 1; j < _basicObjs.Count; ++j)
            {
                ObjectDeclear temp = _basicObjs[j];
                if (obj.Equals(temp))
                    temp.Type = obj.Type;
            }
        }

        for (int i = 0; i < _ComplexObjs.Count; ++i)
        {
            ObjectDeclear obj = _ComplexObjs[i];
            if (!string.IsNullOrEmpty(obj.Type))
                continue;

            obj.Type = string.Format("Type_{0}", namedTypes.Count + 1);
            namedTypes.Add(obj);
            for (int j = i + 1; j < _ComplexObjs.Count; ++j)
            {
                ObjectDeclear temp = _ComplexObjs[j];
                if (obj.Equals(temp))
                    temp.Type = obj.Type;
            }
        }

        for (int i = 0; i < _ComplexObjs.Count; ++i)
        {
            _ComplexObjs[i].ClearEmpty();
        }

        namedTypes.Remove(_root);
        return namedTypes.ToArray();
    }

    void Append(IDeclear type)
    {
        IDeclear top = _stack.Peek();
        if (top.DeclearType == DeclearType.Object)
            ((ObjectDeclear)top).Append(type);
        else if (top.DeclearType == DeclearType.Array)
            ((ArrayDeclear)top).Append(type);
    }

    public void OnObjectBegin()
    {
        ObjectDeclear type = new ObjectDeclear(_key);
        _stack.Push(type);
        if (_root == null)
            _root = type;
    }

    public void OnObjectEnd()
    {
        ObjectDeclear type = _stack.Pop() as ObjectDeclear;
        if (_stack.Count > 0)
            Append(type);

        if (type.IsBasic())
            _basicObjs.Add(type);
        else
            _ComplexObjs.Add(type);
    }

    public void OnArrayBegin()
    {
        _stack.Push(new ArrayDeclear(_key));
    }

    public void OnArrayEnd()
    {
        ArrayDeclear type = _stack.Pop() as ArrayDeclear;
        Append(type);

        if (type.Real == null)
        {
            type.Append(_dumy);

            if (!_basicObjs.Contains(_dumy))
                _basicObjs.Add(_dumy);
        }
    }

    public void OnIndex(int index) { }

    public void OnKey(string key)
    {
        _key = key;
    }

    public void OnValue(float value)
    {
        Append(new GeneralDeclear("float", _key, "0.0f"));
    }

    public void OnValue(string value)
    {
        Append(new GeneralDeclear("string", _key, "string.Empty"));
    }

    public void OnValue(long value)
    {
        Append(new GeneralDeclear("long", _key, "0"));
    }

    public void OnValue(int value)
    {
        Append(new GeneralDeclear("int", _key, "0"));
    }

    public void OnValue(bool value)
    {
        Append(new GeneralDeclear("bool", _key, "false"));
    }
}

public class JsonReport
{
    public IDeclear Root;
    public IDeclear[] Manuals;
}

public class ExportJson
{
    const string FileName = "const.json";

    public static JsonReport ExportJsonFile()
    {
        string jsonText = File.ReadAllText(FileName, Encoding.UTF8);
        if (string.IsNullOrEmpty(jsonText))
        {
            Console.WriteLine("can not load json text from file:{0}", FileName);
            return null;
        }

        JsonData data = JsonMapper.ToObject(jsonText);
        if (data == null)
        {
            Console.WriteLine("parse json data failed from file:{0}", FileName);
            return null;
        }

        DeclearWalker walker = new DeclearWalker();
        Traverse("", data, walker);

        JsonReport def = new JsonReport()
        {
            Root = walker.Root,
            Manuals = walker.Report()
        };
        return def;
    }

    static void Traverse(string name, JsonData jd, IJsonWalker walker)
    {
        if (jd == null)
            return;

        walker.OnKey(name);

        if (jd.IsObject)
        {
            walker.OnObjectBegin();
            foreach (string key in jd.Keys)
                Traverse(key, jd[key], walker);
            walker.OnObjectEnd();
        }
        else if (jd.IsArray)
        {
            walker.OnArrayBegin();
            for (int i = 0; i < jd.Count; ++i)
            {
                walker.OnIndex(i);
                Traverse(name, jd[i], walker);
            }
            walker.OnArrayEnd();
        }
        else if (jd.IsBoolean)
        {
            walker.OnValue((bool)jd);
        }
        else if (jd.IsInt)
        {
            walker.OnValue((int)jd);
        }
        else if (jd.IsLong)
        {
            walker.OnValue((long)jd);
        }
        else if (jd.IsDouble)
        {
            walker.OnValue((float)(double)jd);
        }
        else if (jd.IsString)
        {
            walker.OnValue((string)jd);
        }
    }
}
