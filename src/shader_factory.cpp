#include "../headers/shader_factory.h"

std::shared_ptr<aa::resource> aa::vertex_shader_factory::load(const std::string &path)
{
	std::ifstream file;
	file.open(path);
	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return std::shared_ptr<aa::resource>(new vertex_shader(
			std::move(aa::egl::get_instance()->create_vertex_shader(code))));
}

std::shared_ptr<aa::resource> aa::fragment_shader_factory::load(const std::string &path)
{
	std::ifstream file;
	file.open(path);
	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return std::shared_ptr<aa::resource>(new fragment_shader(
			std::move(aa::egl::get_instance()->create_fragment_shader(code))));
}
