#include "../headers/texture_window.h"
#include <iostream>

aa::texture_window::constructor aa::texture_window::cons;

aa::program &aa::texture_window::prog()
{
	static aa::program program(aa::egl::get_instance()->create_program(
	*resource_manager::get_instance()->load<aa::vertex_shader>("shaders/camera.vert").lock(),
	*resource_manager::get_instance()->load<aa::fragment_shader>("shaders/yuyv.frag").lock()));
	return program;
}

aa::buffer &aa::texture_window::vertex_buff()
{
	static aa::buffer vertex_buffer(aa::egl::get_instance()->create_buffer(aa::buffer::buffer_type::VERTEX));
	return vertex_buffer;
}

aa::buffer &aa::texture_window::index_buff()
{
	static aa::buffer index_buffer(aa::egl::get_instance()->create_buffer(aa::buffer::buffer_type::INDEX));
	return index_buffer;
}

aa::uniform_variable &aa::texture_window::texl_w()
{
	static aa::uniform_variable texl_w_var(prog().get_uniform("texl_w"));
	return texl_w_var;
}

aa::uniform_variable &aa::texture_window::tex_w()
{
	static aa::uniform_variable tex_w_var(prog().get_uniform("tex_w"));
	return tex_w_var;
}

aa::uniform_variable &aa::texture_window::projection()
{
	static aa::uniform_variable projection_var(prog().get_uniform("projection"));
	return projection_var;
}

aa::attribute_variable &aa::texture_window::position()
{
	static aa::attribute_variable position_var(prog().get_attribute("projection"));
	return position_var;
}

aa::attribute_variable &aa::texture_window::text_coord()
{
	static aa::attribute_variable text_coord_var(prog().get_attribute("tex_coord"));
	return text_coord_var;
}

aa::texture_window::constructor::constructor()
{
	auto tex = prog().get_uniform("tex");
	tex.set(0);

	position().enable();
	text_coord().enable();

	unsigned int ids[]= { 1, 2, 0, 3 };
	index_buff().bind();
	index_buff().set(sizeof(ids), &ids);

}

void aa::texture_window::draw_texture(int x, int y, int width, int height)
{
	auto texture = tex.lock();
	if(texture)
	{
		aa::egl::get_instance()->make_surface_current(surf);

		prog().use();

		aa::egl::get_instance()->set_viewport(x, y, width, height);
		aa::egl::get_instance()->clear();

		aa::egl::get_instance()->activate_texture(0);
		texture->bind();

		aa::matrix<float> ortho_mat = aa::ortho_matrix<float>(0.0f, width, height, 0.0f, 0.0f, 1.0f);
		projection().set(ortho_mat);

		texl_w().set(1.0f / texture->get_width());
		tex_w().set(static_cast<float>(texture->get_width()));

		GLfloat verts[] = { 0, 0, 0, 0,
					width, 0, 1, 0,
					width, height, 1, 1,
					0, height, 0, 1 };
		vertex_buff().bind();
		vertex_buff().set(sizeof(verts), &verts);
		position().set_float_attribute_ptr(2, 4 * sizeof(float), 0);
		text_coord().set_float_attribute_ptr(2, 4 * sizeof(float), 2 * sizeof(float));

		index_buff().bind();
		aa::egl::get_instance()->draw_elements(4);
		aa::egl::get_instance()->flush();
		aa::egl::get_instance()->swap_buffers(surf);
	}
}

aa::texture_window::texture_window(std::weak_ptr<aa::texture> tex, const std::string &title,
		int x, int y, unsigned int width, unsigned int height)
	: window(title, x, y, width, height),
	  tex(tex),
	  surf(aa::egl::get_instance()->create_window_surface(*this))
{
	connect_expose(std::bind(&aa::texture_window::draw_texture, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

aa::texture_window::~texture_window()
{
}
