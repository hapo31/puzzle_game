#pragma once
#include<vector>
#include"block.h"
#include"Position.h"

namespace GameObject
{
	class field
	{
	public: enum ERASE_CHK
		{
			NOP,		//まだ何もしてない
			CHECKED,	//調べ終わった
			TMP,		//消すかもしれない
			ERASE,		//消すのが確定
			ERASING,	//消去中
			WALLCONNECT	//壁と接続している 
		};
	private:
		//ブロックデータ
		std::vector<block> data_;
		//ブロックに付けられたフラグ
		std::vector<ERASE_CHK> flags_;
		util::pos<int> field_size_;

		enum{ erase_min = 1 };

		//x,y 次に調べるブロックの座標
		//bef_pos 現在の座標(次の呼び出しでどの方向から呼ばれたかを得るため
		//erase_num 消える個数
		int block_erase_impl(int x, int y, int bef_pos, int erase_num = erase_min - 1);

		//ブロックが消えるまでのフレーム数
		int erase_frame = 150;

	public:
		//その位置のブロックが何個つながっているか
		std::vector<int> connectnum;

		using block_iterator = std::vector<block>::iterator;
		using flags_iterator = std::vector<ERASE_CHK>::iterator;

		field(int sizex, int sizey);
		void initialize();
		
		void block_swap(int ax, int ay, int bx, int by);
		
		//ブロックを生成する
		inline block create_block();
		//向きの数(2 <= dirs <= 4)
		inline block create_block(int dirs);

		//BLANKな箇所を埋める
		int fall_blocks();

		//ブロック消去
		int block_erase();
		//ブロックの更新(ブロックのerase_frameの更新)
		void block_update();
		//ブロックの接続チェック
		std::vector<ERASE_CHK> block_erase_check(bool erase_flag = true);

		decltype(field_size_) get_size() const { return field_size_; }
		block& get_block(int x, int y) { return data_[ y * field_size_.x + x ]; }
		const block& get_block_const(int x, int y) const { return data_[y * field_size_.x + x]; }

		void set_block(int x, int y, block&& object)
		{
			data_[y * field_size_.x + x] = std::move(object);
		}
		bool set_block_secure(int x, int y, block&& object)
		{
			if (x < 0 || x >= field_size_.x || y < 0 || y >= field_size_.y)
				return false;
			data_[y * field_size_.x + x] = std::move(object);
			return true;
		}

		int get_flag(int x, int y) const { return flags_ [ y * field_size_.x + x]; }
		block_iterator block_iterator_begin() { return data_.begin(); }
		block_iterator block_iterator_end() { return data_.end(); }
		
		flags_iterator flags_iterator_begin() { return flags_.begin(); }
		flags_iterator flags_iterator_end() { return flags_.end(); }

	};
}