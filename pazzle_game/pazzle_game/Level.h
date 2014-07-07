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

	enum ENGINE_MASSAGE
	{
		LEVEL_MSG_INIT,
		LEVEL_MSG_NORMAL,
		LEVEL_MSG_BACK,
		LEVEL_MSG_END,
		LEVEL_MSG_PAUSE,
	};

	class ILevel
	{
	public:
		//実行　戻り値は好きに決めてよい
		virtual int execute(int Message) = 0;
		//自分のLEVEL_IDを返す
		virtual int get_myid() const = 0;
		//初期化が完了していればtrue していなければfalse
		virtual bool init(int Message) = 0;
		//終了処理が完了していればtrue していなければfalse
		virtual bool end(int Message) = 0;
	};
}