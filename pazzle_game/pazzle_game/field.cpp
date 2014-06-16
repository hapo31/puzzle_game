#include<random>
#include"field.h"
#include"draw_system.h"
#include"random_manager.h"

using namespace GameObject;
using namespace util;

auto rnd = util::random_manager::getInstance();

field::field(int x, int y) : field_size_(x + 2, y + 2), data_((x + 4) * (y + 4)), flags_((x + 4) * (y + 4))
, connectnum((x + 4) * (y + 4))
{}

void field::initialize()
{
	for (int i = 0; i < field_size_.y; ++i)
	{
		for (int j = 0; j < field_size_.x; ++j)
		{
			int corrent = get_memorypos(j, i);
			flags_[corrent] = NOP;
			//壁の生成
			if (i == 0 || i == field_size_.y - 1 || j == 0 || j == field_size_.x - 1)
			{
				data_[corrent] = WALL;
			}
			else
			{
				data_[corrent] = block::block_new(10, 0);
				// ブロックを初期化する
				data_[corrent].erase();
			}

		}
	}
	create_num = 1;
	while (create_num != 0)
	{
		create_blocks();
		block_update();
	}
	//以下の4つは、開幕消えるのを防止するために出現ブロックを固定する
	data_[get_memorypos(field_size_.x / 2, 1)].set_connect_dir(DOWN | RIGHT);
	data_[get_memorypos(field_size_.x / 2, field_size_.y - 2)].set_connect_dir(UP | LEFT);
	data_[get_memorypos(1, field_size_.y / 2)].set_connect_dir(DOWN | RIGHT);
	data_[get_memorypos(field_size_.x - 2, field_size_.y / 2)].set_connect_dir(DOWN | LEFT);
}

//壁とそのブロックが繋がっているかどうか(壁はしブロック限定)
bool field::is_connected(int x, int y) const
{
	int corrent = get_memorypos(x, y);
	if (data_[corrent].get_block_type() == BLOCK)		
	{
		if (x == 1)
		{
			return data_[corrent] & LEFT;
		}
		else if (x == field_size_.x - 1)
		{
			return data_[corrent] & RIGHT;
		}
		else if (y == 1)
		{
			return data_[corrent] & UP;
		}
		else if (y == field_size_.y - 1)
		{
			return data_[corrent] & DOWN;
		}
	}
	return false;
}


//空いている最上段にブロック生成
int field::create_blocks()
{
	int n = 0;
	for (int i = 1; i < field_size_.x - 1; ++i)
	{
		if (data_[get_memorypos(i, 1)].get_block_type() == BLANK)
		{
			++create_num;
			data_[get_memorypos(i, 1)] = block::block_new(50, 0);
			data_[get_memorypos(i, 1)].set_eraseframe(erase_frame);
			flags_[get_memorypos(i, i)] = NOP;
			++n;
		}
	}
	return n;
}

int field::fall_blocks(int x)
{
	int result = 0, tmp;
	//下から上に向かって見る
	for (int i = field_size_.y - 2; i >= 0; --i)
	{
		if (data_[get_memorypos(x, i)].get_block_type() == BLANK)
		{
			if (result < (tmp = fall_block(x, i)))
			{
				result = tmp;
			}
		}
	}
	
	return result;
}

int field::fall_block(int x, int y, int n)
{
	flags_[get_memorypos(x, y)] = NOP;
	if (y < 1)
	{
		data_[get_memorypos(x, y)].set_old();
		return n;
	}
	else
	{
		//上がブロックならx,y位置のブロックと入れ替え
		if (data_[get_memorypos(x, y - 1)].get_block_type() == BLOCK)
		{
			data_[get_memorypos(x, y)] = std::move( data_[get_memorypos(x, y - 1)]);
			flags_[get_memorypos(x, y)] = NOP;
			return fall_block(x, y - 1, n + 1);
		}
		else
		{
			return fall_block( x, y - 1 , n);
		}
	}
}

std::vector<field::ERASE_CHK> field::block_erase_check(bool erase_flag)
{
	//flags初期化
	for (auto& it : flags_) if (it != ERASING && it != ERASE) it = NOP;
	for (auto& it : connectnum) it = 0;

	for (int i = 0; i < field_size_.y; ++i)
	{
		for (int j = 0; j < field_size_.x; ++j)
		{
			int corrent = get_memorypos(j, i);
			//壁から調べる
			if (data_[corrent].get_block_type() == WALL)
			{
				int right = corrent + 1;
				int left = corrent - 1;
				int up = corrent - field_size_.x;
				int down = corrent + field_size_.x;
				auto flag_tmp = flags_[corrent];
				if (down < (int) data_.size() && (data_[down] & UP))
				{
					//新ブロックなら繋がらなくなるまで回す
				
					while (data_[down].is_new() && data_[down] & UP) data_[down].rota(0);
					if ((connectnum[corrent] = block_erase_impl(j, i + 1, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (right < (int) data_.size() &&  (data_[right] & LEFT))
				{
					while (data_[right].is_new() && data_[right] & LEFT) data_[right].rota(0);
					if ((connectnum[corrent] = block_erase_impl(j + 1, i, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (left >= 0 && (data_[left] & RIGHT))
				{
					while (data_[left].is_new() && data_[left] & RIGHT) data_[left].rota(0);
					if ((connectnum[corrent] = block_erase_impl(j - 1, i, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (up >= 0 &&  (data_[up] & DOWN))
				{
					while (data_[up].is_new() && data_[up] & DOWN) data_[up].rota(0);
					if ((connectnum[corrent] = block_erase_impl(j, i - 1, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (flags_[corrent] == ERASE && flag_tmp == ERASING)
					flags_[corrent] = ERASING;
			}
		}
	}
	return flags_;
}

void field::update()
{	
	create_blocks();
	block_erase_check();
	block_update();
}


//ブロックチェックの実装部
int field::block_erase_impl(int x, int y, int bef_pos, int erase_num)
{
	int corrent = get_memorypos(x, y);
	int t = 0;
	//消す場合はtrue
	bool this_erase = false;
	ERASE_CHK flag_tmp = flags_[corrent];

	//壁の場合はそこで探索終了
	if (data_[corrent].get_block_type() == WALL)
	{
		//最低消去数以上ならその時点での接続数を返す
		if (erase_num > erase_min)
		{
			if (flag_tmp != ERASING)
				flags_[corrent] = ERASE;
			return connectnum[corrent] = erase_num;
		}
		//未満なら0を返す
		else
		{
			return 0;
		}
	}
	if ( data_[corrent].is_new() || flags_[corrent] == TMP || data_[corrent] == BLANK)
	{
		return 0;
	}
	else if (flags_[corrent] == ERASE)
	{
		return connectnum[corrent];
	}
	else
	{
		int right = corrent + 1;
		int left = corrent - 1;
		int down = corrent + field_size_.x;
		int up = corrent - field_size_.x;

		flags_[corrent] = TMP;
		//右が左に繋がっているか
		if (right != bef_pos && (data_[corrent] & RIGHT) && (data_[right].get_block_type() == WALL || data_[right] & LEFT))
		{
			if (t = block_erase_impl(x + 1, y, corrent, erase_num + 1))
			{
				this_erase = true;
				flags_[corrent] = ERASE;
				if (t > connectnum[corrent])
					connectnum[corrent] = t;
			}
			else if (flags_[corrent] == TMP)
			{
				connectnum[corrent] = 0;
			}
		}
		//左が右に繋がっているか
		if ( left != bef_pos && (data_[corrent] & LEFT) && (data_[left].get_block_type() == WALL || data_[left] & RIGHT))
		{
			if (t = block_erase_impl(x - 1, y, corrent, erase_num + 1))
			{
				this_erase = true;
				flags_[corrent] = ERASE;
				if (t > connectnum[corrent])
					connectnum[corrent] = t;
			}
			else if(flags_[corrent] == TMP)
			{
				connectnum[corrent] = 0;
			}
		}
		//上が下に繋がっているか
		if ( up != bef_pos && (data_[corrent] & UP) && (data_[up].get_block_type() == WALL || data_[up] & DOWN))
		{
			if (t = block_erase_impl(x, y - 1, corrent, erase_num + 1))
			{
				this_erase = true;
				flags_[corrent] = ERASE;
				if (t > connectnum[corrent])
					connectnum[corrent] = t;
			}
			else if (flags_[corrent] == TMP)
			{
				connectnum[corrent] = 0;
			}
		}
		//下が上に繋がっているか
		if (down != bef_pos && (data_[corrent] & DOWN) && (data_[down].get_block_type() == WALL || data_[down] & UP))
		{
			if (t = block_erase_impl(x, y + 1, corrent, erase_num + 1))
			{
				this_erase = true;
				flags_[corrent] = ERASE;
				if (t > connectnum[corrent])
					connectnum[corrent] = t;
			}
			else if (flags_[corrent] == TMP)
			{
				connectnum[corrent] = 0;
			}
		}
		if (this_erase && flag_tmp == ERASING)
		{
			flags_[corrent] = ERASING;
		}
	}
	//行き止まり
	if (flags_[corrent] == TMP)
	{
		data_[corrent].set_eraseframe(erase_frame);
		flags_[corrent] = WALLCONNECT;
		return 0;
	}
	else
	{
		return connectnum[corrent];
	}
}

//ax,ayのブロックとbx,byのブロックを入れ替える
void field::block_swap(int ax, int ay, int bx, int by)
{
	block tmp = std::move(data_[ay * field_size_.x + ax]);
	data_[ay * field_size_.x + ax] = std::move(data_[by * field_size_.x + bx]);
	data_[by * field_size_.x + bx] = std::move(tmp);
}

//ブロックの状態更新
void field::block_update()
{	
	bool no_create = create_num == 0;
	//下に隙間がなくなるように落とす
	for (int i = 0; i < field_size_.x; ++i)
	{
		fall_blocks(i);
	}
	for (int i = 0; i < (int)data_.size(); ++i)
	{
		if (data_[i].get_block_type() != BLANK)
		{
			if (no_create)
				data_[i].set_old();
			//flagsをチェック
			switch (flags_[i])
			{
				/* FALL THROUGH */
			case NOP:
			case WALLCONNECT:
				data_[i].set_eraseframe(erase_frame);
				break;
			case ERASE:
				flags_[i] = ERASING;
			case ERASING:
				if (!data_[i].decrement_eraseframe())
				{
					if (data_[i].get_block_type() != WALL)
					{
						//仮のスコア処理
						score_ += connectnum[i] * 30;
						data_[i].erase();
					}
					flags_[i] = NOP;
				}
				break;
			}
		}
		else
		{
			flags_[i] = NOP;
		}
	}
	create_num = 0;
}