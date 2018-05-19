#include <algorithm>
#include <cstring>
#include <string>

using namespace std;

static void erase(int& argc, const char** argv, int index)
{
	for(int i=index; i < argc - 1; ++i)
	{
		argv[i] = argv[i + 1];
	}
	--argc;
}

static const char* find(int& argc, const char** argv, const char* item, const char* default_str=nullptr)
{
	const char* ret = default_str;
	if(item == nullptr)
		return ret;

	for(int i=1; i < argc; ++i)
	{
		if(strcmp(argv[i], item) == 0)
		{
			ret = argv[i];
			erase(argc, argv, i);
			break;
		}
	}
	return ret;
}

static const char* find_option(int& argc, const char** argv, const char* option, const char* default_str=nullptr)
{
	const char* ret = default_str;
	if(option == nullptr)
		return ret;

	for(int i=1; i < argc - 1; ++i)
	{
		if(strcmp(argv[i], option) == 0)
		{
			ret = argv[i + 1];
			erase(argc, argv, i + 1);
			erase(argc, argv, i);
			break;
		}
	}
	return ret;
}

