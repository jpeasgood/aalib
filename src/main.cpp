#include "../headers/signal.h"
#include "../headers/application.h"
#include "../headers/window.h"
#include "../headers/camera.h"
#include "../headers/egl.h"
#include "../headers/texture.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstddef>

int main()
{
	auto devices = aa::camera::get_devices();
	/*for(auto it = devices.begin(); it != devices.end(); it++)
	{
		auto supported_formats = aa::camera::get_supported_formats(*it);
		std::cout << supported_formats.size() << std::endl;
		for(auto it2 = supported_formats.begin(); it2 != supported_formats.end(); it2++)
		{
			std::cout << "format: " << it2->pixel_format << std::endl;
			std::cout << "width: " << it2->width << std::endl;
			std::cout << "height: " << it2->height << std::endl;
			std::cout << "frame rate: " << it2->frame_rate << std::endl;
			std::cout << std::endl;
		}
	}*/

	aa::application app;
	aa::window wnd1("aa1", 0, 0, 400, 400);
	aa::window wnd2("aa2", 0, 0, 400, 400);
	app.set_file_descriptor(wnd1);
	app.set_file_descriptor(wnd2);

	auto camera1_device = *devices.begin();
	auto camera2_device = *(++devices.begin());
	auto camera_format = *aa::camera::get_supported_formats(camera1_device).begin();
	aa::camera cam1(camera1_device, camera_format);
	aa::camera cam2(camera2_device, camera_format);

	app.set_file_descriptor(cam1);
	app.set_file_descriptor(cam2);

	aa::egl egl;

	auto surf1 = egl.create_window_surface(wnd1);
	auto surf2 = egl.create_window_surface(wnd2);

	aa::texture texture1 = egl.create_texture(camera_format.width, camera_format.height);
	aa::texture texture2 = egl.create_texture(camera_format.width, camera_format.height);

	std::ifstream file;
	file.open("shaders/camera.vert");
	std::string vertex_shader_code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	aa::shader vertex_shader = egl.create_shader(aa::shader::shader_type::VERTEX_SHADER, vertex_shader_code);

	file.open("shaders/yuyv.frag");
	std::string fragment_shader_code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	aa::shader fragment_shader = egl.create_shader(aa::shader::shader_type::FRAGMENT_SHADER, fragment_shader_code);

	auto program = egl.create_program(vertex_shader, fragment_shader);
	program.use();

	auto position = program.get_attribute("position");
	auto text_coord = program.get_attribute("texCoord");
	position.enable();
	text_coord.enable();

	auto projection = program.get_uniform("projection");

	auto texl_w = program.get_uniform("texl_w");
	texl_w.set(1.0f / camera_format.width);
	auto tex_w = program.get_uniform("tex_w");
	tex_w.set(static_cast<float>(camera_format.width));

	glActiveTexture(GL_TEXTURE0);
	auto tex = program.get_uniform("tex");
	tex.set(0);

	auto vertex_buff = egl.create_buffer(aa::buffer::buffer_type::VERTEX);

	auto index_buff = egl.create_buffer(aa::buffer::buffer_type::INDEX);

	unsigned int ids[]= { 1, 2, 0, 3 };
	index_buff.bind();
	index_buff.set(sizeof(ids), &ids);

	cam1.connect_process_frame([&] (const void *data, unsigned int size)
	{
		texture1.bind();
		texture1.set(data);
		wnd1.expose();
	});

	cam2.connect_process_frame([&] (const void *data, unsigned int size)
	{
		texture2.bind();
		texture2.set(data);
		wnd2.expose();
	});

	wnd1.connect_expose( [&] (int x, int y, int width, int height)
	{
		egl.make_surface_current(surf1);
		egl.set_viewport(x, y, width, height);
		egl.clear();

		texture1.bind();

		auto ortho_mat = ortho_matrix<float>(0.0f, width, height, 0.0f, 0.0f, 1.0f);
		projection.set(ortho_mat);

		GLfloat verts[] = { 0, 0, 0, 0,
					width, 0, 1, 0,
					width, height, 1, 1,
					0, height, 0, 1 };
		vertex_buff.bind();
		vertex_buff.set(sizeof(verts), &verts);
		position.set_float_attribute_ptr(2, 4 * sizeof(float), 0);
		text_coord.set_float_attribute_ptr(2, 4 * sizeof(float), 2 * sizeof(float));

		index_buff.bind();
		egl.draw_elements(4);
		glFlush();
		egl.swap_buffers(surf1);
		check_gl_error();
	});

	wnd2.connect_expose( [&] (int x, int y, int width, int height)
	{
		egl.make_surface_current(surf2);
		egl.set_viewport(x, y, width, height);
		egl.clear();

		texture2.bind();

		auto ortho_mat = ortho_matrix<float>(0.0f, width, height, 0.0f, 0.0f, 1.0f);
		projection.set(ortho_mat);

		GLfloat verts[] = { 0, 0, 0, 0,
					width, 0, 1, 0,
					width, height, 1, 1,
					0, height, 0, 1 };
		vertex_buff.bind();
		vertex_buff.set(sizeof(verts), &verts);
		position.set_float_attribute_ptr(2, 4 * sizeof(float), 0);
		text_coord.set_float_attribute_ptr(2, 4 * sizeof(float), 2 * sizeof(float));

		index_buff.bind();
		egl.draw_elements(4);
		glFlush();
		egl.swap_buffers(surf2);
		check_gl_error();
	});

	cam1.start_capturing();
	cam2.start_capturing();
	app.run();
	cam2.stop_capturing();
	cam1.stop_capturing();
}
