using System;

namespace cs_test
{

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("dir:{0}", System.IO.Directory.GetCurrentDirectory());

            var tab = new Tab.DataList<Sr3>();
            tab.LoadText(Sr3.TabInfo, Serialize.Utility.Read, "test.tab", 3, true);

            Console.ReadKey();
        }
    }
}
