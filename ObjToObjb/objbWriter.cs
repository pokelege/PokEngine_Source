using System;
using System.Threading;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ObjToObjb
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

			public bool Equals( vertex3<T> other )
			{
				return ( x.Equals( other.x ) && y.Equals( other.y ) && z.Equals( other.z ) );
			}
		}
		class vertex2<T> : IEquatable<vertex2<T>>
		{
			public T x;
			public T y;

			public bool Equals( vertex2<T> other )
			{
				return ( x.Equals( other.x ) && y.Equals( other.y ) );
			}
		}
		class vertexGroup<T> : IEquatable<vertexGroup<T>>
		{
			public vertex3<T> position;
			public vertex2<T> uv;
			public vertex3<T> normals;
			public vertex3<T> tangent;
			public vertex3<T> bitangent;

			public bool Equals( vertexGroup<T> other )
			{
				return ( position.Equals( other.position ) && uv.Equals( other.uv ) && normals.Equals( other.normals ));// && tangent.Equals( other.tangent ) && bitangent.Equals( other.bitangent ) );
			}

			public T[] values()
			{
				T[] vals = new T[14];

				vals[0] = position.x;
				vals[1] = position.y;
				vals[2] = position.z;
				vals[3] = uv.x;
				vals[4] = uv.y;
				vals[5] = normals.x;
				vals[6] = normals.y;
				vals[7] = normals.z;
				vals[8] = tangent.x;
				vals[9] = tangent.y;
				vals[10] = tangent.z;
				vals[11] = bitangent.x;
				vals[12] = bitangent.y;
				vals[13] = bitangent.z;

				return vals;
			}
		}
		public void writeObjb()
		{
			lines = File.ReadAllLines( obj );
			bool vFound = false;
			bool vtFound = false;
			bool vnFound = false;
			bool fFound = false;


			for ( int i = 0; i < lines.Length; i++ )
			{
				string[] t = lines[i].Split();

				switch ( t[0] )
				{
					case ( "v" ):
						{
							if ( !vFound )
							{
								vFound = true;
								vIndex = i;
							}
							break;
						}
					case ( "vt" ):
						{
							if ( !vtFound )
							{
								vtFound = true;
								vtIndex = i;
							}
							break;
						}
					case ( "vn" ):
						{
							if ( !vnFound )
							{
								vnFound = true;
								vnIndex = i;
							}
							break;
						}
					case ( "f" ):
						{
							if ( !fFound )
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

			BinaryWriter toobjB = new BinaryWriter( File.Open( obj + "b", FileMode.Create ) );

			toobjB.Write( verts.Count );
			toobjB.Write( indices.Count );
			Console.WriteLine( "Verts:" + verts.Count );
			Console.WriteLine( "Index:" + indices.Count );
			foreach ( vertexGroup<float> v in verts )
			{
				foreach ( float f in v.values() )
				{
					toobjB.Write( f );
				}
			}

			foreach ( ushort s in indices )
			{
				toobjB.Write( s );
			}

			toobjB.Close();
		}

		private void processPositions()
		{
			for ( int i = vIndex; i < lines.Length && i != vtIndex && i != vnIndex && i != fIndex; i++ )
			{
				string[] t = lines[i].Split();
				if ( t[0].Equals( "v" ) )
				{
					vertex3<float> temp = new vertex3<float>();
					temp.x = float.Parse( t[1] );
					temp.y = float.Parse( t[2] );
					temp.z = float.Parse( t[3] );
					positions.Add( temp );
				}
			}
		}

		private void processUVs()
		{
			for ( int i = vtIndex; i < lines.Length && i != vIndex && i != vnIndex && i != fIndex; i++ )
			{
				string[] t = lines[i].Split();
				if ( t[0].Equals( "vt" ) )
				{
					vertex2<float> temp = new vertex2<float>();
					temp.x = float.Parse( t[1] );
					temp.y = float.Parse( t[2] );
					uvs.Add( temp );
				}
			}
		}

		private void processNormals()
		{
			for ( int i = vnIndex; i < lines.Length && i != vIndex && i != vtIndex && i != fIndex; i++ )
			{
				string[] t = lines[i].Split();
				if ( t[0].Equals( "vn" ) )
				{
					vertex3<float> temp = new vertex3<float>();
					temp.x = float.Parse( t[1] );
					temp.y = float.Parse( t[2] );
					temp.z = float.Parse( t[3] );
					normals.Add( temp );
				}
			}
		}

		private void processFaces()
		{
			char[] delimiter = { '/', ' ' };
			for ( int i = fIndex; i < lines.Length && i != vIndex && i != vtIndex && i != vnIndex; i++ )
			{
				string[] t = lines[i].Split( delimiter );
				if ( t[0].Equals( "f" ) )
				{
					vertex3<int> temp = new vertex3<int>();
					temp.x = int.Parse( t[1] ) - 1;
					temp.y = int.Parse( t[2] ) - 1;
					temp.z = int.Parse( t[3] ) - 1;
					faces.Add( temp );
					vertex3<int> temp2 = new vertex3<int>();
					temp2.x = int.Parse( t[4] ) - 1;
					temp2.y = int.Parse( t[5] ) - 1;
					temp2.z = int.Parse( t[6] ) - 1;
					faces.Add( temp2 );
					vertex3<int> temp3 = new vertex3<int>();
					temp3.x = int.Parse( t[7] ) - 1;
					temp3.y = int.Parse( t[8] ) - 1;
					temp3.z = int.Parse( t[9] ) - 1;
					faces.Add( temp3 );
				}
			}
		}

		private void processVerts()
		{
			for ( int i = 0; i < faces.Count / 3; i++ )
			{
				Console.WriteLine( "Index Max:" + ( from n in indices orderby n descending select n ).FirstOrDefault() );
				vertex3<int> face = faces.ElementAt( i * 3 );
				vertexGroup<float> temp = new vertexGroup<float>();
				temp.position = positions.ElementAt( face.x );
				temp.uv = uvs.ElementAt( face.y );
				temp.normals = normals.ElementAt( face.z );

				vertex3<int> face2 = faces.ElementAt( (i * 3) + 1 );
				vertexGroup<float> temp2 = new vertexGroup<float>();
				temp2.position = positions.ElementAt( face2.x );
				temp2.uv = uvs.ElementAt( face2.y );
				temp2.normals = normals.ElementAt( face2.z );

				vertex3<int> face3 = faces.ElementAt( ( i * 3 ) + 2 );
				vertexGroup<float> temp3 = new vertexGroup<float>();
				temp3.position = positions.ElementAt( face3.x );
				temp3.uv = uvs.ElementAt( face3.y );
				temp3.normals = normals.ElementAt( face3.z );

				vertex3<float> deltaPos1 = new vertex3<float>();
				deltaPos1.x = temp2.position.x
					- temp.position.x;
				deltaPos1.y = temp2.position.y
					- temp.position.y;
				deltaPos1.z = temp2.position.z
					- temp.position.z;

				vertex3<float> deltaPos2 = new vertex3<float>();
				deltaPos2.x = temp3.position.x
					- temp.position.x;
				deltaPos2.y = temp3.position.y
					- temp.position.y;
				deltaPos2.z = temp3.position.z
					- temp.position.z;

				vertex2<float> deltaUV1 = new vertex2<float>();
				deltaUV1.x = temp2.uv.x - temp.uv.x;
				deltaUV1.y = temp2.uv.y - temp.uv.y;

				vertex2<float> deltaUV2 = new vertex2<float>();
				deltaUV2.x = temp3.uv.x - temp.uv.x;
				deltaUV2.y = temp3.uv.y - temp.uv.y;

				float r = 1.0f / ( deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x );

				vertex3<float> deltaPos1timesdeltaUv2y = new vertex3<float>();
				deltaPos1timesdeltaUv2y.x = deltaPos1.x * deltaUV2.y;
				deltaPos1timesdeltaUv2y.y = deltaPos1.y * deltaUV2.y;
				deltaPos1timesdeltaUv2y.z = deltaPos1.z * deltaUV2.y;

				vertex3<float> deltaPos2timesDeltaUV1 = new vertex3<float>();
				deltaPos2timesDeltaUV1.x = deltaPos2.x * deltaUV1.y;
				deltaPos2timesDeltaUV1.y = deltaPos2.y * deltaUV1.y;
				deltaPos2timesDeltaUV1.z = deltaPos2.z * deltaUV1.y;

				vertex3<float> minus = new vertex3<float>();
				minus.x = deltaPos1timesdeltaUv2y.x - deltaPos2timesDeltaUV1.x;
				minus.y = deltaPos1timesdeltaUv2y.y - deltaPos2timesDeltaUV1.y;
				minus.z = deltaPos1timesdeltaUv2y.z - deltaPos2timesDeltaUV1.z;

				vertex3<float> tangent = new vertex3<float>();
				tangent.x = minus.x * r;
				tangent.y = minus.y * r;
				tangent.z = minus.z * r;

				vertex3<float> deltaPos2TimesDeltaUV1X = new vertex3<float>();
				deltaPos2TimesDeltaUV1X.x = deltaPos2.x * deltaUV1.x;
				deltaPos2TimesDeltaUV1X.y = deltaPos2.y * deltaUV1.x;
				deltaPos2TimesDeltaUV1X.z = deltaPos2.z * deltaUV1.x;

				vertex3<float> deltaPos1TimesDeltaUV2X = new vertex3<float>();
				deltaPos1TimesDeltaUV2X.x = deltaPos1.x * deltaUV2.x;
				deltaPos1TimesDeltaUV2X.y = deltaPos1.y * deltaUV2.x;
				deltaPos1TimesDeltaUV2X.z = deltaPos1.z * deltaUV2.x;

				vertex3<float> minus2 = new vertex3<float>();
				minus2.x = deltaPos2TimesDeltaUV1X.x - deltaPos1TimesDeltaUV2X.x;
				minus2.y = deltaPos2TimesDeltaUV1X.y - deltaPos1TimesDeltaUV2X.y;
				minus2.z = deltaPos2TimesDeltaUV1X.z - deltaPos1TimesDeltaUV2X.z;

				vertex3<float> bitangent = new vertex3<float>();
				bitangent.x = minus2.x * r;
				bitangent.y = minus2.y * r;
				bitangent.z = minus2.z * r;

				temp.tangent = tangent;
				temp.bitangent = bitangent;

				temp2.tangent = tangent;
				temp2.bitangent = bitangent;

				temp3.tangent = tangent;
				temp3.bitangent = bitangent;

				int index = verts.IndexOf( temp );

				if ( index == -1 )
				{
					indices.Add( ( ushort )verts.Count );
					verts.Add( temp );
				}
				else
				{
					indices.Add( ( ushort )index );
					verts[index].tangent.x += tangent.x;
					verts[index].tangent.y += tangent.y;
					verts[index].tangent.z += tangent.z;
					verts[index].bitangent.x += bitangent.x;
					verts[index].bitangent.y += bitangent.y;
					verts[index].bitangent.z += bitangent.z;
				}

				int index2 = verts.IndexOf( temp2 );

				if ( index2 == -1 )
				{
					indices.Add( ( ushort )verts.Count );
					verts.Add( temp2 );
				}
				else
				{
					indices.Add( ( ushort )index2 );
					verts[index2].tangent.x += tangent.x;
					verts[index2].tangent.y += tangent.y;
					verts[index2].tangent.z += tangent.z;
					verts[index2].bitangent.x += bitangent.x;
					verts[index2].bitangent.y += bitangent.y;
					verts[index2].bitangent.z += bitangent.z;
				}

				int index3 = verts.IndexOf( temp3 );

				if ( index3 == -1 )
				{
					indices.Add( ( ushort )verts.Count );
					verts.Add( temp3 );
				}
				else
				{
					indices.Add( ( ushort )index3 );
					verts[index3].tangent.x += tangent.x;
					verts[index3].tangent.y += tangent.y;
					verts[index3].tangent.z += tangent.z;
					verts[index3].bitangent.x += bitangent.x;
					verts[index3].bitangent.y += bitangent.y;
					verts[index3].bitangent.z += bitangent.z;
				}
			}
		}
	}
}
