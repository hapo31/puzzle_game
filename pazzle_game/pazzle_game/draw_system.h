#pragma once
#include"Position.h"
#include"block.h"
#include"resource.h"
#include"field.h"
#include"coursor.h"

#include<array>

namespace draw
{

	class draw_game
	{
		//描画するブロックのサイズ
		util::pos<int> block_size_;
		util::pos<int> field_draw_;

		std::array<res::resource, 50> res;
		res::resource background = nullptr;
		
		int frame = 0;
	public:
		draw_game();

		void draw_Block(int x, int y, const GameObject::block& object) const;
		void draw_Field(int x, int y, const GameObject::field& object) const;
		void draw_Flags(int x, int y, const GameObject::field& object) const;
		void draw_coursor(int x, int y, const GameObject::cursor& object) const;
		void add_frame() { ++frame; }
		void reset_frame() { frame = 0; }
		void set_frame(int value){ frame = value; }
	};

	//例えばこんなん
	class draw_menu;
}