#pragma once

#include "engine/utils/types_3d.h"

class YCamera
{
public:

	typedef enum {
		PROJECTION_ORTHO,
		PROJECTION_PERSPECTIVE
	} PROJTYPE;

	YVec3<float> Position; ///< Position de la camera
	YVec3<float> LookAt; ///< Point regarde par la camera
	YVec3<float> Direction; ///< Direction de la camera
	YVec3<float> UpVec; ///< Vecteur up de la camera
	YVec3<float> RightVec; ///< Si on se place dans la camera, indique la droite	
	YVec3<float> UpRef; ///< Ce qu'on consid�re comme le "haut" dans notre monde (et pas le up de la cam)
	

	PROJTYPE ProjType; ///< Type de projection
	float FovY; ///< fov angle sur y in degrees
	float Ratio; ///< with / height
	float Left; ///< Left YPlane
	float Right;
	float Bottom;
	float Top;
	float Near;
	float Far;
			
	YCamera()
	{
		Position = YVec3<float>(0, -1, 0);
		LookAt = YVec3<float>(0, 0, 0);
		UpRef = YVec3<float>(0, 0, 1);
		UpVec = UpRef;
		FovY = 45.0f;
		Ratio = 800.0f / 600.0f;
		Near = 0.1f;
		Far = 800.0f;
		setProjectionPerspective(FovY, Ratio, Near, Far);

		updateVecs();
	}

	void setProjectionPerspective(float fovy, float ratio, float nearPlane, float farPlane) {
		ProjType = PROJECTION_PERSPECTIVE;
		FovY = fovy;
		Ratio = ratio;
		Near = nearPlane;
		Far = farPlane;
	}

	void setProjectionOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		ProjType = PROJECTION_ORTHO;
		Left = left;
		Right = right;
		Bottom = bottom;
		Top = top;
		Near = nearPlane;
		Far = farPlane;
	}


	/**
	* Mise a jour de la camera
	*/
	virtual void update(float elapsed)
	{

	}

	/**
	* Definition du point regarde
	*/
	void setLookAt(YVec3<float> lookat)
	{
		LookAt = lookat;
		updateVecs();
	}

	/**
	* Definition de la position de la camera
	*/
	void setPosition(YVec3<float> pos)
	{
		Position = pos;
		updateVecs();
	}

	/**
	* Definition du haut de notre monde
	*/
	void setUpRef(YVec3<float> upRef)
	{
		UpRef = upRef;
		updateVecs();
	}

	/**
	* Deplacement de la camera d'un delta donn�
	*/
	void move(YVec3<float> delta)
	{
		
	}

	/**
	* Deplacement de la camera � un point donn�
	*/
	void moveTo(const YVec3<float> & target)
	{
		
	}

	/**
	* On recalcule les vecteurs utiles au d�placement de la camera (Direction, RightVec, UpVec)
	* on part du principe que sont connus :
	* - la position de la camera
	* - le point regarde par la camera
	* - la vecteur up de notre monde
	*/
	void updateVecs(void)
	{
		
	}

	/**
	* Rotation droite gauche en subjectif
	*/
	void rotate(float angle)
	{
		
	}

	/**
	* Rotation haut bas en subjectif
	*/
	void rotateUp(float angle)
	{
		
	}

	/**
	* Rotation droite gauche en troisi�me personne
	*/
	void rotateAround(float angle)
	{
		
	}

	/**
	* Rotation haut bas en troisi�me personne
	*/
	void rotateUpAround(float angle)
	{
		
	}

	/**
	* Calcul du bon rep�re de d�part pour la matrice monde
	*/
	void look(void);
};
