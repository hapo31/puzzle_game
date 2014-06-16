#pragma once
#include<DxLib.h>
#include<array>
#include<vector>
#include<unordered_map>

namespace input
{
	enum Key{ DOWN, LEFT, RIGHT, UP, A, B, C, D, E, F, G, H, I, J, K, NO_ASSIGN };
	struct key_mapping
	{
		
	};
	class DxLibControl
	{
	private:
		int input_type_;
		std::vector<std::pair<int, int>> log;
		std::array<int, 33> data;
		std::unordered_map<int, Key> KeyMap;
	public:
		DxLibControl(int input_type = DX_INPUT_KEY_PAD1) : input_type_(input_type)
		{ 
			KeyMap.reserve(data.size());
			KeyMap.emplace(0, DOWN);
			KeyMap.emplace(1, LEFT);
			KeyMap.emplace(2, RIGHT);
			KeyMap.emplace(3, UP);
			KeyMap.emplace(4, A);
			KeyMap.emplace(5, B);
			KeyMap.emplace(6, C);
			KeyMap.emplace(7, D);
			KeyMap.emplace(8, E);
			KeyMap.emplace(9, F);
			data.fill(0); 
		}
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
					//入力のログを記録
					log.push_back({ n, button });
					if (log.size() > 10u)
						log.erase(log.begin());
				}

				n <<= 1;
			}
			//for (auto& str : log)
			//	printfDx("%d\n", str.first);
		}
		//キーコンフィグ
		int set_key_replace(int Key_ID, Key key)
		{
			KeyMap[Key_ID] = key;
		}
		//置き換え済みのキーのフレーム数を取得
		int get_replaced_key(int Key_ID)
		{
			return data[KeyMap[Key_ID]];
		}

		//decltype(KeyMap.begin()) KeyMap_Iterator_begin(){ return KeyMap.begin(); }
		//decltype(KeyMap.end()) KeyMap_Iterator_end(){ return KeyMap.end(); }

		int operator[](int n){ return data[n]; }
		int at(int n){ return data.at(n); }
	};
}