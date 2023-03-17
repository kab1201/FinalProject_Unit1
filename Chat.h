#pragma once

#include "User.h"
#include "Message.h"
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

class Chat
{
private:
	const size_t cntAttempts = 3; // count of attempts to Sign In
	const size_t minPswLen = 5; // minimum password length

	bool isWork_{ false };
	bool isToolsMenu_{ false };
	vector<shared_ptr<User>> users_;
	vector<shared_ptr<Message>> messages_;
	shared_ptr<User> ptr_currentUser_ = nullptr;

	void setPtrCurrentUser(shared_ptr<User> ptr_currentUser);
	
	shared_ptr<User> getPtrUserByLogin(const string& login) const;
	shared_ptr<User> getPtrUserByName(const string& name) const;

	void checkNewLogin(const string& login);
	void checkNewPassword(const string& psw);
	void checkLogin(const string& login, const string& psw);

	void signUp();
	void signIn();
	void Exit();

	void showAllUsers();
	void showAllMessages();
	void sendMessage();

	void changePassword();
	void changeName();

public:
	Chat();
	~Chat() = default;

	bool getIsWork() const;
	bool getIsToolsMenu() const;
	shared_ptr<User> getPtrCurrentUser() const;
	void chatMenu();
	void userMenu();
	void toolsMenu();
};
