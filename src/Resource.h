#ifndef _RESOURCE_H
#define _RESOURCE_H

struct IResource
{
	IResource(std::string path) : directory_(path){}
	virtual ~IResource() {}
	IResource(const IResource&) = delete;
	IResource operator=(const IResource&) = delete;
	std::string directory_;
};

#endif