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
			//�ǂ̐���
			if (i == 0 || i == field_size_.y - 1 || j == 0 || j == field_size_.x - 1)
			{
				data_[corrent] = WALL;
			}
			else if (false)
			{
				// 1/50�̊m����T�����o��
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

//�m���w��Ńu���b�N����
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

//�����w��Ńu���b�N����
block field::block_new(int dir)
{
	block b(dir);
	b.set_eraseframe(erase_frame);
	return b;
}
//�󂢂Ă���ŏ�i�Ƀu���b�N����
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
	//�������Ɍ������ĉ��ɂ��炵�Ȃ��猩��
	for (int j = 1; j < field_size_.x - 1; ++j)
	{
		for (int i = field_size_.y - 2; i >= 0; --i)
		{

			if (data_[i * field_size_.x + j].get_block_type() == BLANK)
			{
				i = fall_block(j , i);
				
				//���������΂���̃u���b�N�Ȃ�A�����Ȃ�����Ȃ��悤�ɉ�]or�ύX(�\��)����
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
		//�オ�u���b�N�Ȃ�x,y�ʒu�̃u���b�N�Ɠ���ւ�
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
	//flags������
	for (auto& it : flags_) if (it != ERASING && it != ERASE && it != NEW) it = NOP;
	for (auto& it : connectnum) it = 0;

	for (int i = 0; i < field_size_.y; ++i)
	{
		for (int j = 0; j < field_size_.x; ++j)
		{
			int corrent = i * field_size_.x + j;
			//�ǂ��璲�ׂ�
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


//�u���b�N�`�F�b�N�̎�����
int field::block_erase_impl(int x, int y, int bef_pos, int erase_num)
{
	int corrent = y * field_size_.x + x;
	int t = 0;
	//�����ꍇ��true
	bool this_erase = false;
	ERASE_CHK flag_tmp = flags_[corrent];

	//�ǂ̏ꍇ�͂����ŒT���I��
	if (data_[corrent].get_block_type() == WALL)
	{
		//�Œ�������ȏ�Ȃ炻�̎��_�ł̐ڑ�����Ԃ�
		if (erase_num > erase_min)
		{
			if (flag_tmp != ERASING)
				flags_[corrent] = ERASE;
			return connectnum[corrent] = erase_num;
		}
		//�����Ȃ�0��Ԃ�
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
		//�E�����Ɍq�����Ă��邩
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
		//�����E�Ɍq�����Ă��邩
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
		//�オ���Ɍq�����Ă��邩
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
		//������Ɍq�����Ă��邩
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
	//�s���~�܂�
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

//ax,ay�̃u���b�N��bx,by�̃u���b�N�����ւ���
void field::block_swap(int ax, int ay, int bx, int by)
{
	block tmp = std::move(data_[ay * field_size_.x + ax]);
	data_[ay * field_size_.x + ax] = std::move(data_[by * field_size_.x + bx]);
	data_[by * field_size_.x + bx] = std::move(tmp);
}

//�u���b�N�̏�ԍX�V
void field::block_update()
{
	for (int i = 0; i < (int)data_.size(); ++i)
	{
		if (data_[i].get_block_type() != BLANK)
		{
			//flags���`�F�b�N
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