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
		//4ビット目を取る
		int t = (connect_dir_ & 0x8) != 0;
		//左にずらす
		connect_dir_ <<= 1;
		//1bit目に挿入
		connect_dir_ |= (t & 0x1);
		//4bit分のみ残す
		connect_dir_ &= 15;
	}
	else
	{
		//1ビット目を取る
		int t = connect_dir_ & 0x1;
		//右にずらして4ビット目にtを挿入
		connect_dir_ >>= 1;
		connect_dir_ |= t << 3;
		connect_dir_ &= 15;
	}
}

//確率指定でブロック生成
block block::block_new(int ThreeLineProb, int FourLineProb)
{
	int lines[] = { RIGHT, UP, LEFT, DOWN };
	
	int line = 0;
	int prob = ::rnd->get_rand();
	if (FourLineProb > 0 && prob % FourLineProb == 0)
	{
		line = (UP | RIGHT | DOWN | LEFT);
	}
	else if (ThreeLineProb > 0 && prob % ThreeLineProb == 0)
	{
		line = ~lines[rnd->get_rand() % 4];
		line &= 15;
	}
	else
	{
		int line1 = rnd->get_rand() % 2;
		int line2 = rnd->get_rand() % 2 + 2;
		line = lines[line1] | lines[line2];
	}
	block b(line);
	return b;
}

//向き指定でブロック生成
block block::block_new(int dir)
{
	return block(dir);
}