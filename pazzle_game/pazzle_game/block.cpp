#include"block.h"

using namespace GameObject;

void block::rota(int dir)
{
	if (dir == 0)
	{
		//5ビット目を取る
		bool t = (connect_dir_ & 0x8) != 0;
		//左にずらして1ビット目にtを挿入
		connect_dir_ <<= 1;
		connect_dir_ |= (int)t;
		connect_dir_ &= 15;
	}
	else
	{
		//1ビット目を取る
		bool t = connect_dir_ & 0x1;
		//右にずらして4ビット目にtを挿入
		connect_dir_ >>= 1;
		connect_dir_ |= (int) (t << 4);
	}
}