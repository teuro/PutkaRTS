#include <functional>
#include <string>

#include "connection/Client.hpp"
#include "connection/Address.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/game/Game.hpp"
#include "gui/menu/JoinGame.hpp"
#include "gui/menu/StartGame.hpp"

GUI::Menu::JoinGame::JoinGame(std::shared_ptr<Widget> parent_):
	Menu(parent_),
gameList(new GUI::Widget::List(100, 200, 200, 200, std::bind(&GUI::Menu::JoinGame::selectGame, this, std::placeholders::_1))),
	selectedId(0),
	statusLabel(new GUI::Widget::Label("", 100, 100, 440, 100)) {
	insert(gameList);
	insert(statusLabel);
	insert(new GUI::Widget::Button("Join", 400, 300, 100, 30, std::bind(&GUI::Menu::JoinGame::joinGame, this)));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, std::bind(&GUI::Menu::JoinGame::closeMenu, this)));
}

void GUI::Menu::JoinGame::updateState(sf::RenderWindow& window) {
	Menu::updateState(window);
	try {
		if (!metaserver.getGames()) {
			return;
		}
	} catch (std::runtime_error& e) {
		statusLabel->setText(e.what());
		return;
	}
	gameList->clear();

	typedef std::pair<const int, Connection::Metaserver::Game> PairType;
	for (const PairType& pair: metaserver.games) {
		const Connection::Metaserver::Game& game(pair.second);
		gameList->insertItem(std::to_string(game.id), game.name);
	}
	gameList->selectItem(std::to_string(selectedId));
}

void GUI::Menu::JoinGame::selectGame(const GUI::Widget::List::Item& item) {
	selectedId = item.key.empty() ? 0 : std::stoi(item.key);
}

void GUI::Menu::JoinGame::joinGame() {
	if (metaserver.games.find(selectedId) == metaserver.games.end()) {
		return; // Invalid selection.
	}
	std::shared_ptr<Connection::EndPoint> connection;
	std::string error;
	for (const std::string& address: metaserver.games[selectedId].addresses) {
		try {
			connection = Connection::Address::connect(address);
			break;
		} catch (std::runtime_error& e) {
			if (!error.empty()) {
				error += "\n";
			}
			error += e.what();
		}
	}
	if (!connection) {
		statusLabel->setText(error);
	} else {
		std::shared_ptr<Connection::Client> client(new Connection::Client(connection));
		GUI::currentWidget.reset(new GUI::Menu::StartGame(GUI::currentWidget, client));
	}
}
