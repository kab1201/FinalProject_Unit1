#pragma once

#include <exception>
#include <string>
#include <iostream>
using namespace std;

class UserException : public exception
{
private:
	string error_;

public:
	explicit UserException(const string& error);

	virtual const char* what() const override;
};

class User
{
private:
	const string login_;
	string password_;
	string name_;

public:
	User(const string& login, const string& password);
	User(const string& login, const string& password, const string& name);

	~User() = default;

	const string& getLogin() const;

	const string& getPassword() const;
	void setPassword(const string& newPassword);

	const string& getName() const;
	void setName(const string& newName);

	friend ostream& operator<<(ostream& out, const User& user);
};

