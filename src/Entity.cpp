#include "Entity.h"

namespace chrono
{

template <typename T, typename ... Args>
std::shared_ptr<T> Entity::addComponent(Args&&... args)
{
	std::shared_ptr<T> temp = std::make_shared<T>();
	temp->entity = self;

	m_components.push_back(temp);

	temp->onInitialize(std::forward<Args>(args)...);

	return temp;
}

} //namespace chrono