#ifndef _MODEL_RENDERER_H
#define _MODEL_RENDERER_H

#include "zero/Component.h"
#include <memory>

class Model;
struct Transform;
class Shader;

struct ModelRenderer : public zero::IBaseComponent
{
	void setup(std::shared_ptr<Model> model, std::shared_ptr<Transform> transform, std::shared_ptr<Shader> shader);
	void RenderMeshes();
	std::shared_ptr<Model> model_;
	std::weak_ptr<Shader> shader_;
	std::weak_ptr<Transform> transform_;
};

#endif