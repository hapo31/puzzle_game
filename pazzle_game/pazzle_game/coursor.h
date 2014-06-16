#pragma once
#include"InputDevice.h"
#include"field.h"
#include"Position.h"

namespace GameObject
{
	class cursor
	{
	public:
		//カーソルの形
		//SOLO 1*1サイズ　そのマスを回転
		//SWAP1 2*1サイズ　左右のブロックを入れ替える
		//SWAP2 1*2サイズ　上下のブロックを入れ替える
		//ROTA 2*2サイズ　4つのブロックを回転移動
		enum MODE{ SOLO, SWAP1, SWAP2, ROTA };
		//このクラスで使う向き指定
		enum M_DIR{ UP, DOWN, LEFT, RIGHT };
	private:
		field* target_;
		MODE mode;
		util::pos<int> pos;
		util::pos<int> size;
	public:
		explicit cursor(field* target);
		void move(M_DIR dir);
		bool set_mode(MODE new_mode);
		MODE get_mode() const { return mode; }

		bool rotation(M_DIR dir);
		bool swap(M_DIR dir);


		util::pos<int> get_pos() const { return pos; }
		util::pos<int> get_size() const { return size; }
		RECT get_rect() const { return RECT{ pos.x, pos.y, pos.x + size.x, pos.y + size.y }; }
	};
}