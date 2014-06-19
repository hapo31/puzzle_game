#pragma once

#include<DxLib.h>

namespace util
{
	//スコープ内にDrawBlendModeを適用する
	class scoped_dx_blendmode
	{
	public:
		scoped_dx_blendmode(int DX_BLEND_MODE, int value)
		{
			SetDrawBlendMode(DX_BLEND_MODE, value);
		}
		~scoped_dx_blendmode()
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	};
}