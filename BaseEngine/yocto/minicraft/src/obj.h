#pragma once

#include <fstream>
#include "engine/engine.h"

struct Mesh {
    vector<YVec3f> vertices;
	vector<pair<float, float>> uvs;
    vector<YVec3f> normals;
	vector<vector<YVec3<int>>> faces;
};

class MObj
{
private :
	int iVertice = 0;

public :
	Mesh mesh;
	MObj(){}
	void readObj(const char* filename) {
		ifstream in(filename, ios::in);
		if (!in)
		{
			cerr << "Cannot open " << filename << endl; exit(1);
		}
		string line;
		while (getline(in, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				istringstream s(line.substr(2));
				YVec3f vec = YVec3f(); s >> vec.X; s >> vec.Y; s >> vec.Z;
				mesh.vertices.push_back(vec);
			}
			else if (line.substr(0, 3) == "vn ") {
				istringstream s(line.substr(3));
				YVec3f nor = YVec3f(); s >> nor.X; s >> nor.Y; s >> nor.Z;
				mesh.normals.push_back(nor);
			}
			else if (line.substr(0, 3) == "vt ") {
				istringstream s(line.substr(3));
				float a, b; s >> a; s >> b;
				mesh.uvs.push_back(make_pair(a, b));
			}
			else if (line.substr(0, 2) == "f ") {
				istringstream s(line.substr(2));
				string str;
				string sNum;
				vector<YVec3<int>> face;
				while(getline(s, str, ' ')){
					istringstream sn(str);
					YVec3<int> vertex;
					getline(sn, sNum, '/');
					vertex.X = stoi(sNum) - 1;
					getline(sn, sNum, '/');
					vertex.Y = stoi(sNum) - 1;
					getline(sn, sNum, '/');
					vertex.Z = stoi(sNum) - 1;

					face.push_back(vertex);
				}
				mesh.faces.push_back(face);
			}
		}
	}
	
	void loadObjInVBO(YVbo* Vbo) {
		
		for (vector<YVec3<int>> face : mesh.faces)
		{
			for (int i = 0; i < 3; i++)
			{
				YVec3<int>& v = face[i];
				YVec3f& a = mesh.vertices[face[i].X];
				pair<float, float>& b = mesh.uvs[face[i].Y];
				YVec3f& c = mesh.normals[face[i].Z];

				Vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
				Vbo->setElementValue(1, iVertice, c.X, c.Y, c.Z);
				Vbo->setElementValue(2, iVertice, b.first, b.second);
				iVertice++;
			}
		}
	}
};