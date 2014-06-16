#pragma once
#include<array>
#include<vector>
#include"DxLibControl.h"

/*
	Controlerクラスに対応したクラスの作り方
		1.int operator[]で内部のバッファを読み込める(読み込み専用)
		2.update()関数を持つ
		3.Key列挙体が内部で定義され、十字キーを含むボタンを表す要素が列挙されている
			(例: enum Key{ DOWN, LEFT, RIGHT, UP, B1, B2... }という列挙体を定義する（要素名は自由）)
	静的多態での対応なので、キーを毎フレーム画面出力する機能をつけたり、リプレイを読み込んで入力したりなどが自由にできる
*/
/*
namespace input
{
	template<class Control_T = DxLibControl>
	class Controler
	{
		Control_T control;
	public:
		typedef enum Control_T::Key Key;
		void update()
		{
			control.update();
		}
		bool is_PushedButton(Key keycode){ return control[keycode] == 1; }
		int is_PushedButton_frames(Key keycode){ return control[keycode]; }
	};
	
	template<class Controler_T>
	class Controler_Manager
	{
		std::vector<Controler_T> controlers;
		Controler_Manager() = default;
		Controler_Manager(const Controler_Manager&) = delete;
		Controler_Manager& operator=(const Controler_Manager&) = delete;
	public:
		static Controler_Manager* get_Instance()
		{
			static Controler_Manager obj;
			return &obj;
		}
		Controler_T& getControl(int n) { return controlers[n]; }
	};

}
*/