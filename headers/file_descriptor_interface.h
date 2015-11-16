#ifndef AA_FILE_DESCRIPTOR_INTERFACE
#define AA_FILE_DESCRIPTOR_INTERFACE

#include "../headers/signal.h"

#include <memory>

namespace aa
{
	class file_descriptor_interface
	{
		friend class application;

		private:
			std::unique_ptr<connection<void()>> fd_connection;

			//void connect_fd(connection<void()> *conn);

		protected:
			virtual void fd_selected() = 0;

		public:
			file_descriptor_interface();
			virtual ~file_descriptor_interface();
			virtual int get_fd() const = 0;
	};
}

#endif
