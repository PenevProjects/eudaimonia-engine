#include "ModelRenderer.h"
#include "Transform.h"
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"

void ModelRenderer::RenderMeshes(const Shader& _shader, const Transform& transform)
{
	_shader.setMat4("u_Model", transform.model_matrix());
	for (auto& mesh : model_->m_meshes)
	{
		mesh->Render(_shader);
	}
}
