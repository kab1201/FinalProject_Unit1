#include "User.h"

UserException::UserException(const string& error) : error_(error) {};

const char* UserException::what() const
{
	return error_.c_str();
}

User::User(const string& login, const string& password) : login_(login), password_(password), name_(login) {}
User::User(const string& login, const string& password, const string& nickName) : login_(login), password_(password), name_(nickName) {}

const string& User::getLogin() const { return login_; }

const string& User::getPassword() const { return password_; }
void User::setPassword(const string& newPassword) { password_ = newPassword; }

const string& User::getName() const { return name_; }
void User::setName(const string& newName) { name_ = newName; }

ostream& operator<<(ostream& out, const User& user)
{
	out << user.login_;

	return out;
}
