#pragma once
#include"translater.h"
#include<DxLib.h>

namespace level
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̊Ǘ�
	class fade_updater
	{
		using value_type = double;
		util::translater<value_type> updater;
		int frame_ = 10;
	public:
		enum type { in, out };
		fade_updater() : updater(255, 255, 1) {};
		explicit fade_updater(type t) : updater(t ? 255 : 0, t ? 0 : 255, frame_){}
		fade_updater(type t, int frames) : frame_(frames), updater(t ? 255 : 0, t ? 0 : 255, frame_){}
		void set(type t, int frame)
		{
			updater = util::translater<value_type>(t == out ? 255 : 0, t == out ? 0 : 255, frame);
		}
		void set(type t)
		{
			set(t, frame_);
		}
		bool update()
		{
			//���`
			if (updater.next([&](int frame, int now, value_type end) -> value_type
			{
				return (now / (float) frame) * end;
			}) == boost::none)
			{
				//�t�F�[�h�C���A�A�E�g���I�������true
				return true;
			}
			else
			{
				//�I����Ă��Ȃ���΍X�V
				auto n = updater.get_now();
				SetDrawBright((int) *n, (int) *n, (int) *n);
				return false;
			}
		}
		bool is_end() 
		{
			return !updater.get_now();
		}
	};
	typedef enum fade_updater::type fade_type;
}