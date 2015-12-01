#ifndef AA_TEXTURE_WINDOW_H
#define AA_TEXTURE_WINDOW_H

#include "../headers/matrix.h"
#include "../headers/egl.h"
#include "../headers/surface.h"
#include "../headers/texture.h"
#include "../headers/window.h"

#include <memory>
#include <string>

namespace aa
{
	class texture_window : public window
	{
		friend class constructor;

		private:
			struct constructor
			{
				constructor();
			};

			static constructor cons;

			static program &prog();
			static buffer &vertex_buff();
			static buffer &index_buff();
			static uniform_variable &texl_w();
			static uniform_variable &tex_w();
			static uniform_variable &projection();
			static attribute_variable &position();
			static attribute_variable &text_coord();

			surface surf;
			std::weak_ptr<texture> tex;

			void draw_texture(int x, int y, int width, int height);

		public:
			texture_window(const texture_window &other) = delete;
			texture_window(texture_window &&other) = default;
			texture_window(std::weak_ptr<texture> tex, const std::string &title, int x, int y, unsigned int width, unsigned int height);
			virtual ~texture_window();

			texture_window &operator=(const texture_window &other) = delete;
			texture_window &operator=(texture_window &&other) = default;
	};
}


#endif
