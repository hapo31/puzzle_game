#include"block.h"
#include"random_manager.h"

using namespace GameObject;
namespace
{
	auto rnd = util::random_manager::getInstance();
}
void block::rota(int dir)
{
	if (dir == 0)
	{
		//5�r�b�g�ڂ����
		bool t = (connect_dir_ & 0x8) != 0;
		//���ɂ��炵��1�r�b�g�ڂ�t��}��
		connect_dir_ <<= 1;
		connect_dir_ |= (int)t;
		connect_dir_ &= 15;
	}
	else
	{
		//1�r�b�g�ڂ����
		bool t = connect_dir_ & 0x1;
		//�E�ɂ��炵��4�r�b�g�ڂ�t��}��
		connect_dir_ >>= 1;
		connect_dir_ |= (int) (t << 4);
	}
}

//�m���w��Ńu���b�N����
block block::block_new(int ThreeLineProb, int FourLineProb)
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
	return b;
}

//�����w��Ńu���b�N����
block block::block_new(int dir)
{
	return block(dir);
}