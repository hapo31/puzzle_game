#pragma once

namespace level
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̊Ǘ�
	template<class Bright_Updater>
	class fade_updater
	{
		//���݂̃t���[����
		int frames = 0;
		//�t�F�[�h�ɂ�����t���[����
		int fade_frame = 0;
		int before_bright_ = 255;
		int after_bright_ = 255;
		Bright_Updater updater;
	public:
		fade_updater() = default;
		fade_updater(int before_bright, int after_bright, int frame) : before_bright_(before_bright), after_bright_(after_bright), frames(frame) {}
		//before_bright����after_bright��frame�����Ė��邳��ς���
		void set_fade(int before_bright, int after_bright, int frame)
		{
			fade_frame = frame;
			before_bright_ = before_bright;
			after_bright_ = after_bright;
		}
		//���邳�̃A�b�v�f�[�g�@�I����Ă����true
		bool update()
		{
			if (frames < fade_frame)
			{
				int bright = updater(before_bright_, after_bright_, frame);
				SetDrawBright(bright, bright, bright);
				++frames;
				return false;
			}
			else
			{
				frames = 0;
				return true;
			}
		}
	};
}