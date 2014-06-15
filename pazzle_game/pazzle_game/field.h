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
			WALLCONNECT,//壁と接続している 
		};
	private:
		//ブロックデータ
		std::vector<block> data_;
		//ブロックに付けられたフラグ
		std::vector<ERASE_CHK> flags_;
		util::pos<int> field_size_;
		
		//そのフレームで生成されたブロックの数
		int create_num = 0;

		enum{ erase_min = 1 };

		//x,y 次に調べるブロックの座標
		//bef_pos 現在の座標(次の呼び出しでどの方向から呼ばれたかを得るため
		//erase_num 消える個数
		int block_erase_impl(int x, int y, int bef_pos, int erase_num = erase_min - 1);

		//ブロックが消えるまでのフレーム数
		int erase_frame = 150;
		//ブロックの整列　再帰処理用
		int fall_block(int x, int y, int n = 0);		

		//1列ずつブロックを生成する 生成できない(フィールドが埋まっている)場合は0を返す
		inline int create_blocks();

		//ブロックが壁に繋がっているかどうかをチェックする
		bool is_connected(const util::pos<int>& block_pos) const { return is_connected(block_pos.x, block_pos.y); };
		bool is_connected(int ax, int ay) const;

		//指定列のブロックを重力にしたがって落とす
		int fall_blocks(int x);

		//ブロック消去
		int block_erase();
		//ブロックの更新(ブロックのerase_frameの更新)
		void block_update();
		//ブロックの接続チェック
		std::vector<ERASE_CHK> block_erase_check(bool erase_flag = true);

		//data_やflagsのメモリ位置を取得
		int get_memorypos(int x, int y) const { return y * field_size_.x + x; }

	public:
		//その位置のブロックが何個つながっているか
		std::vector<int> connectnum;

		using block_iterator = std::vector<block>::iterator;
		using flags_iterator = std::vector<ERASE_CHK>::iterator;

		field(int sizex, int sizey);
		void initialize();

		//フィールドのアップデート
		void update();

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

		//aとbを入れ替える
		void block_swap(int ax, int ay, int bx, int by);
		void block_swap(const util::pos<int>& a, const util::pos<int>& b){ block_swap(a.x, a.y, b.x, b.y); }

		int get_flag(int x, int y) const { return flags_ [ y * field_size_.x + x]; }
		block_iterator block_iterator_begin() { return data_.begin(); }
		block_iterator block_iterator_end() { return data_.end(); }
		
		flags_iterator flags_iterator_begin() { return flags_.begin(); }
		flags_iterator flags_iterator_end() { return flags_.end(); }

	};
}