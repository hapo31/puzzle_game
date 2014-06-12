#pragma once
#include<random>
#include<memory>

namespace util
{
	class random_manager
	{
		long seed = 123456789;
		std::shared_ptr<std::mt19937> rnd_algo;

		random_manager() : rnd_algo(std::make_shared<std::mt19937>(seed)){}
		random_manager(const random_manager&) = delete;
		random_manager& operator=(const random_manager&) = delete;
	public:
		static random_manager* getInstance()
		{
			static random_manager obj;
			return &obj;
		}
		void set_seed(long value){ rnd_algo = std::make_shared<std::mt19937>(seed = value); }
		long get_seed() const { return seed; }

		uint32_t get_rand() const { return (*rnd_algo)(); }
	};
}