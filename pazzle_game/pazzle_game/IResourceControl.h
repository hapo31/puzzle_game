#pragma once

namespace res
{
	class IResource_Controler
	{
	public:
		virtual int Load(const char* filename) = 0;
		virtual bool Delete(int handle) = 0;
	};
}