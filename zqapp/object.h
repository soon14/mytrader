#pragma once
#include "base.h"
#include "utility.h"

namespace zq {

	class Object;

	class Object
	{
	public:
		//static Object* CreateObject(const char* name, intptr_t parent, const char* xml = nullptr, size_t xmlflag = 0);
	public:
		Object() {}
		virtual ~Object() {}

	protected:
		virtual bool get_config(boost::property_tree::ptree& cfg, const char* key, const char* spec = nullptr);
		virtual bool set_config(const boost::property_tree::ptree& cfg, const char* key, const char* spec = nullptr);
	};
}
