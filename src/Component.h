#ifndef _COMPONENT_H
#define _COMPONENT_H


class IBaseComponent
{
public:
	virtual ~IBaseComponent() = default;
	virtual void setup() {}
};

struct TransformComponent : public IBaseComponent
{
	int a_;
	int b_;
	void setup(int a, int b) {
		a_ = a;
		b_ = b;
	}

};

#endif