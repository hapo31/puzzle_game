#include"resource_manager.h"

using namespace res;

resource& Resource_mng::Regist(const char* filename, Loader file_loader, bool cache)
{
	auto t = res_map.emplace(filename, std::make_tuple(resource(filename, file_loader), cache));
	return get_resource(t.first->second);
}

bool Resource_mng::Unregist(const resource& resource_)
{
	for (auto it = res_map.begin(); it != res_map.end(); ++it)
	{
		if (get_resource(it->second).get_Handle() == resource_)
		{
			res_map.erase(it);
			return true;
		}
	}
	return false;
}