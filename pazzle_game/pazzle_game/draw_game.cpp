#include"draw_system.h"
#include"resource_manager.h"
#include"resource_loader.h"

#include<DxLib.h>

using namespace draw;
using namespace res;
using namespace GameObject;

const int CursorColor = GetColor(255, 50, 0);
const int LineColor = GetColor(255, 255, 0);
const int BlockDefaultColor = GetColor(0, 128, 255);
const int block_size = 64;
//カーソル枠の太さ
const int frame_tickness = 3;
//ブロック線の太さ
const int blockline_tickness = 5;

enum RES
{
	BLOCK,
	FIELD,
};

draw_game::draw_game() : block_size_(block_size, block_size)
{
	auto* resmn = Resource_mng::get_Instance();
	auto gr_control = get_loader<graphic_controler>();
	this->res[RES::BLOCK] = resmn->Regist("block.png", gr_control);
	this->res[RES::FIELD] = resmn->Regist("field.png", gr_control);

	for (auto& r : res)
	{
		if (r.is_Enable())
			r.Load();
	}
}

void draw_game::draw_Field(int x, int y, const field& object) const
{
	typedef enum GameObject::field::ERASE_CHK FLAG;
	auto field_size = object.get_size();
	//背景描画
	DrawBox(x - frame_tickness, y - frame_tickness, x + (frame_tickness + block_size_.x) * field_size.x, y + (frame_tickness + block_size_.y) * field_size.y, GetColor(50, 50, 150), true);
	for (int i = 0; i < field_size.y; ++i)
	{
		for (int j = 0; j < field_size.x; ++j)
		{
			draw_Block(x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness), object.get_block_const(j, i));
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			switch (object.get_flag(j, i))
			{
			case FLAG::WALLCONNECT:
				DrawBox(
					x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness),
					x + (j + 1) * (block_size_.x + frame_tickness) - frame_tickness, y + (i + 1) * (block_size_.y + frame_tickness) - frame_tickness,
					GetColor(255, 0, 255), true
					);
				break;
			case FLAG::ERASING:
				DrawBox(
					x + j * (block_size_.x + frame_tickness), y + i * (block_size_.y + frame_tickness),
					x + (j + 1) * (block_size_.x + frame_tickness) - frame_tickness, y + (i + 1) * (block_size_.y + frame_tickness) - frame_tickness,
					GetColor(255, 255, 0), true
					);
				break;
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void draw_game::draw_Flags(int x, int y, const field& object) const
{
	auto field_size = object.get_size();
	typedef enum GameObject::field::ERASE_CHK FLAG;

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
			case FLAG::NEW:
				str = "Nw";
				color = GetColor(255, 0, 255);
				break;
			default:
				str = "No";
				break;
			}
			DrawString(3 + x + j * (block_size + frame_tickness), y + i * (block_size + frame_tickness), str.c_str(), color);
			DrawFormatString(3 + x + j * (block_size + frame_tickness), 20 + y + i * (block_size + frame_tickness), GetColor(255, 255, 255), "%d", object.connectnum[i * field_size.x + j]);
			DrawFormatString(3 + x + j * (block_size + frame_tickness), 40 + y + i * (block_size + frame_tickness), GetColor(255, 255, 255), "%d", object.get_block_const(j, i).get_eraseframe());

			//(j * (block_size_.x + 5), i * (block_size_.y + 5), object.get_block(j, i));
		}
	}
}

const double PI = 3.1415926;

void draw_game::draw_coursor(int x, int y, const cursor& object) const
{
	auto rect = object.get_rect();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(55 * sin(frame / 10) ) + 100);
	for (int i = rect.top; i < rect.bottom; ++i)
	{
		for (int j = rect.left; j < rect.right; ++j)
		{
			DrawBox(x + j * (block_size + frame_tickness), y + i * (block_size + frame_tickness), x + (j + 1) * (block_size + frame_tickness) - frame_tickness, y + (i + 1) * (block_size + frame_tickness) - frame_tickness, CursorColor, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void draw_game::draw_Block(int x, int y, const block& object) const
{
	int cn = object.get_connect_dir();
	//ブロックの中央座標
	util::pos<int> center(x + block_size_.x / 2, y + block_size_.y / 2 ) ;

	switch (object.get_block_type())
	{
	case BLOCK_TYPE::BLANK:
		DrawBox(x, y, x + block_size_.x, y + block_size_.y, BlockDefaultColor, true);
		break;
	case BLOCK_TYPE::WALL:
		DrawBox(x, y, x + block_size_.x, y + block_size_.y, GetColor(128, 128, 128), true);
		break;
	case BLOCK_TYPE::BLOCK:
		//仮でDrawBox
		DrawBox(x, y, x + block_size_.x, y + block_size_.y, BlockDefaultColor, true);
		//線の方向にLineを描画する
		if (cn & DIR::UP)
		{
			DrawLine(center.x, center.y, center.x, center.y - block_size_.y / 2, LineColor, blockline_tickness);
		}
		if (cn & DIR::LEFT)
		{
			DrawLine(center.x, center.y, center.x - block_size_.x / 2, center.y, LineColor, blockline_tickness);
		}
		if (cn & DIR::DOWN)
		{
			DrawLine(center.x, center.y, center.x, center.y + block_size_.y / 2, LineColor, blockline_tickness);
		}
		if (cn & DIR::RIGHT)
		{
			DrawLine(center.x, center.y, center.x + block_size_.x / 2, center.y, LineColor, blockline_tickness);
		}
		break;
	}
}