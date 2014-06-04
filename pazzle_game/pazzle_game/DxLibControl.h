#pragma once
#include<DxLib.h>
#include<array>
#include<vector>

namespace input
{
	class DxLibControl
	{
		int input_type_;
		std::vector<std::pair<int, int>> log;
		std::array<int, 32> data;
	public:
		enum Key{ DOWN, LEFT, RIGHT, UP, A, B, C, D, E, F, G, H, I, J, K };

		DxLibControl(int input_type = DX_INPUT_KEY_PAD1) : input_type_(input_type){ data.fill(0); }
		void update()
		{
			int input = GetJoypadInputState(input_type_);
			int n = 1;
			for (auto&& button : data)
			{
				if ((n & input) != 0)
					++button;
				else
					button = 0;

				if (button == 1)
				{
					//“ü—Í‚ÌƒƒO‚ð‹L˜^
					log.push_back({ n, button });
					if (log.size() > 10u)
						log.erase(log.begin());
				}

				n <<= 1;
			}
			for (auto& str : log)
				printfDx("%d\n", str.first);
		}
		int operator[](int n){ return data[n]; }
		int at(int n){ return data.at(n); }
	};
}