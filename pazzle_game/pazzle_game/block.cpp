#include"block.h"

using namespace GameObject;

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