#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include <fstream>

#include "engine/engine.h"

#include "avatar.h"
#include "world.h"

class MEngineMinicraft : public YEngine {

public :
	YVbo* VboCube;
	GLuint ShaderObj;
	
	MObj objMesh;

	YTexFile* tex;

	//Gestion singleton
	static YEngine * getInstance()
	{
		if (Instance == NULL)
			Instance = new MEngineMinicraft();
		return Instance;
	}

	/*HANDLERS GENERAUX*/
	void loadShaders() {
		ShaderObj = Renderer->createProgram("shaders/cube");
	}

	void init() 
	{
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");

		Renderer->setBackgroundColor(YColor(0.0f,0.0f,0.0f,1.0f));
		Renderer->Camera->setPosition(YVec3f(2, 2, 2));
		Renderer->Camera->setLookAt(YVec3f());

		tex = YTexManager::getInstance()->loadTexture("textures/grass/grass_block_top.png");

		objMesh.readObj("objs/suzanne.obj");//suzanne.obj//cube_simple.obj
		VboCube = new YVbo(3, objMesh.mesh.faces.size()*3, YVbo::PACK_BY_ELEMENT_TYPE);
		
		//Définition du contenu du VBO
		VboCube->setElementDescription(0, YVbo::Element(3)); //Sommet
		VboCube->setElementDescription(1, YVbo::Element(3)); //Normale
		VboCube->setElementDescription(2, YVbo::Element(2)); //UV
		
		//On demande d'allouer la mémoire coté CPU
		VboCube->createVboCpu();

		objMesh.loadObjInVBO(VboCube); 
		
		//fillVBOCube(VboCube);
		
		//On envoie le contenu au GPU
		VboCube->createVboGpu();

		//On relache la mémoire CPU
		VboCube->deleteVboCpu();

	}

	void fillVBOCube(YVbo* vbo, float size = 5.0f)
	{
		int iVertice = 0;

		YVec3f a(size / 2.0f, -size / 2.0f, -size / 2.0f);
		YVec3f b(size / 2.0f, size / 2.0f, -size / 2.0f);
		YVec3f c(size / 2.0f, size / 2.0f, size / 2.0f);
		YVec3f d(size / 2.0f, -size / 2.0f, size / 2.0f);
		YVec3f e(-size / 2.0f, -size / 2.0f, -size / 2.0f);
		YVec3f f(-size / 2.0f, size / 2.0f, -size / 2.0f);
		YVec3f g(-size / 2.0f, size / 2.0f, size / 2.0f);
		YVec3f h(-size / 2.0f, -size / 2.0f, size / 2.0f);

		iVertice += addQuadToVbo(vbo, iVertice, a, b, c, d); //x+
		iVertice += addQuadToVbo(vbo, iVertice, f, e, h, g); //x-
		iVertice += addQuadToVbo(vbo, iVertice, b, f, g, c); //y+
		iVertice += addQuadToVbo(vbo, iVertice, e, a, d, h); //y-
		iVertice += addQuadToVbo(vbo, iVertice, c, g, h, d); //z+
		iVertice += addQuadToVbo(vbo, iVertice, e, f, b, a); //z-
	}

	int addQuadToVbo(YVbo* vbo, int iVertice, YVec3f& a, YVec3f& b, YVec3f& c, YVec3f& d)
	{
		YVec3f normal = (b - a).cross(d - a);
		normal.normalize();

		vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 0);

		iVertice++;

		//vbo->setElementValue(0, iVertice, b.X, b.Y, b.Z);
		//vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		//vbo->setElementValue(2, iVertice, 1, 0);

		vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 1);

		iVertice++;

		//vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		//vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		//vbo->setElementValue(2, iVertice, 1, 1);

		vbo->setElementValue(0, iVertice, b.X, b.Y, b.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 0);

		iVertice++;

		vbo->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 0);

		iVertice++;

		//vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		//vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		//vbo->setElementValue(2, iVertice, 1, 1);

		vbo->setElementValue(0, iVertice, d.X, d.Y, d.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 0, 1);

		iVertice++;

		//vbo->setElementValue(0, iVertice, d.X, d.Y, d.Z);
		//vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		//vbo->setElementValue(2, iVertice, 0, 1);

		vbo->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		vbo->setElementValue(1, iVertice, normal.X, normal.Y, normal.Z);
		vbo->setElementValue(2, iVertice, 1, 1);

		iVertice++;

		return 6;
	}

	void update(float elapsed) 
	{
	}

	void renderObjects() 
	{
		glUseProgram(0);

#ifndef MODERNOGL
		//Rendu des axes
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(10000, 0, 0);
		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 10000, 0);
		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 10000);
		glEnd();

#endif // !MODERNOGL

		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		//Exemple d'utilisation d'un shader
		glUseProgram(ShaderObj); //Demande au GPU de charger ces shaders

		tex->setAsShaderInput(ShaderObj);
		
		Renderer->rotationModel(90, 1, -1, 0);
		Renderer->rotationModel(225, 0, 0, 1);
		Renderer->rotationModel(this->DeltaTimeCumul / 10.0f * 360, 0, 0, 1);

		//Renderer->rotationModel(this->DeltaTimeCumul / 10.0f * 360, -1, 1, 0);
		
		Renderer->updateMatrices(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderObj); //Envoie les matrices au shader


		VboCube->render(); //Demande le rendu du VBO

	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/

	void keyPressed(int key, bool special, bool down, int p1, int p2) 
	{	

	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		
	}

	void mouseClick(int button, int state, int x, int y, bool inUi)
	{
		
	}

	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		
	}
	
};


#endif