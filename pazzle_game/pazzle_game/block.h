#pragma once
#include<utility>

namespace GameObject
{
	enum DIR
	{
		UP		= 0x1,
		RIGHT	= 0x2,
		DOWN	= 0x4,
		LEFT	= 0x8,
	};
	enum BLOCK_TYPE
	{
		BLANK,
		WALL,
		BLOCK,
	};
	class block
	{
		//線がある向き
		int connect_dir_ = 0;
		//ブロックが消えるまでのフレーム数
		int erase_frame = 0xff;
		BLOCK_TYPE mytype_ = BLANK;
	public:
		block();
		explicit block(int dir) : connect_dir_(dir), mytype_(BLOCK) {}
		explicit block(BLOCK_TYPE type) : mytype_(type) {}
		block(block&& object)
		{
			*this = std::move(object);
		}
		//0 = Right 1 = Left
		void rota(int dir);
		//update my type
		block& operator=(BLOCK_TYPE type)
		{
			if (type != BLOCK) mytype_ = type;
			return *this;
		}
		block& operator=(block&& object)
		{
			mytype_ = object.mytype_;
			connect_dir_ = object.connect_dir_;
			object.mytype_ = BLANK;
			object.connect_dir_ = 0;
			erase_frame = object.erase_frame;
			return *this;
		}
		void set_eraseframe(int value){ erase_frame = value; }
		int get_eraseframe() const { return erase_frame; }
		//引いた結果が0以下の場合はfalse
		bool decrement_eraseframe() { return (--erase_frame) >= 1; }
		//ブロックを消す
		bool erase() 
		{ 
			if (mytype_ != BLANK)
			{
				if (mytype_ == BLOCK)
					mytype_ = BLANK;
				connect_dir_ = 0;
				erase_frame = 0xff;
				return true;
			}
			else
				return false;
		}

		bool operator==(BLOCK_TYPE type) const { return mytype_ == type; }
		bool operator&(int dir) const { return (connect_dir_ & dir) != 0; }
		BLOCK_TYPE get_block_type() const { return  mytype_; }
		int get_connect_dir() const { return connect_dir_; }
		void set_connect_dir(int dir){ connect_dir_ = dir; mytype_ = BLOCK; }
	};
}