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
#ifdef _DEBUG
			OutputDebugStringA("Load\n");
#endif
			return LoadGraph(filename);
		}
		virtual bool Delete(int handle) override
		{
#ifdef _DEBUG
			OutputDebugStringA("Delete\n");
#endif
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

	class font_controler : public IResource_Controler
	{
	public:
		struct font_info
		{
			std::string font_name;
			int size;
			int tick;
			int type;
		} font;

		void set_fontinfo(const std::string& fontname, int size, int tick, int type)
		{
			font.font_name = fontname;
			font.size = size;
			font.tick = tick;
			font.type = type;
		}

		virtual int Load(const char* null) override
		{
			return CreateFontToHandle( font.font_name.c_str(), font.size, font.tick, font.type);
		}
		virtual bool Delete(int handle) override
		{
			return DeleteFontToHandle(handle) != -1;
		}
	};

	using graphic_controler_sh = std::shared_ptr<graphic_controler>;
	using sound_controler_sh = std::shared_ptr<sound_controler>;

	template<class Loader_Type>
	auto get_loader() -> decltype(std::make_shared<Loader_Type>()) { return std::make_shared<Loader_Type>(); }
}