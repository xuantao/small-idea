/*
 * 二进制序列化
*/

namespace CrossCall
{
    /*
     * 二进制数据Reader 
     */
    public class BinaryReader : Serialize.BinaryReader
    {
        public BinaryReader(System.IO.BinaryReader reader) : base(reader)
        {
        }

        public override bool StructBegin(int code, string name = null)
        {
            int check = 0;
            if (!Read(ref check, null))
                return false;

            return check == code;
        }
    }

    /*
     * 二进制数据Writer
     */
    public class BinaryWriter : Serialize.BinaryWriter
    {
        public BinaryWriter(System.IO.BinaryWriter writer) : base(writer)
        {
        }

        public override bool StructBegin(int code, string name = null)
        {
            return Write(code);
        }

        public override bool StructEnd()
        {
            return true;
        }
    }
}
