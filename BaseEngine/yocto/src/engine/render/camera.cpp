#include "engine/render/camera.h"
#include "engine/render/renderer.h"

void YCamera::look(void)
{
	YRenderer* renderer = YRenderer::getInstance();
	if (ProjType == PROJECTION_PERSPECTIVE) {
		renderer->setProjectionPersp(FovY, Ratio, Near, Far);
	}
	if (ProjType == PROJECTION_ORTHO) {
		renderer->setProjectionOrtho(Left, Right, Bottom, Top, Near, Far);
	}
	renderer->resetModel();
	renderer->setViewMatix(Position, LookAt, UpVec);
}
