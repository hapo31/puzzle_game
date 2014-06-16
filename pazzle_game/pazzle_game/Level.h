#pragma once

#include"draw_system.h"
#include"field.h"

namespace level
{
	enum LEVEL_ID
	{
		TITLE,
		LOADING,
		CONFIG,
		GAME_MAIN,
		EXIT,
	};

	class ILevel
	{
	public:
		//実行　intの値の内容は好きに決めてよい
		virtual int execute() = 0;
		//自分のLEVEL_IDを返す
		virtual int get_myid() const = 0;
		//初期化が完了していればtrue していなければfalse
		virtual bool init() = 0;
		//終了処理が完了していればtrue していなければfalse
		virtual bool end() = 0;
	};
}