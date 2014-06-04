#pragma once
#include<DxLib.h>
#include"IResourceControl.h"

namespace res
{
	class graphic_controler : public IResource_Controler
	{
	public:
		virtual int Load(const char* filename) override
		{
			OutputDebugStringA("Load\n");
			return LoadGraph(filename);
		}
		virtual bool Delete(int handle) override
		{
			OutputDebugStringA("Delete\n");
			return DeleteGraph(handle) != -1;
		}
	};

	class sound_controler : public IResource_Controler
	{
	public:
		virtual int Load(const char* filename) override
		{
			return LoadSoundMem(filename);
		}
		virtual bool Delete(int handle) override
		{
			return DeleteSoundMem(handle) != -1;
		}
	};

	using graphic_controler_sh = std::shared_ptr<graphic_controler>;
	using sound_controler_sh = std::shared_ptr<sound_controler>;

	template<class Loader_Type>
	auto get_loader() -> decltype(std::make_shared<Loader_Type>()) { return std::make_shared<Loader_Type>(); }
}