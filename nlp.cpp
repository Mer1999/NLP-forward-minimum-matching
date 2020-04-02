/*
Program Name:���ڻ�е�ִʵ����ķִ�ϵͳ
Function:ʹ���������ƥ���㷨���ܹ�ʶ������� ���� ��ĸ
Author:Mu Ronghao
Student Number:1752117
*/
#include <iostream>
#include<string>
#include<fstream>
#include<unordered_map>

using namespace std;

#define DICTPATH "dictionary.txt" //�ֵ��ļ���
#define DIVISIONPATH "division.txt"//�ָ���ļ���
#define NUMLETTERPATH "numletter.txt"//������ĸ�ļ���
#define PENDINGPATH "pend.txt"//�������ļ���
#define SPACE "  "

int MAXLEN = 0;//�ֵ�������������
unordered_map<string, int>nlpmap;//��Ŵ����Ĺ�ϣ��
unordered_map<string, int>divmap;//��ŷָ�ʵĹ�ϣ��
unordered_map<string, int>numlettermap;//���������ĸ�Ĺ�ϣ��


//��ȡ�ֵ�������������
void getMaxlen()
{
	string str;
	ifstream fin;
	fin.open(DICTPATH);
	while (!fin.eof())
	{
		getline(fin, str);
		//�жϵ�ǰ�еĴ��������Ƿ��MAXLEN���������������Ϊ�˳���
		if (str.length() > MAXLEN)
		{
			MAXLEN = str.length();
		}
	}
	//cout << "MAXLEN=" << MAXLEN << endl;
	fin.close();
}

//��ȡ�ֵ��ļ�
void readDictionary()
{
	cout << "���ڻ�е�ִʵ����ķִ�ϵͳ" << endl;
	cout << "--������Ӵʿ⡭��" << endl;
	string str;

	ifstream fin;
	fin.open(DICTPATH);
	while (!fin.eof())
	{
		getline(fin, str);
		//cout << str << endl;
		nlpmap.insert(make_pair(str, 1));//�����������ϣ����
	}
	fin.close();

	ifstream fin2;
	fin2.open(DIVISIONPATH);
	while (!fin2.eof())
	{
		getline(fin2, str);
		divmap.insert(make_pair(str, 1));//���ָ�ʴ����ϣ����
	}
	fin2.close();

	ifstream fin3;
	fin3.open(NUMLETTERPATH);
	while (!fin3.eof())
	{
		getline(fin3, str);
		numlettermap.insert(make_pair(str, 1));//��������ĸ�����ϣ����
	}
	fin3.close();

	cout << "--�ʿ���ӳɹ�����" << endl << endl;
}

//������Сƥ��
void forwardMinimum()
{
	string str;
	ifstream fin;
	fin.open(PENDINGPATH);
	string chinese;//�����ֶ�
	int i; //ƫ����
	string afterpend;
	int length = 0;//�������ֶγ���
	int j;
	int deadline;//���ӽ���λ��
	int isover;//���仰�Ƿ�ִʽ�����־
	int sentencenum = 0;//�ڼ��仰
	int ismark;//���ָ�
	int isnumletter;//������ĸ�ָ�

	while (!fin.eof())
	{
		sentencenum++;
		getline(fin, str);//��ȡtxtһ�� �൱��һ���ı�
		cout << "��" << sentencenum << "��ԭ�ģ�" << endl << endl;
		cout << str << endl << endl;
		cout << "��" << sentencenum << "�ηִʴ����" << endl << endl;

		//��ʼ��
		i = 0;
		isover = 0;
		ismark = 0;
		isnumletter = 0;
		afterpend = "";//��afterpend���
		str += "@";//�������ӽ�����־

		//ÿ��ѭ������һ�仰���������Ϊֹ
		while (1)
		{
			//��ʼ��
			ismark = 0;
			isnumletter = 0;
			j = i;

			//��isover=1��˵�����仰�Ѿ��������������ѭ��
			if (isover)
				break;

			//���ȼ��㴿�����ı�������һ��������/����/��ĸ���ĳ��� length
			while (1)
			{
				//���Ϊֹ
				if (divmap.find(str.substr(j, 2)) != divmap.end())
				{
					length = j - i;//�õ��������ı�����
					deadline = j;
					ismark = 1;
					break;
				}
				//������ĸΪֹ
				else if (numlettermap.find(str.substr(j, 1)) != numlettermap.end())
				{
					length = j - i;
					deadline = j;
					isnumletter = 1;
					break;
				}
				//����Ϊ�����֣��Ƶ���һ��
				else
					j += 2;
			}

			//�������Ϊ0��˵��������/��ĸ/�����ֱ�������������һ��ѭ��
			if (length == 0)
			{
				if (ismark)
				{
					afterpend += str.substr(i, 2)+SPACE;
					i+=2;
				}
				else if (isnumletter)
				{
					afterpend += str.substr(i, 1);
					if (numlettermap.find(str.substr(i + 1, 1)) == numlettermap.end())
					{
						afterpend += SPACE;
					}
					i++;
				}
				continue;
			}

			//�������Ϊ2��һ�������ַ�������ֱ�����
			if (length == 2)
			{
				if (ismark)
				{
					afterpend += str.substr(i, 2) + SPACE + str.substr(i + 2, 2) + SPACE;
					i += 4;
				}
				if (isnumletter)
				{
					afterpend += str.substr(i, 2) + SPACE + str.substr(i + 2, 1);
					if (numlettermap.find(str.substr(i + 3, 1)) == numlettermap.end())
					{
						afterpend += SPACE;
					}
					i += 3;
				}
				if (str.substr(i, 1) == "@")
				{
					isover = 1;
				}
				continue;
			}

			//����length>2������������С����
			else
			{
				j = i + 2;//����i�Ĵ�����
				while (1)
				{
					//����ﵽdeadline��û��ƥ�������򽫵������
					if (j == deadline)
					{
						//�ж��Ƿ������һ����
						if (nlpmap.find(str.substr(i, j - i)) != nlpmap.end())
						{
							afterpend += str.substr(i, j - i) + SPACE;
							i = j;
						}
						else
						{
							afterpend += str.substr(i, 2) + SPACE;
							i += 2;
							j = i + 2;
						}
					}

					//����ﵽMAXLEN��û��ƥ�������򽫵������
					if (j == i + MAXLEN)
					{
						afterpend += str.substr(i, 2) + SPACE;
						i += 2;
						j = i + 2;
					}

					//����ַ��Ѿ�������ϣ�����ѭ����������һ�仰
					if (i == deadline)
					{
						if (ismark)
						{
							afterpend += str.substr(i, 2) + SPACE;
							i += 2;
						}
						if (isnumletter)
						{
							afterpend += str.substr(i, 1);
							if (numlettermap.find(str.substr(i + 1, 1)) == numlettermap.end())
							{
								afterpend += SPACE;
							}
							i += 1;
						}
						//���Ѿ������仰��β������isover=1
						if (str.substr(i, 1) == "@")
							isover = 1;

						break;
					}

					//������Сƥ��
					if (nlpmap.find(str.substr(i, j - i)) != nlpmap.end())
					{
						afterpend += str.substr(i, j - i) + SPACE;
						i = j;
						if (i != deadline)
						{
							j = i + 2;
						}
					}
					else
					{
						j += 2;
					}
				}
			}
		}
		cout << afterpend << endl << endl;//����ִʺ����
	}
	fin.close();
}

int main()
{
	getMaxlen();
	readDictionary();
	forwardMinimum();
	system("pause");
	return 0;
}