#include<DxLib.h>
#include<array>
#include<string>

#include"Controler_Manager.h"
#include"Level.h"
#include"translater.h"

namespace level
{
	class Title : public ILevel
	{
		enum MENU
		{
			START, VS_START,  EXIT
		};
		struct color
		{
			int r = 0, g = 0, b = 0;
			color() = default;
			color(int r_, int g_, int b_) : r(r_), g(g_), b(b_){}
			int get_color() const { return GetColor(r, g, b); }
		};
		util::translater<color> background_color;

		const LEVEL_ID id = TITLE;
		int menu_select = 0;
		std::array<std::pair<std::string,LEVEL_ID>, 3> menu;
		std::array<res::resource_sh, 2> resdata;
		res::font_controler::font_info *info = nullptr;
		bool loading = false;
		bool end_flag = false;
		
	public:
		Title();
		virtual int execute(int Message) override;
		virtual int get_myid() const override{ return id; }
	
		virtual bool init(int Message) override;
		virtual bool end(int Message) override;

	};
}