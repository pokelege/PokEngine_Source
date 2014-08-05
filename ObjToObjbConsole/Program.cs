using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ObjToObjbConsole
{
    class objbWriter
    {
        public string obj;
        private string[] lines;
        List<vertex3<float>> positions = new List<vertex3<float>>();
        List<vertex2<float>> uvs = new List<vertex2<float>>();
        List<vertex3<float>> normals = new List<vertex3<float>>();
        List<vertex3<int>> faces = new List<vertex3<int>>();
        List<vertexGroup<float>> verts = new List<vertexGroup<float>>();
        List<ushort> indices = new List<ushort>();

        int vIndex;
        int vtIndex;
        int vnIndex;
        int fIndex;
        class vertex3<T> : IEquatable<vertex3<T>>
        {
            public T x;
            public T y;
            public T z;

            public bool Equals(vertex3<T> other)
            {
                return (x.Equals(other.x) && y.Equals(other.y) && z.Equals(other.z));
            }
        }
        class vertex2<T> : IEquatable<vertex2<T>>
        {
            public T x;
            public T y;

            public bool Equals(vertex2<T> other)
            {
                return (x.Equals(other.x) && y.Equals(other.y));
            }
        }
        class vertexGroup<T> : IEquatable<vertexGroup<T>>
        {
            public vertex3<T> position;
            public vertex2<T> uv;
            public vertex3<T> normals;

            public bool Equals(vertexGroup<T> other)
            {
                return (position.Equals(other.position) && uv.Equals(other.uv) && normals.Equals(normals));
            }

            public T[] values()
            {
                T[] vals = new T[8];

                vals[0] = position.x;
                vals[1] = position.y;
                vals[2] = position.z;
                vals[3] = uv.x;
                vals[4] = uv.y;
                vals[5] = normals.x;
                vals[6] = normals.y;
                vals[7] = normals.z;
                return vals;
            }
        }
        public void writeObjb()
        {
            lines = File.ReadAllLines(obj);
            bool vFound = false;
            bool vtFound = false;
            bool vnFound = false;
            bool fFound = false;


            for (int i = 0; i < lines.Length; i++)
            {
                string[] t = lines[i].Split();

                switch (t[0])
                {
                    case ("v"):
                        {
                            if (!vFound)
                            {
                                vFound = true;
                                vIndex = i;
                            }
                            break;
                        }
                    case ("vt"):
                        {
                            if (!vtFound)
                            {
                                vtFound = true;
                                vtIndex = i;
                            }
                            break;
                        }
                    case ("vn"):
                        {
                            if (!vnFound)
                            {
                                vnFound = true;
                                vnIndex = i;
                            }
                            break;
                        }
                    case ("f"):
                        {
                            if (!fFound)
                            {
                                fFound = true;
                                fIndex = i;
                            }
                            break;
                        }
                }
            }

            processPositions();
            processUVs();
            processNormals();
            processFaces();
            processVerts();

            BinaryWriter toobjB = new BinaryWriter(File.Open(obj + "b", FileMode.Create));

            toobjB.Write(verts.Count);
            toobjB.Write(indices.Count);

            foreach (vertexGroup<float> v in verts)
            {
                foreach (float f in v.values())
                {
                    toobjB.Write(f);
                }
            }

            foreach (ushort s in indices)
            {
                toobjB.Write(s);
            }

            toobjB.Close();
        }

        private void processPositions()
        {
            for (int i = vIndex; i < lines.Length && i != vtIndex && i != vnIndex && i != fIndex; i++)
            {
                string[] t = lines[i].Split();
                if (t[0].Equals("v"))
                {
                    vertex3<float> temp = new vertex3<float>();
                    temp.x = float.Parse(t[1]);
                    temp.y = float.Parse(t[2]);
                    temp.z = float.Parse(t[3]);
                    positions.Add(temp);
                }
            }
        }

        private void processUVs()
        {
            for (int i = vtIndex; i < lines.Length && i != vIndex && i != vnIndex && i != fIndex; i++)
            {
                string[] t = lines[i].Split();
                if (t[0].Equals("vt"))
                {
                    vertex2<float> temp = new vertex2<float>();
                    temp.x = float.Parse(t[1]);
                    temp.y = float.Parse(t[2]);
                    uvs.Add(temp);
                }
            }
        }

        private void processNormals()
        {
            for (int i = vnIndex; i < lines.Length && i != vIndex && i != vtIndex && i != fIndex; i++)
            {
                string[] t = lines[i].Split();
                if (t[0].Equals("vn"))
                {
                    vertex3<float> temp = new vertex3<float>();
                    temp.x = float.Parse(t[1]);
                    temp.y = float.Parse(t[2]);
                    temp.z = float.Parse(t[3]);
                    normals.Add(temp);
                }
            }
        }

        private void processFaces()
        {
            char[] delimiter = { '/', ' ' };
            for (int i = fIndex; i < lines.Length && i != vIndex && i != vtIndex && i != vnIndex; i++)
            {
                string[] t = lines[i].Split(delimiter);
                if (t[0].Equals("f"))
                {
                    vertex3<int> temp = new vertex3<int>();
                    temp.x = int.Parse(t[1]) - 1;
                    temp.y = int.Parse(t[2]) - 1;
                    temp.z = int.Parse(t[3]) - 1;
                    faces.Add(temp);
                    vertex3<int> temp2 = new vertex3<int>();
                    temp2.x = int.Parse(t[4]) - 1;
                    temp2.y = int.Parse(t[5]) - 1;
                    temp2.z = int.Parse(t[6]) - 1;
                    faces.Add(temp2);
                    vertex3<int> temp3 = new vertex3<int>();
                    temp3.x = int.Parse(t[7]) - 1;
                    temp3.y = int.Parse(t[8]) - 1;
                    temp3.z = int.Parse(t[9]) - 1;
                    faces.Add(temp3);
                }
            }
        }

        private void processVerts()
        {
            for (int i = 0; i < faces.Count; i++)
            {
                vertex3<int> face = faces.ElementAt(i);
                vertexGroup<float> temp = new vertexGroup<float>();
                temp.position = positions.ElementAt(face.x);
                temp.uv = uvs.ElementAt(face.y);
                temp.normals = normals.ElementAt(face.z);

                int index = verts.IndexOf(temp);

                if (index == -1)
                {
                    indices.Add((ushort)verts.Count);
                    verts.Add(temp);
                }
                else
                {
                    indices.Add((ushort)index);
                }
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 1)
            {
                objbWriter writer = new objbWriter();
                writer.obj = args[0];
                writer.writeObjb();   
            }
        }
    }
}
