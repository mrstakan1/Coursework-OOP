#include "Base.h"

//����������� ������
Base::Base(Base* pHeadObject, string objectName)
{
	//������������� �����
	this->objectName = objectName;
	this->pHeadObject = pHeadObject;

	if (pHeadObject != nullptr) // �������� ������ �� ������� ��������� - ��������� ����� � ����������� �������� 
		pHeadObject->subordinateObjects.push_back(this);
}

//����� ������������ ��������� �� �������� ������
Base* Base::GetHead()
{
	return pHeadObject;
}

//����� ��������������� �������� ������
void Base::SetHead(Base* pHead)
{
	pHeadObject = pHead;
}

//����� ��������������� ��� �������
void Base::SetName(string name)
{
	objectName = name;
}

//����� ������������ ��������� �������
int Base::GetState()
{
	return objState;
}

//����� ��������� ���������
void Base::SetState(int state)
{
	if (state == 0)
	{
		objState = state;
		//���� ����� ��� ����, ���, ���� ��������� ������ - ��������� ���� �����������
		for (int i = 0; i < subordinateObjects.size(); i++)
		{
			subordinateObjects[i]->SetState(state);
		}
	}

	if (pHeadObject != nullptr && pHeadObject->objState != 0)
		objState = state;
}

//����� ������������ ��� �������
string Base::GetName()
{
	return objectName;
}

//����� ������ �������
Base* Base::FindObject(Base* currentObject, string objectPath)
{
	Base* foundObj = nullptr;

	if (objectPath.length() > 0 && objectPath[0] == '/') // ���� � ������� � ������ /, �� ���� �� ����� ������� ���� ����� /
	{
		objectPath.erase(0, 1);

		if (objectPath == "")
			return this;
		else
			return this->FindObject(currentObject, objectPath);
	}

	if (objectPath.length() > 0) // ����� ������� �� ������������� ���������� (path_1/path_2)
	{
		int slashPos = objectPath.find('/');
		string tempName = objectPath;

		if (slashPos != string::npos) // ���� �� ������ ����, ������ �� �������� ��� �������, ������� �� ����
		{
			tempName.erase(slashPos, objectPath.size());
			objectPath.erase(0, slashPos);
		}
		else
		{
			objectPath = "";
		}

		//���� ����� ��� ������ ������� �� ����� ����� ���������� ��������, ����������� � ��� ���������
		for (int i = 0; i < currentObject->subordinateObjects.size(); i++) {
			if (currentObject->subordinateObjects[i]->GetName() == tempName) {
				foundObj = currentObject->subordinateObjects[i];
				if (objectPath != "") {
					return foundObj->FindObject(foundObj, objectPath);
				}
				return foundObj;
			}
		}
	}
	return foundObj;
}

//����� ��������� �����
void Base::SetConnect(string signalName, TYPE_SIGNAL pSignal, Base* pTarget, TYPE_HANDLER pHandler)
{
	// ���� ��� ���������� ���������� ������������ �����
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i]->signalName == signalName)
			return;
	}

	Connection* bond = new Connection();                 // �������� ������� ��������� ��� �������� ���������� � ����� �����
	bond->signalName = signalName;
	bond->pSignal = pSignal;
	bond->pTarget = pTarget;
	bond->pHandler = pHandler;

	connections.push_back(bond);          // ���������� ����� �����                 
}

//����� ������ �������
string Base::EmitSignal(string signalName, string command)
{
	string response = "null";

	if (objState == 0)
		return response;

	for (int i = 0; i < connections.size(); i++) // ���� �� ���� ������������
	{
		if (connections[i]->signalName == signalName)      // ����������� ����������� �����������
		{
			if (connections[i]->pSignal != nullptr)
			{
				(this->*connections[i]->pSignal)(command);      // ����� ������ �������
			}

			if (connections[i]->pHandler != nullptr)
			{
				response = (connections[i]->pTarget->*(connections[i]->pHandler))(command);      // ����� ������ �����������
			}

			break;
		}
	}

	return response;
}

//����� ������ ������ ��������
void Base::PrintTree(string tab)
{
	if (pHeadObject == nullptr)
		cout << endl << objectName << ((objState != 0) ? " is ready" : "is not ready");

	for (size_t i = 0; i < subordinateObjects.size(); i++)
	{
		cout << endl;
		cout << tab << subordinateObjects[i]->GetName() <<
			((subordinateObjects[i]->objState != 0) ? " is ready" : " is not ready");

		subordinateObjects[i]->PrintTree(tab + "\t");
	}
}