#include "Operator.h"

Operator::Operator(Base* pHeadObject, string objectName) : Base(pHeadObject, objectName) {}

//����� �������� ������ � �������� ��������
void Operator::GetInputSignal(string& info)
{
	string input;
	getline(cin, input); // ��������� ����
	info = input;
}

//����� ����������
string Operator::GetInputHandler(string info)
{
	return info;
}
