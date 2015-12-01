#include "../headers/resource_manager.h"

aa::resource_manager::constructor aa::resource_manager::cons;

aa::resource_manager::constructor::constructor()
{
	aa::resource_manager::get_instance()->add_factory<aa::vertex_shader>(std::unique_ptr<aa::resource_factory>(new aa::vertex_shader_factory));
	aa::resource_manager::get_instance()->add_factory<aa::fragment_shader>(std::unique_ptr<aa::resource_factory>(new aa::fragment_shader_factory));
}
