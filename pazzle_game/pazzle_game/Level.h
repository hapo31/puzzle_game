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
		//���s�@int�̒l�̓��e�͍D���Ɍ��߂Ă悢
		virtual int execute() = 0;
		//������LEVEL_ID��Ԃ�
		virtual int get_myid() const = 0;
		//���������������Ă����true ���Ă��Ȃ����false
		virtual bool init() = 0;
		//�I���������������Ă����true ���Ă��Ȃ����false
		virtual bool end() = 0;
	};
}