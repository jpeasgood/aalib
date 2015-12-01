#ifndef AA_RESOURCE_FACTORY_H
#define AA_RESOURCE_FACTORY_H

#include <memory>
#include <string>

namespace aa
{
	class resource
	{
		public:
			virtual ~resource() = 0;
	};

	inline resource::~resource() { }

	class resource_factory
	{
		public:
			virtual std::shared_ptr<resource> load(const std::string &path) = 0;
	};
}

#endif
