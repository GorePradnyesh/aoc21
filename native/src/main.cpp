#include <iostream>

#include "boost/program_options.hpp"


namespace
{
	void on_age(int age)
	{
	  std::cout << "On age: " << age << '\n';
	}
}

int main(int argc, const char *argv[])
{
	boost::program_options::options_description desc{"Options"};
	desc.add_options()
	  ("help,h", "Help screen")
	  ("pi", boost::program_options::value<float>()->default_value(3.14f), "Pi")
	  ("age", boost::program_options::value<int>()->notifier(on_age), "Age");

	boost::program_options::variables_map vm;
	boost::program_options::store(parse_command_line(argc, argv, desc), vm);
	notify(vm);
	
	if (vm.count("help"))
	{
		  std::cout << desc << '\n';
	}
    return 0;
}