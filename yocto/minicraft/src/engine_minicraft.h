#ifndef __YOCTO__ENGINE_TEST__
#define __YOCTO__ENGINE_TEST__

#include "engine/engine.h"
#include "engine/render/camera.h"
#include "engine/noise/perlin.h"

#include "avatar.h"
#include "world.h"
#include "chunk.h"
#include "cube.h"

class MEngineMinicraft : public YEngine {

public :

	YVbo * VboCube;
	
	int iVertice = 0;
	GLuint ShaderCubeDebug;
	GLuint ShaderCube;
	GLuint ShaderSun;
	GLuint ShaderWorld;

	YColor SunColor;
	YColor SkyColor;

	YVec3<float> SunPosition;
	YVec3<float> SunDirection;
	
	bool mouseButtonTabl[3];
	bool keyboardTabl[256];
	bool keyboardSpecialTabl[256];

	float elapsed = 0;
	bool BoostingTime = false;
	float time = 0;

	MAvatar * Avatar;
	
	MWorld * World;
	
	//Gestion singleton
	static YEngine * getInstance()
	{
		if (Instance == NULL)
			Instance = new MEngineMinicraft();
		return Instance;
	}

	/*HANDLERS GENERAUX*/
	void loadShaders() {
		
	}

	void init() 
	{
		YLog::log(YLog::ENGINE_INFO,"Minicraft Started : initialisation");

		// SunColor = new YColor(1,0,0,1);
		// SkyColor = new YColor(0, 0, 0, 1);
		SunColor = YColor(1.0f, 1.0f, 0.8f, 1.0f);
		SkyColor = YColor(0.0f, 181.f / 255.f, 221.f / 255.f, 1.0f);
		
		Renderer->setBackgroundColor(YColor(SkyColor.R, SkyColor.V, SkyColor.B, 1.0f));//YColor(0.0f,0.0f,0.0f,1.0f)
		// Renderer->Camera->setPosition(YVec3f(10, 10, 10));
		// Renderer->Camera->setLookAt(YVec3f());
		
		ShaderCubeDebug = Renderer->createProgram("shaders/cube_debug");
		ShaderCube = Renderer->createProgram("shaders/cube");
		ShaderSun = Renderer->createProgram("shaders/sun");
		ShaderWorld = Renderer->createProgram("shaders/world");

		//init sun
		SunPosition = { 0, 0, 0 };
		SunDirection = { 0, 0, 0 };

		// --Rendu du cube--
		//Création du monde
		World = new MWorld();
		World->init_world(0);

		Avatar = new MAvatar(Renderer->Camera, World);
		
		// --Rendu du cube--
		//Creation du VBO

		VboCube = new YVbo(3, 36, YVbo::PACK_BY_ELEMENT_TYPE);
		VboCube->setElementDescription(0, YVbo::Element(3)); //Sommet
		VboCube->setElementDescription(1, YVbo::Element(3)); //Normale
		VboCube->setElementDescription(2, YVbo::Element(2)); //UV

		VboCube->createVboCpu();

		//point
		YVec3f a{ 1, -1, -1 };
		YVec3f b{ 1, 1, -1 };
		YVec3f c{ 1, 1, 1 };
		YVec3f d{ 1, -1, 1 };
		
		YVec3f e{ -1, -1, -1 };
		YVec3f f{ -1, 1, -1 };
		YVec3f g{ -1, 1, 1 };
		YVec3f h{ -1, -1, 1 };
		
		//quad
		showQuad(a, b, c, d, VboCube);
		showQuad(e, h,g,f, VboCube);
		showQuad(c, g, h, d, VboCube);
		showQuad(b, a,e,f, VboCube);
		showQuad(b, f, g, c, VboCube);
		showQuad(a, d, h,e , VboCube);

		//On envoie le contenu au GPU
		VboCube->createVboGpu();

		//On relache la mémoire CPU
		VboCube->deleteVboCpu();

	}

	void update(float elapsed) 
	{
		this->elapsed += elapsed;
		// printf("%.6f \n", elapsed);
		updateLights(time);
		if (BoostingTime)
		{
			time += 0.5;
		}
		
		Avatar->update(elapsed);
		Avatar->Run = GetKeyState(VK_LSHIFT) & 0x80;
		Renderer->Camera->moveTo(Avatar->Position + YVec3f(0, 0, Avatar->CurrentHeight / 2));
	}

	void renderObjects() 
	{
		glUseProgram(0);
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
		
		glPushMatrix();

		// -- rendu d'un cube de terre --
		glUseProgram(ShaderCube);
		Renderer->updateMatricesFromOgl();
		Renderer->sendMatricesToShader(ShaderCube);
		GLuint var1 = glGetUniformLocation(ShaderCube, "cube_color");
		glUniform4f(var1, 40.0f / 255.0f, 25.0f / 255.0f, 0.0f, 1.0f);

		// -- rendu du monde --
		// World->render_world_basic(ShaderCube, VboCube);
		glUseProgram(ShaderWorld);
		// - envoi elapsed -
		// printf("elapsed : %.6f \n", elapsed);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendTimeToShader(elapsed, ShaderWorld); //Envoie elapsed au shader

		// - envoi LightPos au monde -
		// printf("LightPos : %.6f, %.6f, %.6f \n", 
		//	 SunPosition.X, SunPosition.Y, SunPosition.Z);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendLightPosToShader(SunPosition, ShaderWorld); //Envoie "light position" au shader

		// - envoi CamPos au monde -
		// printf("CamPos : %.6f, %.6f, %.6f \n", 
		//	 Renderer->Camera->Position.X, 
		//	 Renderer->Camera->Position.Y, 
		//	 Renderer->Camera->Position.Z);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendCamPosToShader(Renderer->Camera->Position, ShaderWorld); //Envoie "camera position" au shader
		
		// - envoi SunColor au monde -
		// printf("SunColor : %.6f, %.6f, %.6f, %.6f \n", SunColor.R, SunColor.V, SunColor.B, SunColor.A);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendSunColorToShader(SunColor, ShaderWorld); //Envoie "sun color" au shader

		World->render_world_vbo(false, true); //rendu VBO du World
		
		// -- rendu du soleil --
		glUseProgram(ShaderSun); //Demande au GPU de charger ces shaders
		GLuint var2 = glGetUniformLocation(ShaderSun, "sun_color");
		glUniform3f(var2, SunColor.R, SunColor.V, SunColor.B);
		glTranslatef(SunPosition.X, SunPosition.Y, SunPosition.Z);
		glScalef(10, 10, 10);
		Renderer->updateMatricesFromOgl(); //Calcule toute les matrices à partir des deux matrices OGL
		Renderer->sendMatricesToShader(ShaderSun); //Envoie les matrices au shader
		
		VboCube->render(); //Demande le rendu du VBO
		glPopMatrix();
	}

	void resize(int width, int height) {
	
	}

	/*INPUTS*/

	void keyPressed(int key, bool special, bool down, int p1, int p2) 
	{	
		// printf("%d : ", key);
		// printf(special ? "true : " : "false : ");
		// printf(down ? "true : " : "false : ");
		// printf("%d : ", p1);
		// printf("%d\n", p2);
		// if (!special)
		// {
		// 	keyboardTabl[key] = down;
		// }
		// else
		// {
		// 	keyboardSpecialTabl[key] = down;
		// }
		if (key == 'z')
			Avatar->avance = down;
		if (key == 's')
			Avatar->recule = down;
		if (key == 'q')
			Avatar->gauche = down;
		if (key == 'd')
			Avatar->droite = down;
		if (key == ' ')
			Avatar->Jump = down;
		if (key == 'g')
			// printf("%d : ", key);
			// printf(down ? "true \n" : "false \n");
			BoostingTime = down;
		if (key == 'e'&& !down) {
			int xC, yC, zC;
			YVec3f inter;
			World->getRayCollision(Renderer->Camera->Position,
				Renderer->Camera->Position + Renderer->Camera->Direction * 30,
				inter, xC, yC, zC);
			World->deleteCube(xC, yC, zC);
		}
	}

	void mouseWheel(int wheel, int dir, int x, int y, bool inUi)
	{
		// printf("%d : ", wheel);
		// printf("%d : ", dir);
		// printf("%d : ", x);
		// printf("%d : ", y);
		// printf(inUi ? "true\n" : "false\n");
		Renderer->Camera->move(Renderer->Camera->Direction * 10.0f * dir);
	}

	void mouseClick(int button, int state, int x, int y, bool inUi)
	{
		// printf("%d : ", button);
		// printf("%d : ", state);
		// printf("%d : ", x);
		// printf("%d : ", y);
		// printf(inUi ? "true\n" : "false\n");
		if (!inUi)
		{
			if (state != 1)
				mouseButtonTabl[button] = true;
			else
				mouseButtonTabl[button] = false;
		}
	}

	void mouseMove(int x, int y, bool pressed, bool inUi)
	{
		// printf("%d : ", x);
		// printf("%d : ", y);
		// printf(pressed ? "true : " : "false : ");
		// printf(inUi ? "true\n" : "false\n");

		static float lastx = 0;
		static float lasty = 0;
		
		if (!inUi)
		{
			if (pressed)
			{
				if(mouseButtonTabl[0])//ClicGauche
				{
					// printf("Clic gauche + la souris\n");
				}
				if (mouseButtonTabl[1])//ClicMollette
				{
					// printf("Clic mollette + la souris\n");
					if (lastx == -1 && lasty == -1)
					{
						lastx = x;
						lasty = y;
					}

					int dx = x - lastx;
					int dy = y - lasty;

					if (dx == 0 && dy == 0)
						return;

					lastx = x;
					lasty = y;
					
					if (MouseBtnState & GUI_MMBUTTON)
					{
						showMouse(false);
						if (GetKeyState(VK_LCONTROL) & 0x80)
						{
							YVec3f strafe = Renderer->Camera->RightVec;
							strafe.Z = 0;
							strafe.normalize();
							strafe *= (float)-dx / 2.0f;

							YVec3f avance = Renderer->Camera->Direction;
							avance.Z = 0;
							avance.normalize();
							avance *= (float)dy / 2.0f;

							Renderer->Camera->move(avance + strafe);
						}
						else {
							YVec3f strafe = Renderer->Camera->RightVec;
							strafe.Z = 0;
							strafe.normalize();
							strafe *= (float)-dx / 5.0f;

							Renderer->Camera->move(Renderer->Camera->UpRef * (float)dy / 5.0f);
							Renderer->Camera->move(strafe);
							glutWarpPointer(Renderer->ScreenWidth / 2, Renderer->ScreenHeight / 2);
							lastx = Renderer->ScreenWidth / 2;
							lasty = Renderer->ScreenHeight / 2;
						}
					}
				}
				if (mouseButtonTabl[2])//ClicDroit
				{
					// printf("Clic droit + la souris\n");
					if (lastx == -1 && lasty == -1)
					{
						lastx = x;
						lasty = y;
					}
			
					int dx = x - lastx;
					int dy = y - lasty;
			
					if (dx == 0 && dy == 0)
						return;
			
					lastx = x;
					lasty = y;
			
					if (MouseBtnState & GUI_MRBUTTON)
					{
						showMouse(false);
						if (GetKeyState(VK_LCONTROL) & 0x80)
						{
							Renderer->Camera->rotateAround((float)-dx / 300.0f);
							Renderer->Camera->rotateUpAround((float)-dy / 300.0f);
							glutWarpPointer(Renderer->ScreenWidth / 2, Renderer->ScreenHeight / 2);
							lastx = Renderer->ScreenWidth / 2;
							lasty = Renderer->ScreenHeight / 2;
						}
						else {
							showMouse(false);
							Renderer->Camera->rotate((float)-dx / 300.0f);
							Renderer->Camera->rotateUp((float)-dy / 300.0f);
							glutWarpPointer(Renderer->ScreenWidth / 2, Renderer->ScreenHeight / 2);
							lastx = Renderer->ScreenWidth / 2;
							lasty = Renderer->ScreenHeight / 2;
						}
					}
			
				}
			}
			else
			{
				lastx = x;
				lasty = y;
				showMouse(true);
			}
		}
	}

	void showQuad(YVec3f a, YVec3f b, YVec3f c, YVec3f d, YVbo * VboCube)
	{

		//calcul normal
		YVec3f aN{ c.X - d.X, c.Y - d.Y, c.Z - d.Z };
		YVec3f bN{ c.X - a.X, c.Y - a.Y, c.Z - a.Z };

		YVec3f normalV{
			sqrt((aN.Y * bN.Z - aN.Z * bN.Y) * (aN.Y * bN.Z - aN.Z * bN.Y)) ,
			sqrt((aN.Z * bN.X - aN.X * bN.Z) * (aN.Z * bN.X - aN.X * bN.Z)),
			sqrt((aN.X * bN.Y - aN.Y * bN.X) * (aN.X * bN.Y - aN.Y * bN.X)) };

		//triangle 1
		VboCube->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;
		VboCube->setElementValue(0, iVertice, b.X, b.Y, b.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;
		VboCube->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;

		//triangle 2
		VboCube->setElementValue(0, iVertice, c.X, c.Y, c.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;
		VboCube->setElementValue(0, iVertice, d.X, d.Y, d.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;
		VboCube->setElementValue(0, iVertice, a.X, a.Y, a.Z);
		VboCube->setElementValue(1, iVertice, normalV.X, normalV.Y, normalV.Z);
		VboCube->setElementValue(2, iVertice, 0, 0);
		iVertice++;
		
	}

	bool getSunDirFromDayTime(YVec3f & sunDir, float mnLever, float mnCoucher, float boostTime)
	{
		bool nuit = false;

		SYSTEMTIME t;
		GetLocalTime(&t);

		//On borne le tweak time à une journée (cyclique)
		while (boostTime > 24 * 60)
			boostTime -= 24 * 60;

		//Temps écoulé depuis le début de la journée
		float fTime = (float)(t.wHour * 60 + t.wMinute);
		fTime += boostTime;
		while (fTime > 24 * 60)
			fTime -= 24 * 60;

		//Si c'est la nuit
		if (fTime < mnLever || fTime > mnCoucher)
		{
			nuit = true;
			if (fTime < mnLever)
				fTime += 24 * 60;
			fTime -= mnCoucher;
			fTime /= (mnLever + 24 * 60 - mnCoucher);
			fTime *= (float)M_PI;
		}
		else
		{
			//c'est le jour
			nuit = false;
			fTime -= mnLever;
			fTime /= (mnCoucher - mnLever);
			fTime *= (float)M_PI;
		}

		//Direction du soleil en fonction de l'heure
		sunDir.X = cos(fTime);
		sunDir.Y = 0.2f;
		sunDir.Z = sin(fTime);
		sunDir.normalize();

		return nuit;
	}

	void updateLights(float boostTime = 0)
	{
		//// -- premier test de rotation autour de l'origine --
		// glRotatef(this->DeltaTimeCumul / 6.0f * 360, -7, -7, -7);
		// glTranslatef(0, -12,0);
		//glDisable(GL_CULL_FACE);
		
		//On recup la direciton du soleil
		bool nuit = getSunDirFromDayTime(SunDirection, 6.0f * 60.0f, 19.0f * 60.0f, boostTime);
		SunPosition = Renderer->Camera->Position + SunDirection * 500.0f;
	
		//Pendant la journée
		if (!nuit)
		{
			//On definit la couleur
			SunColor = YColor(1.0f, 1.0f, 0.8f, 1.0f);
			SkyColor = YColor(0.0f, 181.f / 255.f, 221.f / 255.f, 1.0f);
			YColor downColor(0.9f, 0.5f, 0.1f, 1);
	
			SunColor = SunColor.interpolate(downColor, (abs(SunDirection.X)));
			SkyColor = SkyColor.interpolate(downColor, (abs(SunDirection.X)));
		}
		else
		{
			//La nuit : lune blanche et ciel noir
			SunColor = YColor(1, 1, 1, 1);
			SkyColor = YColor(0, 0, 0, 1);
		}
	
		Renderer->setBackgroundColor(SkyColor);
	}

};


#endif