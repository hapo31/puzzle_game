#include<DxLib.h>
#include<array>
#include<string>

#include"Controler_Manager.h"
#include"Level.h"

namespace level
{
	class Title : public ILevel
	{
		const LEVEL_ID id = TITLE;
		int menu_select = 0;
		std::array<std::pair<std::string,LEVEL_ID>, 3> menu;
		std::array<res::resource, 2> resdata;
		res::font_controler::font_info *info = nullptr;
		bool loading = false;

		int bright = 0;
		
	public:
		Title();
		virtual int execute() override;
		virtual int get_myid() const override{ return id; }
	
		virtual bool init() override;
		virtual bool end() override;

	};
}