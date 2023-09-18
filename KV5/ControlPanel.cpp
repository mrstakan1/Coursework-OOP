#include "ControlPanel.h"

ControlPanel::ControlPanel(Base* pHeadObject, string objectName) : Base(pHeadObject, objectName) {};

string ControlPanel::CommandProcessingHandler(string command)
{
	//�������� ��������� �� ������� ������
	Base* Controller = this;
	Base* Reader = FindObject(Controller->GetHead(), "/Reader");
	Base* System = Controller->GetHead();
	string reply;
	int currentTick = stoi(Controller->EmitSignal("GetCurrentTick"));


	if (command == "Turn off the system") // ���� ������� Turn off the system - ��������� �������
	{
		Controller->EmitSignal("PrintOut", "\nTurn off the ATM");
		return command;
	}

	//������� ����� �������, ���������� ����������
	string commandClone = command;
	string tempCommand;
	tempCommand = commandClone.substr(0, 15);

	if (tempCommand == "Send a message ") // ���� ������� Send a message - ������� ��������� ��������� ���������
	{
		if (IsLastCommandWasSend == false) // ���� �� ����� ���� ������� �� send a message - ������ �������� ����� ���� � ����������� ������ ��������� �������
		{
			Controller->EmitSignal("TickStep");
			ProceedTypingList();
			ProceedQueue();
		}
		Base* Controller = this;

		//�������������, ��� ���������
		IsLastCommandWasSend = true;
		string tempCommand = commandClone.substr(15);

		TrySendAMessage(tempCommand);
	}
	else
	{
		//������������� ����, �������� �� ���, ���� �� ��������� ������� = send a message �������� false
		IsLastCommandWasSend = false;
	}


	// ����� ���� ���� � ��� �� �������� send a message
	if (IsLastCommandWasSend == false)
	{
		Controller->EmitSignal("TickStep");
		currentTick = stoi(Controller->EmitSignal("GetCurrentTick"));

		//=================================
		if (command == "Display the system status")
		{
			System->EmitSignal("GetSystemStatus"); // ����� ������� ������� ������� �� ����� ���������� � �������
		}

		//=================================
		if (command.substr(0, 25) == "Display the pager status ") // ���� ������� Display the pager status - ����� ������� ������� ������ ���������� � ������ ��������, ���� �� �� � ������
		{
			int pagerId = stoi(command.substr(25));
			Base* pPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(pagerId));
			bool isPagerBusy = (Controller->EmitSignal("GetPagerState_" + to_string(pagerId)) == "busy" ? true : false);

			if (isPagerBusy == false)
				pPager->EmitSignal("PagerStatus");
		}

		//����� �������, ������� ��������� ������ ��������� �������
		ProceedTypingList();
		ProceedQueue();
	}

	return "";
}

//����� �������������� ������� ��������� ���������
void ControlPanel::ProceedQueue()
{
	//�������� ��������� �� ������ ������� ������� � �������� ���������� � ������� �����
	Base* Controller = this;
	Base* Reader = FindObject(this->GetHead(), "Reader");
	int currentTick = stoi(Controller->EmitSignal("GetCurrentTick"));


	//���� ������� �� ������ - �� ���������� ���������
	if (messagesQueue.empty() == false)
	{
		Message message = messagesQueue.back();

		if (message.createTick == currentTick) // ���� ��������� ������ ����� � ���� ����� - �� ��� �� ����������
			return;

		//������������ ��������� ������� �� ����� ��������� ���������
		string newMessage = to_string(message.idFrom) + " " + to_string(message.idTo) +
			" " + to_string(message.tickMadeSent) + " " + message.message;

		if (CheckNumber(message.idTo) == true) // ���������, ���� ���������� ���������� - ���������� ��� ���������
		{
			this->EmitSignal("EditProcessedMessages");
			Reader->EmitSignal("ReceiveMessage_" + to_string(message.idTo), newMessage);
		}

		messagesQueue.pop_back();
	}
}

//����� ������� ������������ ��������������������� ���� ��������� ��������������
void ControlPanel::ProceedTypingList()
{
	//�������� ��������� �� ������ ������� ������� � �������� �������� �����
	Base* Controller = this;
	int currentTick = stoi(Controller->EmitSignal("GetCurrentTick"));


	//������ ����� ��� �������� ���� ��������� ������� ��������� � ������
	for (auto it = typingMessages.rbegin(); it != typingMessages.rend(); )
	{
		if (it->ticksLeft == 1) // ��������� ��������� � ���� �����
		{
			//����� ������� ������� ��������� ������������� ��������� ����������� � ���������� - ���� ���, ����� ������� �������������� ������
			CheckNumberWithSignal(it->idFrom);
			CheckNumberWithSignal(it->idTo);

			//������������� ����, � ������� ����� ���������� ���������
			it->tickMadeSent = currentTick + messagesQueue.size();
			it->createTick = currentTick;

			//�������� ��������� �� ������� ������ ���������
			Base* senderPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(it->idFrom));
			Base* receiverPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(it->idTo));

			//������������ ��������� ������� �� ����� ��������� ���������
			string newMessage = to_string(it->idFrom) + " " + to_string(it->idTo) +
				" " + to_string(it->tickMadeSent) + " " + it->message;

			messagesQueue.push_back(*it);

			//����� �������� ������� ������� ������ "�����" � �������� � �������� ��������� ���������
			senderPager->EmitSignal("SetPagerState_" + to_string(it->idFrom), "is not busy");
			senderPager->EmitSignal("SendMessage_" + to_string(it->idFrom), newMessage);

			//���� ���������� ����������, �� �������� ����� ������������ ��� ���������
			if (CheckNumber(it->idTo) == true)
				Controller->EmitSignal("SetProcessingMessages");

			it = decltype(it)(typingMessages.erase(next(it).base())); // ������� ������� �������� ������ �� ������ ���������� ���������
		}
		else
		{
			it->ticksLeft--; // ������ ����, ������������ ������ ��� 10 �������� ���������
			it++;
		}
	}
}

//����� ��� �������� ������ ������� ���������, ������ ������� �������� �� �������
string ControlPanel::SetIds(string numbers)
{
	//���� ����� ��� ��������� ������� ���������
	while (numbers.find(" ") != string::npos)
	{
		//� ������ ��������� ��������� ����� �����
		iDs.push_back(stoi(numbers.substr(0, numbers.find(" ") + 1)));
		numbers.erase(0, numbers.find(" ") + 1);
	}

	return "";
}

//����� ����������� ������������� ������ �������� � �������� ��������������� ������
bool ControlPanel::CheckNumberWithSignal(int number)
{
	//���� ����� ��� ������� �� ���� ������� ���������
	for (int i = 0; i < iDs.size(); i++)
	{
		//���� ������� ����������, ���������� �������� ������
		if (number == iDs[i])
			return true;
	}

	//����� ������� ������ �� ����� � ����������� � ���, ��� ������� �� ����������
	this->EmitSignal("PrintOut", "\nSubscriber " + to_string(number) + " not found");
	return false;
}

//����� ����������� ������������� ������ �������� � �������� ��������������� ������
bool ControlPanel::CheckNumber(int number)
{
	//���� ����� ��� ������� �� ���� ������� ���������
	for (int i = 0; i < iDs.size(); i++)
	{

		if (number == iDs[i]) //���� ������� ����������, ���������� �������� ������
			return true;
	}


	return false;
}

//����� �������� ��������� ���������
void ControlPanel::TrySendAMessage(string message)
{
	Base* Controller = this;
	string tempCommand = message;

	bool isSenderExist, isReceieverExist;

	//---------------------------��������� ��������� �� �����
	int senderId = stoi(tempCommand.substr(0, tempCommand.find(" ") + 1));
	tempCommand = tempCommand.erase(0, tempCommand.find(" ") + 1);

	int receiverId = stoi(tempCommand.substr(0, tempCommand.find(" ") + 1));
	tempCommand = tempCommand.erase(0, tempCommand.find(" ") + 1);
	string messageText = tempCommand;
	//-----------------------------

	//�������� ��������� �� ������ ��������
	Base* senderPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(senderId));
	Base* receiverPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(receiverId));

	//��������� ����� �� ������� ��� �������� ���������
	bool isSenderPagerBusy = (Controller->EmitSignal("GetPagerState_" + to_string(senderId)) == "busy" ? true : false);


	if (isSenderPagerBusy == false) // ���� ������� �� � ������, ���������� ���������
	{
		if (messageText.size() >= 1 && messageText.size() <= 30) // ���� ��������� ����������� �������
		{
			Message newMessage;

			//������������� ��������� � ��������� Message
			newMessage.message = messageText;
			newMessage.idFrom = senderId;
			newMessage.idTo = receiverId;
			int ticksLeft = messageText.size() / 10 + 1;
			newMessage.ticksLeft = ticksLeft;

			//�������� ��������� �� ������ ��������, ������� ���������� ���������
			Base* pPager = FindObject(this->GetHead(), "/Reader/Pager_" + to_string(senderId));

			if (messageText.size() > 10) // ���� ��������� ������� ������ 10, �� ������� ����� ��� ������
			{
				pPager->EmitSignal("SetPagerState_" + to_string(senderId), "busy");
			}

			//���������� ������ ��������� � "�����"
			typingMessages.push_back(newMessage);
		}
	}

	//������� ��������� ���������
	//TrySendAMessage(commandClone);
}