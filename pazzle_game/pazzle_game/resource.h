#pragma once
#include<memory>
#include<stdexcept>
#include"resource_loader.h"

namespace res
{
	using Loader = std::shared_ptr<IResource_Controler>;
	class resource
	{
		
		Loader loader = nullptr;
		std::shared_ptr<int> handle = nullptr;
		std::string filename_;
		static bool async_flag_;
	public:
		resource() = default;

		explicit resource(Loader loader_) : loader(loader_){}

		//ファイルとローダー指定
		explicit resource(const char* filename, Loader loader_) : loader(loader_), filename_(filename)
		{
			if (!loader)
				std::runtime_error("Handle controler is null");
		}
		//Move constractor
		resource(resource&& rval)
		{
			*this = std::move(rval);
		}
		//nullptr constractor
		resource(std::nullptr_t nullptr_) : handle(nullptr_) {}
		~resource()
		{ 
			Delete();
		}

		resource& operator=(const resource& rvalue)
		{
			//if (handle.use_count() == 1)
			//	Delete();
			loader = rvalue.loader;
			filename_ = rvalue.filename_;
			handle = rvalue.handle;
			return *this;
		}
		resource& operator=(std::shared_ptr<int>& value)
		{
			//if (value == nullptr)
			//	this->Delete();
			this->handle = value;
			return *this;
		}

		explicit operator bool() const { return is_Enable(); }
		//ハンドルを読み込む準備ができているかどうか
		bool is_Enable() const { return loader != nullptr; }
		//読み込み済みかどうか asyncでの読み込み中も考慮する
		bool is_Loaded() const { return handle && (!async_flag_ || CheckHandleASyncLoad(*handle) == FALSE); }
		operator int() const { return handle ? *handle : -1; }
		int get_Handle() const { return handle ? *handle : -1; }
		void set_filename(const std::string& name){ this->filename_ = name; }
		int get_usecount() const { return handle.use_count(); }
		Loader get_controler() const { return loader; }

		bool Load()
		{
			if (!handle || CheckHandleASyncLoad(*handle) == FALSE)
			{
				handle = std::make_shared<int>((*loader).Load(filename_.c_str()));
			}
			return *handle > 0;
		}

		bool Delete()
		{
			//参照カウントが1になったらリソース解放
			if (handle.use_count() == 1)
			{
				(*loader).Delete(*handle);
				handle = nullptr;
				return true;
			}
			else
			{
				handle = nullptr;
				return false;
			}
		}
		static void set_async(bool value)
		{
			async_flag_ = value;
			SetUseASyncLoadFlag(value);
		}
	};
	using resource_sh = std::shared_ptr <resource>;
	inline resource_sh make_resource(const std::string& filename, Loader loader) { return std::make_shared<resource>(filename.c_str(), loader); }
}