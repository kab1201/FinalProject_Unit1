#include "Chat.h"

int main()
{
	Chat chat{};

	do
	{
		chat.chatMenu();

		while (chat.getPtrCurrentUser())
		{
			if (!chat.getIsToolsMenu())
				chat.userMenu();
			else
				chat.toolsMenu();
		}

	} while (chat.getIsWork());

	return 0;
}