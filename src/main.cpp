#include "../headers/signal.h"
#include "../headers/application.h"
#include "../headers/texture_window.h"
#include "../headers/camera.h"
#include "../headers/texture.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstddef>

int main()
{
	auto devices = aa::camera::get_devices();
	auto camera1_device = *devices.begin();
	auto camera2_device = *(++devices.begin());
	auto camera_format = *aa::camera::get_supported_formats(camera1_device).begin();
	aa::camera cam1(camera1_device, camera_format);
	aa::camera cam2(camera2_device, camera_format);

	auto texture1 = std::shared_ptr<aa::texture>(new aa::texture(aa::egl::get_instance()->create_texture(camera_format.width, camera_format.height)));
	auto texture2 = std::shared_ptr<aa::texture>(new aa::texture(aa::egl::get_instance()->create_texture(camera_format.width, camera_format.height)));

	aa::texture_window wnd1(texture1, "aa1", 0, 0, 400, 400);
	aa::texture_window wnd2(texture2, "aa2", 0, 0, 400, 400);

	aa::application::get_instance()->set_file_descriptor(wnd1);
	aa::application::get_instance()->set_file_descriptor(wnd2);
	aa::application::get_instance()->set_file_descriptor(cam1);
	aa::application::get_instance()->set_file_descriptor(cam2);

	cam1.connect_process_frame([&](const void *data, unsigned int size)
	{
		texture1->bind();
		texture1->set(data);
		wnd1.expose();
	});

	cam2.connect_process_frame([&](const void *data, unsigned int size)
	{
		texture2->bind();
		texture2->set(data);
		wnd2.expose();
	});

	wnd1.connect_delete_window([&]()
	{
		cam1.stop_capturing();
	});

	wnd2.connect_delete_window([&]()
	{
		cam2.stop_capturing();
	});

	cam1.start_capturing();
	cam2.start_capturing();
	aa::application::get_instance()->run();
}
