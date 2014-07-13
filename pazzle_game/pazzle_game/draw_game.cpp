#include"draw_system.h"
#include"resource_manager.h"
#include"resource_loader.h"
#include"Scoped_DxBlendMode.h"

#include<DxLib.h>

using namespace draw;
using namespace res;
using namespace GameObject;

const int CursorColor = GetColor(255, 50, 0);
const int LineColor = GetColor(200, 200, 20);
const int DarkLineColor = GetColor(150, 150, 0);
const int BlockDefaultColor = GetColor(0, 128, 255);
const int block_size = 64;
//カーソル枠の太さ
const int frame_tickness = 3;
//ブロック線の太さ
const int blockline_tickness = 3;

enum RES
{
	BLOCK,
	WALL,
	FIELD,
	FONT
};

draw_game::draw_game() : block_size_(block_size, block_size)
{
	auto resmn = Resource_mng::get_Instance();

	res[RES::BLOCK] = resmn->Get_RegistedResource("data/wall.png");
	res[RES::WALL] = resmn->Get_RegistedResource("data/wall.png");
	res[RES::FIELD] = resmn->Get_RegistedResource("data/field.png");
	res[RES::FONT] = resmn->Get_RegistedResource("MS Gothic2");
}

void draw_game::draw_Field(int x, int y, const field& object) const
{
	typedef enum GameObject::field::ERASE_CHK FLAG;
	auto field_size = object.get_size();
	x += offset.x;
	y += offset.y;
	//背景描画
	//DrawBox(x - frame_tickness, y - frame_tickness, x + (frame_tickness + block_size_.x) * field_size.x, y + (frame_tickness + block_size_.y) * field_size.y, GetColor(50, 50, 150), true);
	DrawGraph(x - frame_tickness, y - frame_tickness, *res[RES::FIELD], true);
	for (int i = 0; i < field_size.y; ++i)
	{
		for (int j = 0; j < field_size.x; ++j)
		{
			switch (object.get_flag(j, i))
			{
			case FLAG::WALLCONNECT:
			{
				util::scoped_dx_blendmode d(DX_BLENDMODE_ADD, 100);
				DrawBox(
					x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness),
					x + (j + 1) * (block_size_.x + frame_tickness) - frame_tickness, y + (i + 1) * (block_size_.y + frame_tickness) - frame_tickness,
					GetColor(255, 255, 255), true
					);
			}
				draw_Block(x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness), object.get_block_const(j, i), true);
				/*DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(
					x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness),
					x + (j + 1) * (block_size_.x + frame_tickness) - frame_tickness, y + (i + 1) * (block_size_.y + frame_tickness) - frame_tickness,
					GetColor(255, 0, 255), true
					);*/
				break;
			case FLAG::ERASING:
				draw_Block(x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness), object.get_block_const(j, i), true);
				if (object.get_block_const(j, i).get_block_type() != GameObject::WALL)
				{
					util::scoped_dx_blendmode d(DX_BLENDMODE_ALPHA, 255 - object.get_block_const(j, i).get_eraseframe() / 2);
					DrawBox(
					x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness),
					x + (j + 1) * (block_size_.x + frame_tickness) - frame_tickness, y + (i + 1) * (block_size_.y + frame_tickness) - frame_tickness,
					GetColor(255, 255, 0), true
					);
				}
				break;
			
			default:
				draw_Block(x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness), object.get_block_const(j, i), false);
			
				break;
			}
		}
	}
	//スコアの描画
	DrawFormatStringToHandle
		(x + ((block_size_.x + frame_tickness) * field_size.x) / 2,
		y + (block_size_.y + frame_tickness) * field_size.y + 5,
		GetColor(0, 0, 0), *res[FONT], "%d", object.get_score()
		);
}

void draw_game::draw_Flags(int x, int y, const field& object) const
{
	auto field_size = object.get_size();
	typedef enum GameObject::field::ERASE_CHK FLAG;
	x += offset.x;
	y += offset.y;
	for (int i = 0; i < field_size.y; ++i)
	{
		for (int j = 0; j < field_size.x; ++j)
		{
			int flag = object.get_flag(j, i);
			std::string str;
			int color = 0;
			int color2 = object.connectnum[i * field_size.x + j] != 0 ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
			switch (flag)
			{
			case FLAG::ERASE:
				str = "Er";
				color = GetColor(255, 0, 0);
				break;
			case FLAG::ERASING:
				str = "Eg";
				color = GetColor(255, 0, 0);
				break;
			case FLAG::WALLCONNECT:
				str = "Wc";
				color = GetColor(0, 255, 255);
				break;
			case FLAG::WALL_COOL_DOWN:
				str = "Cd";
				color = GetColor(255, 0, 255);
				break;
			default:
				str = "No";
				break;
			}
			if (object.get_block_const(j, i).is_new())
			{
				str = "Nw";
			}
			DrawString(3 + x + j * (block_size + frame_tickness), y + i * (block_size + frame_tickness), str.c_str(), color);
			DrawFormatString(3 + x + j * (block_size + frame_tickness), 20 + y + i * (block_size + frame_tickness), GetColor(255, 0, 255), "%d", object.connectnum[i * field_size.x + j]);
			DrawFormatString(3 + x + j * (block_size + frame_tickness), 40 + y + i * (block_size + frame_tickness), GetColor(255, 0, 255), "%d", object.get_block_const(j, i).get_eraseframe());

			//(j * (block_size_.x + 5), i * (block_size_.y + 5), object.get_block(j, i));
		}
	}
}

const double PI = 3.1415926;
inline int BLINK_(int timer, int base, double spd) { return abs((int) (timer*spd) % base * 2 - base); }

void draw_game::draw_coursor(int x, int y, const cursor& object) const
{
	auto rect = object.get_rect();
	util::scoped_dx_blendmode d(DX_BLENDMODE_ALPHA, BLINK_(frame, 150, 3) + 55);
	x += offset.x;
	y += offset.y;
	for (int i = rect.top; i < rect.bottom; ++i)
	{
		for (int j = rect.left; j < rect.right; ++j)
		{
			DrawBox(x + j * (block_size + frame_tickness), y + i * (block_size + frame_tickness), x + (j + 1) * (block_size + frame_tickness) - frame_tickness, y + (i + 1) * (block_size + frame_tickness) - frame_tickness, CursorColor, true);
		}
	}
}

void draw_game::draw_Block(int x, int y, const block& object, bool is_connect) const
{
	int cn = object.get_connect_dir();
	//ブロックの中央座標
	util::pos<int> center(x + block_size_.x / 2, y + block_size_.y / 2 ) ;

	switch (object.get_block_type())
	{
	case BLOCK_TYPE::BLANK:
		//DrawGraph(x, y, *res[RES::WALL], true);
		//DrawBox(x, y, x + block_size_.x, y + block_size_.y, BlockDefaultColor, true);
		break;
	case BLOCK_TYPE::WALL:
		DrawGraph(x, y, *res[RES::WALL], true);
		//DrawBox(x, y, x + block_size_.x, y + block_size_.y, GetColor(128, 128, 128), true);
		break;
	case BLOCK_TYPE::BLOCK:
	{
		int color = is_connect ? LineColor : DarkLineColor;
		//仮でDrawBox
		//DrawBox(x, y, x + block_size_.x, y + block_size_.y, BlockDefaultColor, true);
		//DrawGraph(x, y, *res[RES::BLOCK], true);
		//線の方向にLineを描画する
		if (cn & DIR::UP)
		{
			DrawLine(center.x, center.y, center.x, center.y - block_size_.y / 2, color, blockline_tickness);
		}
		if (cn & DIR::LEFT)
		{
			DrawLine(center.x, center.y, center.x - block_size_.x / 2, center.y, color, blockline_tickness);
		}
		if (cn & DIR::DOWN)
		{
			DrawLine(center.x, center.y, center.x, center.y + block_size_.y / 2, color, blockline_tickness);
		}
		if (cn & DIR::RIGHT)
		{
			DrawLine(center.x, center.y, center.x + block_size_.x / 2, center.y, color, blockline_tickness);
		}
	}
		break;
	}
}