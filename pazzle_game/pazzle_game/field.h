#pragma once
#include<vector>
#include"block.h"
#include"Position.h"

namespace GameObject
{
	class field
	{
	public: enum ERASE_CHK
		{
			NOP,		//�܂��������ĂȂ�
			CHECKED,	//���׏I�����
			TMP,		//������������Ȃ�
			ERASE,		//�����̂��m��
			ERASING,	//������
			WALLCONNECT,//�ǂƐڑ����Ă��� 
			NEW,		//���o�����u���b�N
		};
	private:
		//�u���b�N�f�[�^
		std::vector<block> data_;
		//�u���b�N�ɕt����ꂽ�t���O
		std::vector<ERASE_CHK> flags_;
		util::pos<int> field_size_;

		enum{ erase_min = 1 };

		//x,y ���ɒ��ׂ�u���b�N�̍��W
		//bef_pos ���݂̍��W(���̌Ăяo���łǂ̕�������Ă΂ꂽ���𓾂邽��
		//erase_num �������
		int block_erase_impl(int x, int y, int bef_pos, int erase_num = erase_min - 1);

		//�u���b�N��������܂ł̃t���[����
		int erase_frame = 150;
		//�u���b�N�̐���@�ċA�����p
		int fall_block(int x, int y);		
		//a��b�����ւ���
		void block_swap(int ax, int ay, int bx, int by);
		void block_swap(const util::pos<int>& a, const util::pos<int>& b){ block_swap(a.x, a.y, b.x, b.y); }

		//�u���b�N�𐶐����� 3������4�����̐����m�����w��ł���
		inline block block_new(int ThreeLineProb, int FourlineProb);
		//�����w��Ńu���b�N�𐶐�����
		inline block block_new(int dir);
		//1�񂸂u���b�N�𐶐����� �����ł��Ȃ�(�t�B�[���h�����܂��Ă���)�ꍇ��0��Ԃ�
		inline int create_blocks();

		//BLANK�ȉӏ��𖄂߂�
		int fall_blocks();

		//�u���b�N����
		int block_erase();
		//�u���b�N�̍X�V(�u���b�N��erase_frame�̍X�V)
		void block_update();
		//�u���b�N�̐ڑ��`�F�b�N
		std::vector<ERASE_CHK> block_erase_check(bool erase_flag = true);

	public:
		//���̈ʒu�̃u���b�N�����Ȃ����Ă��邩
		std::vector<int> connectnum;

		using block_iterator = std::vector<block>::iterator;
		using flags_iterator = std::vector<ERASE_CHK>::iterator;

		field(int sizex, int sizey);
		void initialize();

		//�t�B�[���h�̃A�b�v�f�[�g
		void update();

		decltype(field_size_) get_size() const { return field_size_; }
		block& get_block(int x, int y) { return data_[ y * field_size_.x + x ]; }
		const block& get_block_const(int x, int y) const { return data_[y * field_size_.x + x]; }

		void set_block(int x, int y, block&& object)
		{
			data_[y * field_size_.x + x] = std::move(object);
		}
		bool set_block_secure(int x, int y, block&& object)
		{
			if (x < 0 || x >= field_size_.x || y < 0 || y >= field_size_.y)
				return false;
			data_[y * field_size_.x + x] = std::move(object);
			return true;
		}

		int get_flag(int x, int y) const { return flags_ [ y * field_size_.x + x]; }
		block_iterator block_iterator_begin() { return data_.begin(); }
		block_iterator block_iterator_end() { return data_.end(); }
		
		flags_iterator flags_iterator_begin() { return flags_.begin(); }
		flags_iterator flags_iterator_end() { return flags_.end(); }

	};
}