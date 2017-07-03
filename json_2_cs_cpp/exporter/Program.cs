﻿using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace test_cs_idea
{
    class Program
    {
        static void DoExport()
        {
            JsonReport report = ExportJson.ExportJsonFile();
            if (report == null)
                return;

            StringBuilder builder = new StringBuilder();
            StringWriter writer = new StringWriter(builder);

            CSharpExporter.Export(writer, "Config", report);
            File.WriteAllText("Config.cs", builder.ToString(), Encoding.UTF8);

            CSharpExporter.Export(Console.Out, "Config", report);
        }

        static void DoLoad()
        {
            string fileName = "const.json";
            string jsonText = File.ReadAllText(fileName, Encoding.UTF8);

            //Config cfg = new Config();
            //cfg.LoadJson(jsonText);
        }

        static void DoCppExport()
        {
            JsonReport report = ExportJson.ExportJsonFile();
            if (report == null)
                return;

            CppExporter.Export(Console.Out, Console.Out, "Config", report);
        }

        static void Main(string[] args)
        {
            //DoExport();
            //DoLoad();

            DoCppExport();

            Console.Write("press any key to continue");
            Console.ReadKey(true);
        }
    }
}