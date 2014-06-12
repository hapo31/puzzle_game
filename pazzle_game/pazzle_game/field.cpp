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
			int corrent = i * field_size_.x + j;
			flags_[corrent] = NOP;
			//壁の生成
			if (i == 0 || i == field_size_.y - 1 || j == 0 || j == field_size_.x - 1)
			{
				data_[corrent] = WALL;
			}
			else if (false)
			{
				// 1/50の確率でT字を出す
				data_[corrent] = block_new(50, 5);
				data_[corrent].set_eraseframe(erase_frame);
			}

		}
	}
}

/*
block field::block_new()
{
	int lines [] = { RIGHT, UP, LEFT, DOWN };
	int rnd = ::rnd->get_rand() % 4;
	int max = ::rnd->get_rand() % 3;
	int rnd2 = (rnd + max) % 4;
	rnd2 = rnd2 == rnd ? (rnd2 + 1) % 4 : rnd2;
	int line = lines[rnd] | lines[rnd2];
	return block(line);
}

block field::block_new(int dirs)
{
	int line = 0;

	switch (dirs)
	{
	case 0:
	case 1:
		throw std::runtime_error("dirs too small");
	case 2:
		return block_new();
	case 3:
	{
		int lines [] = { RIGHT, UP, LEFT, DOWN };
		int rnd = ::rnd->get_rand() % 4;
		line = (~lines[rnd]);
	}
		break;
	case 4:
		line = 31;
		break;
	default:
		throw std::runtime_error("dirs too big");
	}

	return block(line);
}
*/

//確率指定でブロック生成
block field::block_new(int ThreeLineProb, int FourLineProb)
{
	int lines[] = { RIGHT, UP, LEFT, DOWN };
	int rnd = ::rnd->get_rand() % 4;
	int line = 0;
	int prob = ::rnd->get_rand();
	if (FourLineProb > 0 && prob % FourLineProb == 0)
	{
		line = (UP | RIGHT | DOWN | LEFT);
	}
	else if (ThreeLineProb > 0 && prob % ThreeLineProb == 0)
	{
		line = ~lines[rnd];
	}
	else
	{
		int max = ::rnd->get_rand() % 3;
		int rnd2 = (rnd + max) % 4;
		rnd2 = rnd2 == rnd ? (rnd2 + 1) % 4 : rnd2;
		line = lines[rnd] | lines[rnd2];
	}
	block b(line);
	b.set_eraseframe(erase_frame);
	return b;
}

//向き指定でブロック生成
block field::block_new(int dir)
{
	block b(dir);
	b.set_eraseframe(erase_frame);
	return b;
}
//空いている最上段にブロック生成
int field::create_blocks()
{
	int n = 0;
	for (int i = 1; i < field_size_.x - 1; ++i)
	{
		if (data_[field_size_.x + i].get_block_type() == BLANK)
		{
			data_[field_size_.x + i] = block_new(50, 5);
			flags_[field_size_.x + i] = NEW;
			++n;
		}
	}
	return n;
}

int field::fall_blocks()
{
	//下から上に向かって横にずらしながら見る
	for (int j = 1; j < field_size_.x - 1; ++j)
	{
		for (int i = field_size_.y - 2; i >= 0; --i)
		{

			if (data_[i * field_size_.x + j].get_block_type() == BLANK)
			{
				i = fall_block(j , i);
				
				//生成したばかりのブロックなら、いきなり消えないように回転or変更(十字)する
				if (data_[i * field_size_.x + j].get_block_type() == WALL)
				{

				}
			}
		}
	}
	return 0;
}

int field::fall_block(int x, int y)
{
	flags_[y * field_size_.x + x] = NOP;
	if (y < 1)
		return 1;
	else
	{
		//上がブロックならx,y位置のブロックと入れ替え
		if (data_[(y - 1) * field_size_.x + x].get_block_type() == BLOCK)
		{
			data_[y * field_size_.x + x] = std::move( data_[(y - 1) * field_size_.x + x]);
			auto t = flags_[y * field_size_.x + x];
			flags_[y * field_size_.x + x] = flags_[(y - 1)  * field_size_.x + x];
			flags_[(y - 1)  * field_size_.x + x] = t;
			return 1 + fall_block(x, y - 1);
		}
		else
		{
			return fall_block( x, y - 1 );
		}
	}
}

std::vector<field::ERASE_CHK> field::block_erase_check(bool erase_flag)
{
	//flags初期化
	for (auto& it : flags_) if (it != ERASING && it != ERASE && it != NEW) it = NOP;
	for (auto& it : connectnum) it = 0;

	for (int i = 0; i < field_size_.y; ++i)
	{
		for (int j = 0; j < field_size_.x; ++j)
		{
			int corrent = i * field_size_.x + j;
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
					if ((connectnum[corrent] = block_erase_impl(j, i + 1, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (right < (int) data_.size() &&  (data_[right] & LEFT))
				{
					if ((connectnum[corrent] = block_erase_impl(j + 1, i, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (left >= 0 && (data_[left] & RIGHT))
				{
					if ((connectnum[corrent] = block_erase_impl(j - 1, i, corrent)) != 0)
						flags_[corrent] = ERASE;
					else
						flags_[corrent] = NOP;
				}
				if (up >= 0 &&  (data_[up] & DOWN))
				{
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
	block_erase_check();
	block_update();
	create_blocks();
	fall_blocks();
}


//ブロックチェックの実装部
int field::block_erase_impl(int x, int y, int bef_pos, int erase_num)
{
	int corrent = y * field_size_.x + x;
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
	if (flags_[corrent] == TMP || flags_[corrent] == NEW || data_[corrent] == BLANK)
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
	for (int i = 0; i < (int)data_.size(); ++i)
	{
		if (data_[i].get_block_type() != BLANK)
		{
			//flagsをチェック
			switch (flags_[i])
			{
			case NEW:
				flags_[i] = NOP;
				/* FALL THROUGH */
			case NOP:
			case WALLCONNECT:
				data_[i].set_eraseframe(erase_frame);
				break;
			case ERASING:
				if (!data_[i].decrement_eraseframe())
				{
					if (data_[i].get_block_type() != WALL)
						data_[i].erase();
					else
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
}