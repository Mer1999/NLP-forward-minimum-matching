/*
Program Name:基于机械分词的中文分词系统
Function:使用正向最大匹配算法，能够识别标点符号 数字 字母
Author:Mu Ronghao
Student Number:1752117
*/
#include <iostream>
#include<string>
#include<fstream>
#include<unordered_map>

using namespace std;

#define DICTPATH "dictionary.txt" //字典文件名
#define DIVISIONPATH "division.txt"//分割词文件名
#define NUMLETTERPATH "numletter.txt"//数字字母文件名
#define PENDINGPATH "pend.txt"//待处理文件名
#define SPACE "  "

int MAXLEN = 0;//字典内最大词条长度
unordered_map<string, int>nlpmap;//存放词条的哈希表
unordered_map<string, int>divmap;//存放分割词的哈希表
unordered_map<string, int>numlettermap;//存放数字字母的哈希表


//获取字典内最大词条长度
void getMaxlen()
{
	string str;
	ifstream fin;
	fin.open(DICTPATH);
	while (!fin.eof())
	{
		getline(fin, str);
		//判断当前行的词条长度是否比MAXLEN更大，若更大则更新为此长度
		if (str.length() > MAXLEN)
		{
			MAXLEN = str.length();
		}
	}
	//cout << "MAXLEN=" << MAXLEN << endl;
	fin.close();
}

//读取字典文件
void readDictionary()
{
	cout << "基于机械分词的中文分词系统" << endl;
	cout << "--正在添加词库……" << endl;
	string str;

	ifstream fin;
	fin.open(DICTPATH);
	while (!fin.eof())
	{
		getline(fin, str);
		//cout << str << endl;
		nlpmap.insert(make_pair(str, 1));//将词条存入哈希表中
	}
	fin.close();

	ifstream fin2;
	fin2.open(DIVISIONPATH);
	while (!fin2.eof())
	{
		getline(fin2, str);
		divmap.insert(make_pair(str, 1));//将分割词存入哈希表中
	}
	fin2.close();

	ifstream fin3;
	fin3.open(NUMLETTERPATH);
	while (!fin3.eof())
	{
		getline(fin3, str);
		numlettermap.insert(make_pair(str, 1));//将数字字母存入哈希表中
	}
	fin3.close();

	cout << "--词库添加成功！！" << endl << endl;
}

//正向最小匹配
void forwardMinimum()
{
	string str;
	ifstream fin;
	fin.open(PENDINGPATH);
	string chinese;//保存字段
	int i; //偏移量
	string afterpend;
	int length = 0;//纯中文字段长度
	int j;
	int deadline;//句子结束位置
	int isover;//本句话是否分词结束标志
	int sentencenum = 0;//第几句话
	int ismark;//标点分割
	int isnumletter;//数字字母分割

	while (!fin.eof())
	{
		sentencenum++;
		getline(fin, str);//读取txt一行 相当于一段文本
		cout << "第" << sentencenum << "段原文：" << endl << endl;
		cout << str << endl << endl;
		cout << "第" << sentencenum << "段分词处理后：" << endl << endl;

		//初始化
		i = 0;
		isover = 0;
		ismark = 0;
		isnumletter = 0;
		afterpend = "";//将afterpend清空
		str += "@";//在最后添加结束标志

		//每次循环处理一句话，到间隔符为止
		while (1)
		{
			//初始化
			ismark = 0;
			isnumletter = 0;
			j = i;

			//若isover=1，说明本句话已经处理结束，跳出循环
			if (isover)
				break;

			//首先计算纯中文文本（到下一个标点符号/数字/字母）的长度 length
			while (1)
			{
				//标点为止
				if (divmap.find(str.substr(j, 2)) != divmap.end())
				{
					length = j - i;//得到纯中文文本长度
					deadline = j;
					ismark = 1;
					break;
				}
				//数字字母为止
				else if (numlettermap.find(str.substr(j, 1)) != numlettermap.end())
				{
					length = j - i;
					deadline = j;
					isnumletter = 1;
					break;
				}
				//否则为中文字，移到下一个
				else
					j += 2;
			}

			//如果长度为0，说明是数字/字母/标点则直接提出，进入下一个循环
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

			//如果长度为2（一个中文字符），则直接提出
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

			//否则（length>2）进行正向最小划分
			else
			{
				j = i + 2;//保持i的纯洁性
				while (1)
				{
					//如果达到deadline还没有匹配结果，则将单字提出
					if (j == deadline)
					{
						//判断是否是最后一个词
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

					//如果达到MAXLEN还没有匹配结果，则将单字提出
					if (j == i + MAXLEN)
					{
						afterpend += str.substr(i, 2) + SPACE;
						i += 2;
						j = i + 2;
					}

					//如果字符已经划分完毕，跳出循环，处理下一句话
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
						//若已经到本句话结尾，则置isover=1
						if (str.substr(i, 1) == "@")
							isover = 1;

						break;
					}

					//正向最小匹配
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
		cout << afterpend << endl << endl;//输出分词后段落
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