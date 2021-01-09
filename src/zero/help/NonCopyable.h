#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace zero
{
class NonCopyable 
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;


	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable &) = delete;
};
}//namespace zero
#endif
