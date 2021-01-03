#pragma once
#include <exception>
#include <string>

namespace chrono
{

class Exception : public std::exception
{
public:
	  Exception(const std::string& message);
	  virtual ~Exception() throw();
	  virtual const char* what() const throw();

private:
	  std::string message;

};

} //namespace chrono
