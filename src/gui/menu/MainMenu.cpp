#include <memory>
#include <thread>
#include <functional>

#include "connection/Server.hpp"
#include "connection/Client.hpp"
#include "connection/Address.hpp"

#include "MainMenu.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/menu/SettingsMenu.hpp"
#include "gui/menu/StartGame.hpp"
#include "gui/menu/JoinGame.hpp"

GUI::Menu::MainMenu::MainMenu(sf::RenderWindow& window):
	Menu() {
	// Build the main menu GUI.
	insert(new GUI::Widget::Button("New game", 200, 100 + 0 * 70, 240, 50,
				       std::bind(&GUI::Menu::MainMenu::startGame, this, std::ref(window))));
	insert(new GUI::Widget::Button("Connect",  200, 100 + 1 * 70, 240, 50,
				       std::bind(&GUI::Menu::MainMenu::startMultiGame, this, std::ref(window))));
	insert(new GUI::Widget::Button("Settings", 250, 100 + 2 * 70, 140, 50,
				       std::bind(&GUI::Menu::MainMenu::gotoSettings, this, std::ref(window))));
	insert(new GUI::Widget::Button("Exit",     250, 100 + 3 * 70, 140, 50,
				       std::bind(&sf::RenderWindow::close, std::ref(window))));
}

void GUI::Menu::MainMenu::startMultiGame(sf::RenderWindow& window) {
	GUI::currentWidget.reset(new GUI::Menu::JoinGame(GUI::currentWidget));
}

void GUI::Menu::MainMenu::startGame(sf::RenderWindow& window) {
	std::shared_ptr<Connection::Server> server(new Connection::Server());
	std::shared_ptr<Connection::Client> client(server->createLocalClient());
	std::thread serverThread(std::bind(&Connection::Server::run, server));
	serverThread.detach();
	GUI::currentWidget.reset(new GUI::Menu::StartGame(GUI::currentWidget, client));
}

void GUI::Menu::MainMenu::gotoSettings(sf::RenderWindow& window) {
	GUI::currentWidget.reset(new GUI::Menu::SettingsMenu(GUI::currentWidget, window));
}
