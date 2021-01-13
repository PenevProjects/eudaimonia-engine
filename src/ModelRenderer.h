#ifndef _MODEL_RENDERER_H
#define _MODEL_RENDERER_H

#include "zero/Component.h"
#include <memory>

class Model;
struct Transform;
class Shader;

struct ModelRenderer : public zero::IBaseComponent
{
	void setup(std::shared_ptr<Model> model) { this->model_ = model; }
	void RenderMeshes(const Shader& _shader, const Transform& transform);
	std::shared_ptr<Model> model_;
};

#endif