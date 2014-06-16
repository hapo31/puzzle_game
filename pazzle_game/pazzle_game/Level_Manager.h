#pragma once
#include<list>
#include<unordered_map>
#include<memory>

#include"Level.h"

namespace level
{
	using level_t = std::shared_ptr < ILevel > ;

	//ƒQ[ƒ€‚ÌƒV[ƒ“(Level)‘JˆÚ‚ğŠÇ—‚·‚é
	class Level_Manager
	{
		std::unordered_map<int, level_t> levels;
		std::list <level_t> level_stack;
		int next_id = 0;
		Level_Manager() = default;
		Level_Manager(const Level_Manager&) = delete;
		Level_Manager& operator=(const Level_Manager&) = delete;
		enum STATE{ INIT, EXE, END, END_BACK, NEXT } state = INIT;
		int frame = 0;
	public:
		static Level_Manager* get_Instance()
		{
			static Level_Manager obj;
			return &obj;
		}

		//Ÿ‚ÌLevel‚Ö‘JˆÚ
		//‘JˆÚ‚ª”­¶‚µ‚Ä‚¢‚ê‚Îtrue
		bool set_next_level( int id);
		//ˆê‚Âã‚ÌLevel‚Ö–ß‚é
		bool back_level();
		//Level‚ğ“o˜^
		bool regist_level(int id, level_t Level);
		//Às
		level_t execute();

		int get_frame() const { return frame; }
	};
	template<class T>
	auto create_level() -> decltype(std::make_shared<T>()) { return std::make_shared<T>(); }
}