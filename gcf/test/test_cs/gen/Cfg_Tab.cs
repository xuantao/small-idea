/*
 * this file is auto generated.
 * please does not edit it manual!
*/

using System;

namespace Tab
{
    public class Info
    {
        public string[] Titles;
        public string[] Types;
        public string[] Descs;
    }

    public static partial class Utility
    {


        public static Info GetInfo(TabTest_1 _)
        {
            return TabTest_1.TabInfo;
        }

        public static Info GetInfo(Sr3 _)
        {
            return Sr3.TabInfo;
        }

        public static Info GetInfo(object _)
        {
            throw new NotImplementedException();
        }
    }

} // namespace Tab
public partial class TabTest_1
{
    public static Tab.Info TabInfo = new Tab.Info
    {
        Titles = new string[] {
            "_bool", "_byte", "_int", "_long", "_float",
             "_double", "_string"
        },
        Types = new string[] {
            "bool", "byte", "int", "long", "float",
             "double", "string"
        },
        Descs = new string[] {
            "测试bool", "测试byte", "测试int", "测试long", "测试float",
             "测试double", "测试string"
        }
    };
}

public partial class Sr3
{
    public static Tab.Info TabInfo = new Tab.Info
    {
        Titles = new string[] {
            "ID", "StageID", "Name"
        },
        Types = new string[] {
            "int", "int", "string"
        },
        Descs = new string[] {
            "模板ID", "关卡ID", "名称"
        }
    };
}
