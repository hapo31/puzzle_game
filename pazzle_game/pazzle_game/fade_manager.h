#pragma once

namespace level
{
	//フェードイン、フェードアウトの管理
	template<class Bright_Updater>
	class fade_updater
	{
		//現在のフレーム数
		int frames = 0;
		//フェードにかけるフレーム数
		int fade_frame = 0;
		int before_bright_ = 255;
		int after_bright_ = 255;
		Bright_Updater updater;
	public:
		fade_updater() = default;
		fade_updater(int before_bright, int after_bright, int frame) : before_bright_(before_bright), after_bright_(after_bright), frames(frame) {}
		//before_brightからafter_brightへframeかけて明るさを変える
		void set_fade(int before_bright, int after_bright, int frame)
		{
			fade_frame = frame;
			before_bright_ = before_bright;
			after_bright_ = after_bright;
		}
		//明るさのアップデート　終わっていればtrue
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