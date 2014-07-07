#include<DxLib.h>
#include<array>
#include<string>

#include"Controler_Manager.h"
#include"Level.h"

namespace level
{
	class Title : public ILevel
	{
		enum MENU
		{
			START, CONFIG, EXIT
		};
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