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

	class font_controler : public IResource_Controler
	{
	public:
		struct font_info
		{
			std::string font_name;
			int size;
			int tick;
			int type;
		};
		//const charŒ^‚Ìƒ|ƒCƒ“ƒ^‚ðfont_infoŒ^‚É•ÏŠ·‚µ‚Ä‚­‚¾‚µ‚ 
		virtual int Load(const char* font_infop_fontdata) override
		{
			const font_info* info = (const font_info*) (font_infop_fontdata);
			return CreateFontToHandle( info->font_name.c_str(), info->size, info->tick, info->type);
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