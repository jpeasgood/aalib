#include "../headers/file_descriptor_interface.h"

aa::file_descriptor_interface::file_descriptor_interface()
{
	fd_connection = nullptr;
}

aa::file_descriptor_interface::~file_descriptor_interface()
{
	if(fd_connection)
	{
		fd_connection->disconnect();
	}
}

/*void aa::file_descriptor_interface::connect_fd(connection<void()> *conn)
{
	if(fd_connection)
	{
		fd_connection->disconnect();
		delete fd_connection;
	}
	fd_connection = conn;
}*/
