/*
 * The CLI program entry point.
 *
 * Copyright 2011 Lauri Kenttä
 *
 * This file is part of PutkaRTS.
 *
 * PutkaRTS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PutkaRTS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdexcept>
#include <boost/thread.hpp>

#include "ProgramInfo.hpp"
#include "util/Path.hpp"
#include "connection/Server.hpp"
#include "connection/TCPListener.hpp"

/**
 * Main function for the command-line interface.
 */
int main(int argc, char** argv)
try {
	Path::init(argc ? argv[0] : "./bin/unknown.exe");
	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", CLI)";
	std::cout << title << std::endl;
	std::shared_ptr<Connection::Server> server(new Connection::Server());

	int listeners = 0;
	try {
		std::cout << "Starting TCP listener on IPv6... ";
		server->addListener(std::make_shared<Connection::TCPListener>(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 6667)));
		std::cout << "OK!\n";
		++listeners;
	} catch (std::runtime_error& e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	try {
		std::cout << "Starting TCP listener on IPv4... ";
		server->addListener(std::make_shared<Connection::TCPListener>(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6667)));
		std::cout << "OK!\n";
		++listeners;
	} catch (std::runtime_error& e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	if (!listeners) {
		std::cout << "No listeners, bailing out...\n";
		return 1;
	}
	std::cout << "Listeners added, starting the main loop." << std::endl;
	server->run();
	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal exception: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Fatal exception of unknown cause!" << std::endl;
	return 1;
}
