#include "Display.h"

//����������� �������
Display::Display(Base* pHeadObject, string objectName) : Base(pHeadObject, objectName) {}

//����� ��������� ���������� ������ �� �����
string Display::PrintHandler(string info)
{
	cout << info;
	return "";
}