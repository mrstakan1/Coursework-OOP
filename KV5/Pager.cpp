#include "Pager.h"

//����������� ������
Pager::Pager(Base* pHeadObject, string objectName, int id) : Base(pHeadObject, objectName)
{
	_iD = id;
}

//����� ������������ id ��������
int Pager::GetId()
{
	return _iD;
}

//����� �������� ���������� � �������� ��������
void Pager::PagerStatusSignal(string& signal)
{
	signal = "\nThe list of messages sent by the pager " + to_string(_iD);

	//���� �������������� ���������� �� ������������ ����������
	for (int i = 0; i < sentMessages.size(); i++)
	{
		signal += "\n" + to_string(sentMessages[i].tickMadeSent) + " "
			+ to_string(sentMessages[i].idTo) + " "
			+ sentMessages[i].message;
	}

	signal += "\nList of received messages by the pager " + to_string(_iD);

	//���� �������������� ���������� �� ���������� ����������
	for (int i = 0; i < receivedMessages.size(); i++)
	{
		signal += "\n" + to_string(receivedMessages[i].tickMadeSent) + " "
			+ to_string(receivedMessages[i].idFrom) + " "
			+ receivedMessages[i].message;
	}

}

//����� ������������ ���-�� ������������ ��������� ���������
int Pager::GetSentMessagesCount()
{
	return sentMessages.size();
}

//����� ������������ ���-�� ���������� ��������� ���������
int Pager::GetReceivedMessagesCount()
{
	return receivedMessages.size();
}

//����� ��������������� ��������� ��������
string Pager::SetPagerStateHandler(string state)
{
	if (state == "busy")
		isBusy = true;

	if (state == "is not busy")
		isBusy = false;

	return "";
}

//����� ������������ ��������� ��������
string Pager::GetPagerState(string state)
{
	return (isBusy == true) ? "busy" : "is not busy";
}


//����� ������� ��������� ���������� ���������
string Pager::AddReceivedMessageHandler(string message)
{
	Message receivedMessage;
	string tempMessage = message;

	//�������� id �����������
	receivedMessage.idFrom = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� id ����������
	receivedMessage.idTo = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage = tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� ���� �����������
	receivedMessage.tickMadeSent = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� ���� ���������
	receivedMessage.message = tempMessage;

	receivedMessages.push_back(receivedMessage);

	SortMessages(); // �������� ����� ���������� ���������
	return "";
}

//����� ������� ��������� ������������ ��������� � ���������� ������ � ����������
void Pager::SendMessageSignal(string& message)
{
	Message sentMessage;
	string tempMessage = message;

	//�������� id �����������
	sentMessage.idFrom = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� id ����������
	sentMessage.idTo = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage = tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� id ���� �����������
	sentMessage.tickMadeSent = stoi(tempMessage.substr(0, tempMessage.find(" ") + 1));
	tempMessage.erase(0, tempMessage.find(" ") + 1);

	//�������� ���������
	sentMessage.message = tempMessage;

	sentMessages.push_back(sentMessage);

	SortMessages(); // �������� ����� ���������� ���������
}

//����� ���������� ���������
void Pager::SortMessages()
{
	if (sentMessages.size() > 1) // ���� ������������ ��������� ������ 1, ��������� ���� ��� ����������� ����������
	{
		for (int i = 0; i < sentMessages.size(); i++)
		{
			for (int j = 0; j < sentMessages.size() - (i + 1); j++)
				if (sentMessages[j].idFrom > sentMessages[j + 1].idFrom)
					swap(sentMessages[j], sentMessages[j + 1]);
		}
	}

	if (receivedMessages.size() > 1) // ���� ���������� ��������� ������ 1, ��������� ���� ��� ����������� ����������
	{
		for (int i = 0; i < receivedMessages.size(); i++)
		{
			for (int j = 0; j < receivedMessages.size() - (i + 1); j++)
				if (receivedMessages[j].idFrom > receivedMessages[j + 1].idFrom)
					swap(receivedMessages[j], receivedMessages[j + 1]);
		}
	}
}
