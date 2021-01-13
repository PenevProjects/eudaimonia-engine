#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include "zero/System.h"

struct IResource;

class ResourceSystem : public zero::IBaseSystem
{
	std::map<std::string, std::shared_ptr<IResource>> resources_;
public:
	template<typename T>
	std::shared_ptr<T> load(std::string _directory)
	{
		//if resource isnt loaded
		if (resources_.find(_directory) == resources_.end())
		{
			resources_.emplace(_directory, std::make_shared<T>(_directory));
			return std::dynamic_pointer_cast<T>(resources_.at(_directory));
		}
		else
		{
			return std::dynamic_pointer_cast<T>(resources_.at(_directory));
		}
	}
};

#endif