#include "../headers/signal.h"
#include "../headers/application.h"
#include "../headers/window.h"
#include "../headers/web_cam.h"
#include "../headers/egl.h"

#include <iostream>

int main()
{
	auto devices = aa::web_cam::get_devices();
	for(auto it = devices.begin(); it != devices.end(); it++)
	{
		auto supported_formats = aa::web_cam::get_supported_formats(*it);
		std::cout << supported_formats.size() << std::endl;
		for(auto it2 = supported_formats.begin(); it2 != supported_formats.end(); it2++)
		{
			std::cout << "format: " << it2->pixel_format << std::endl;
			std::cout << "width: " << it2->width << std::endl;
			std::cout << "height: " << it2->height << std::endl;
			std::cout << "frame rate: " << it2->frame_rate << std::endl;
			std::cout << std::endl;
		}
	}
	aa::application app;
	aa::window wnd1("aa1", 0, 0, 400, 400);
	app.set_file_descriptor(wnd1);
	//aa::web_cam cam(*devices.begin(), *aa::web_cam::get_supported_formats(*devices.begin()).begin());
	//app.set_file_descriptor(cam);
	//aa::egl egl(wnd1);
	//cam.start_capturing();
	app.run();
	//cam.stop_capturing();
}
