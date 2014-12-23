#include <string>

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

std::string greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(libhello)
{
    using namespace boost::python;
    def("greet", greet);
}
