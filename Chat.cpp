#include "Chat.h"
#define NOMINMAX
#include <windows.h>

// Colors for the console
const size_t colorWhite = 7;
const size_t colorLightBlue = 11;
const size_t colorLightGreen = 10;
const size_t colorYellow = 14;
const size_t colorDarkBlue = 19;

// Declare and initialize variables to change the color of text in the console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
size_t concolColor = colorWhite;

Chat::Chat()
{
	users_.push_back(make_shared<User>(User{ static_cast<string>("all"), static_cast<string>("allPswd") }));
	users_.push_back(make_shared<User>(User{ static_cast<string>("test1"), static_cast<string>("t1Pswd"), static_cast<string>("TEST1") }));
	users_.push_back(make_shared<User>(User{ static_cast<string>("test2"), static_cast<string>("t2Pswd"), static_cast<string>("TEST2") }));
	isWork_ = true;
}

bool Chat::getIsWork() const { return isWork_; }
bool Chat::getIsToolsMenu() const { return isToolsMenu_; }

shared_ptr<User> Chat::getPtrCurrentUser() const { return ptr_currentUser_; }
void Chat::setPtrCurrentUser(shared_ptr<User> ptr_currentUser) { ptr_currentUser_ = ptr_currentUser; }

shared_ptr<User> Chat::getPtrUserByLogin(const string& login) const
{
	for (auto& u : users_)
	{
		if (u->getLogin() == login)
			return u;
	}
	return nullptr;
}

shared_ptr<User> Chat::getPtrUserByName(const string& name) const
{
	for (auto& u : users_)
	{
		if (u->getName() == name)
			return u;
	}
	return nullptr;
}

void Chat::checkNewLogin(const string& login)
{
	auto ptrUser = getPtrUserByLogin(login);
	if (ptrUser)
		throw UserException("This login already exists!\n");

	if (login == "all")
		throw UserException("This login is reserved!\n");
}

void Chat::checkNewPassword(const string& psw)
{
	if (psw.length() < minPswLen)
		throw UserException("Password must be equal to or more than 5 characters long!\n");
}

void Chat::checkLogin(const string& login, const string& psw)
{
	
	auto ptrUser = getPtrUserByLogin(login);
	if (!ptrUser)
		throw UserException("Invalid login! Try again, please.\n");

	if (ptrUser->getPassword() != psw)
		throw UserException("Invalid password! Try again, please.\n");

	setPtrCurrentUser(ptrUser);
}

void Chat::signUp()
{
	string userLogin;
	string userPassword;
	string userName;

	do
	{
		try
		{
			// User sign in attempt 
			cout << "Login: ";
			cin >> userLogin;
			checkNewLogin(userLogin);

			cout << endl << "Password: ";
			cin >> userPassword;
			checkNewPassword(userPassword);

			cout << endl << "Name (optional): ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, userName);
			cout << endl;

			if (userName.length() == 0)
				userName = userLogin;

			auto ptrNewUser = make_shared<User>(User{ userLogin, userPassword, userName });
			users_.push_back(ptrNewUser);
			setPtrCurrentUser(ptrNewUser);
		}
		catch (UserException& e)
		{
			ptr_currentUser_ == nullptr;
			cerr << e.what();
		}
	} while (ptr_currentUser_ == nullptr);

	if (ptr_currentUser_)
		cout << *ptr_currentUser_ << ", you have successfully registered!\n" << endl;
}

void Chat::signIn()
{
	string userLogin;
	string userPassword;
	size_t cntAttemptsLogin = 0;

	do
	{
		cout << "Login: ";
		cin >> userLogin;
		cout << endl << "Password: ";
		cin >> userPassword;
		cout << endl;

		try
		{
			// User sign in attempt 
			++cntAttemptsLogin;
			checkLogin(userLogin, userPassword);
		}
		catch (UserException& e)
		{
			cerr << e.what();
		}
	} while (ptr_currentUser_ == nullptr && (cntAttemptsLogin < cntAttempts));

	cntAttemptsLogin = 0;

	if (ptr_currentUser_ == nullptr)
		cout << "You have made " << cntAttempts << " attempts! Sign Up, please.\n";
}

void Chat::Exit()
{
	cout << "Goodby, " << *ptr_currentUser_ << "!" << endl;
	if (ptr_currentUser_)
		ptr_currentUser_ = nullptr;
}

void Chat::showAllUsers()
{
	// меняем цвет текста
	concolColor = colorDarkBlue;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cout << "*****   USERS   *****" << endl;
	// меняем цвет текста
	concolColor = colorWhite;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	for (auto& u : users_)
	{
		cout << u->getName() << ((u == ptr_currentUser_) ? " (*)" : "") << endl;
	}
}

void Chat::showAllMessages()
{
	// меняем цвет текста
	concolColor = colorDarkBlue;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cout << "*****   MESSAGES   *****" << endl;
	// меняем цвет текста
	concolColor = colorWhite;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	for (auto& msg : messages_)
	{
		if (msg->getTo() == ptr_currentUser_->getLogin() || (msg->getTo() == static_cast<string>("all") && msg->getFrom() != ptr_currentUser_->getLogin()))
			cout << "->  " << *msg << endl;
		if (msg->getFrom() == ptr_currentUser_->getLogin())
			cout << "<-  " << *msg << endl;
	}
}

void Chat::sendMessage()
{
	string to{};
	string text{};

	cout << "To (name or all): ";
	cin >> to;
	cout << endl << "Text: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, text);
	cout << endl;

	try
	{
		auto ptrUser = getPtrUserByName(to);
		if (!ptrUser && to != "all")
			throw MessageException("Invalid user name (to)! Choose another user, please.\n");

		messages_.push_back(make_shared<Message>(Message{ ptr_currentUser_->getLogin(), ptrUser->getLogin(), text }));
	}
	catch (MessageException& e)
	{
		cerr << e.what();
	}
}

void Chat::changePassword()
{
	string newPassword;

	cout << "Enter new password: ";
	cin >> newPassword;
	cout << endl;

	try
	{
		checkNewPassword(newPassword);
	}
	catch (UserException& e)
	{
		cerr << e.what();
	}

	ptr_currentUser_->setPassword(newPassword);
}

void Chat::changeName()
{
	string newName;

	cout << "Enter new nick name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, newName);
	cout << endl;

	ptr_currentUser_->setName(newName);
}

void Chat::chatMenu()
{
	int choiceSign = 1;

	string userLogin;
	string userPassword;
	string userName;

	// меняем цвет текста
	concolColor = colorYellow;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cout << string(47, '-') << endl;
	cout << "| Sign In (1) || Sign Up (2) || Stop chat (0) |" << endl;
	cout << string(47, '-') << endl;
	// меняем цвет текста
	concolColor = colorWhite;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cin >> choiceSign;

	switch (choiceSign)
	{
	case 1:	
		signIn();
		break;
	case 2:
		signUp();
		break;
	case 0:
		isWork_ = false;
		break;
	default:
		cout << "Wrong choice!" << endl << endl;
		break;
	}
}

void Chat::userMenu()
{
	int choice = 1;

	// меняем цвет текста
	concolColor = colorLightBlue;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cout << string(74, '-') << endl;
	cout << "| Send Message (1) || Messages (2) || Users (3) || Tools (4) || Exit (0) |" << endl;
	cout << string(74, '-') << endl;
	// меняем цвет текста
	concolColor = colorWhite;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cin >> choice;

	switch (choice)
	{
	case 1:
		sendMessage();
		break;
	case 2:
		showAllMessages();
		break;
	case 3:
		showAllUsers();
		break;
	case 4:
		isToolsMenu_ = true;
		break;
	case 0:
		Exit();
		break;
	default:
		cout << "Wrong choice!" << endl << endl;
		break;
	}
}

void Chat::toolsMenu()
{
	int choice = 1;

	// меняем цвет текста
	concolColor = colorLightGreen;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cout << string(56, '-') << endl;
	cout << "| Change Name (1) || Change password (2) || Return (0) |" << endl;
	cout << string(56, '-') << endl;
	// меняем цвет текста
	concolColor = colorWhite;
	SetConsoleTextAttribute(hConsole, concolColor);
	// ----------
	cin >> choice;

	switch (choice)
	{
	case 1:
		changeName();
		break;
	case 2:
		changePassword();
		break;
	case 0:
		isToolsMenu_ = false;
		break;
	default:
		cout << "Wrong choice!" << endl << endl;
		break;
	}
}