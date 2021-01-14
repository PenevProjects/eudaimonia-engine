#include "ModelRenderer.h"
#include "Transform.h"
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"


void ModelRenderer::setup(std::shared_ptr<Model> model, std::shared_ptr<Transform> transform, std::shared_ptr<Shader> shader)
{
	this->model_ = model;
	this->transform_ = transform;
	this->shader_ = shader;
}
void ModelRenderer::RenderMeshes()
{
	shader_.lock()->setMat4("u_Model", transform_.lock()->model_matrix());
	for (auto& mesh : model_->meshes_)
	{
		mesh->render(*shader_.lock());
	}
}


