using System.Collections.Generic;

namespace cs_test
{

    class FixAr
    {
        public int this[int index]
        {
            get { return a[index]; }
            set { a[index] = value; }
        }

        public int[] a = new int[10];
    }

    //public class FixArray<T, int>
    //{
    //}

    static class Utility
    {
        public static void Read<T>(List<T> lst)
        {

        }

        public static void Read<T>(T[] arr)
        {

        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            int[] ar = new int[10];

        }
    }
}
