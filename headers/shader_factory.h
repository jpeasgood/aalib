#ifndef AA_SHADER_FACTORY_H
#define AA_SHADER_FACTORY_H

#include "../headers/egl.h"
#include "../headers/resource_factory.h"
#include "../headers/shader.h"

#include <memory>
#include <fstream>

namespace aa
{
	class vertex_shader_factory : public resource_factory
	{
		public:
			vertex_shader_factory(const vertex_shader_factory &other) = delete;
			vertex_shader_factory(vertex_shader_factory &&other) = default;
			vertex_shader_factory() = default;

			vertex_shader_factory &operator=(const vertex_shader_factory &other) = delete;
			vertex_shader_factory &operator=(vertex_shader_factory &&other) = delete;

			virtual std::shared_ptr<resource> load(const std::string &file) final;
	};

	class fragment_shader_factory : public resource_factory
	{
		public:
			fragment_shader_factory(const fragment_shader_factory &other) = delete;
			fragment_shader_factory(fragment_shader_factory &&other) = default;
			fragment_shader_factory() = default;

			fragment_shader_factory &operator=(const fragment_shader_factory &other) = delete;
			fragment_shader_factory &operator=(fragment_shader_factory &&other) = default;

			virtual std::shared_ptr<resource> load(const std::string &file) final;
	};
}

#endif
