#include<iostream>
#include<vector>
#include<string>
using namespace std;

class cup {
public:
	int int_Capacity;//容量
	int int_NowContain;//现在的水量
	int int_WannaCapacity;//目标水量
	cup(int, int, int);
	cup();
	friend istream& operator>>(istream& in, cup& cup);//重载输入运算符
};

class situation {//状况类
public:
	int Poured_Cup_One, Poured_Cup_Two, father;//倒水的两个杯子以及它的父节点
	vector<int> Contain_Situation;//各个杯子的水量
	situation(int, int, int, vector<int>);
	friend ostream& operator<<(ostream& out, situation& s);//重载输出运算符
};

class pour_water {
public:
	void pour(int k);//倒水过程
	bool Pour_Water(int, int, int);//倒水这个动作
	void erase_last_situation();//删除最后一个状况
	bool Cin_Cup_List();
	void Cout_Situation_Stack();
	bool Is_Same();//最后这个情况是否和前面出现过的情况相同
	bool Can_Pour_Water(int, int);//是否可以倒水
	bool Is_Finished();//判断是否已经完成倒水
	pour_water(vector<cup>);
	pour_water();
private:
	int index;//正在查找的状况列表中的位置
	vector<cup> Cup_List;//杯子列表
	vector<situation> Situation_Stack;//状况列表
};

cup::cup(int Capacity, int NowContain, int WannaCapacity) : int_Capacity(Capacity), int_NowContain(NowContain), int_WannaCapacity(WannaCapacity) {}
cup::cup() : int_Capacity(0), int_NowContain(0), int_WannaCapacity(0) {}
situation::situation(int CupOne, int CupTwo, int ifather, vector<int> NowSituation) : Poured_Cup_One(CupOne), Poured_Cup_Two(CupTwo), father(ifather), Contain_Situation(NowSituation) {}

pour_water::pour_water(vector<cup> new_list) {
	vector<int> situate;
	Cup_List = new_list;
	for (unsigned int i = 0; i < Cup_List.size(); i++) {//把所有杯子的现有水量填入situate这个临时变量中
		situate.push_back(Cup_List.at(i).int_NowContain);
	}
	situation* s = new situation(0, 0, -1, situate);
	Situation_Stack.push_back(*s);
	delete(s);
	index = 0;
}

pour_water::pour_water() {
	while (!Cin_Cup_List()) {
		cout << "输入数据错误！请重新输入！！！" << endl << endl;
	}
	vector<int> situate;
	for (unsigned int i = 0; i < Cup_List.size(); i++) {//把所有杯子的现有水量填入situate这个临时变量中
		situate.push_back(Cup_List.at(i).int_NowContain);
	}
	situation* s = new situation(0, 0, 0, situate);
	Situation_Stack.push_back(*s);
	delete(s);
	index = 0;
}

bool pour_water::Is_Same() {//判断是否重复
	unsigned int i, j;
	int int_is_same_num;
	i = 0;
	while (i < Situation_Stack.size() - 1) {
		int_is_same_num = j = 0;
		while (j < Situation_Stack.at(i).Contain_Situation.size()) {
			if (Situation_Stack.at(i).Contain_Situation.at(j) == Situation_Stack.at(Situation_Stack.size() - 1).Contain_Situation.at(j)) {
				int_is_same_num++;
			}
			if (int_is_same_num == Situation_Stack.at(i).Contain_Situation.size()) {
				return true;
			}
			j++;
		}
		i++;
	}
	return false;
}

bool pour_water::Can_Pour_Water(int i, int j) {//这两个杯子能否倒水
	if (Situation_Stack.at(index).Poured_Cup_One == j&&Situation_Stack.at(index).Poured_Cup_Two == i) {//这次倒水的两个杯子不能和上一次的相同
		return false;
	}
	if (Situation_Stack.at(index).Poured_Cup_One == i&&Situation_Stack.at(index).Poured_Cup_One == j) {//这次倒水的两个杯子不能和上一次的相同
		return false;
	}
	if (Cup_List.at(j).int_Capacity == Cup_List.at(j).int_NowContain) {//第二个杯子满了
		return false;
	}
	if (Cup_List.at(i).int_NowContain == 0) {//第一个杯子为空z
		return false;
	}
	return true;
}

bool pour_water::Is_Finished() {//是否完成
	for (unsigned int i = 0; i < Cup_List.size(); i++) {
		if (Cup_List.at(i).int_NowContain != Cup_List.at(i).int_WannaCapacity) {
			return false;
		}
	}
	return true;
}

void pour_water::pour(int k) {//倒水过程
	//cout << k;
	unsigned int i, j;
	for (i = 0; i < Cup_List.size(); i++) {
		for (j = 0; j < Cup_List.size(); j++) {
			if (i != j) {
				if (Pour_Water(i, j, k)) {//倒水
					if (Is_Same()) {//如果和以前的重复，那么删除这个节点
						erase_last_situation();
					}
					else {
						if (Is_Finished()) {//如果达到了想要水量
							break;
						}
						else {
							for (int i = 0; i < Cup_List.size(); i++) {//把杯子的水量改回父节点储存的状态
								Cup_List.at(i).int_NowContain = Situation_Stack.at(index).Contain_Situation.at(i);
							}
						}
					}
				}
			}
		}
	}
	if (index < Situation_Stack.size() && !Is_Finished()) {//递归，重复这个过程
		pour(++index);
	}
	return;
}

bool pour_water::Pour_Water(int Cup_One, int Cup_Two, int father) {
	vector<int> situate;
	if (Can_Pour_Water(Cup_One, Cup_Two)) {//如果这两个杯子可以倒水
		if ((Cup_List.at(Cup_One).int_NowContain + Cup_List.at(Cup_Two).int_NowContain) < Cup_List.at(Cup_Two).int_Capacity) {//如果两个杯子的水量相加小于第二个杯子的总水量
			Cup_List.at(Cup_Two).int_NowContain = Cup_List.at(Cup_One).int_NowContain + Cup_List.at(Cup_Two).int_NowContain;//第二个杯子现有水量变成第一个杯子的水量加上第二个杯子的
			Cup_List.at(Cup_One).int_NowContain = 0;//那么第一个杯子全倒
		}
		else {
			Cup_List.at(Cup_One).int_NowContain = Cup_List.at(Cup_One).int_NowContain - (Cup_List.at(Cup_Two).int_Capacity - Cup_List.at(Cup_Two).int_NowContain);//否者第一个杯子的水量等于两个杯子的总水量减去第二个杯子的容量
			Cup_List.at(Cup_Two).int_NowContain = Cup_List.at(Cup_Two).int_Capacity;//第二个杯子被装满
		}
		for (unsigned int i = 0; i < Cup_List.size(); i++) {//把所有杯子的现有水量填入situate这个临时变量中
			situate.push_back(Cup_List.at(i).int_NowContain);
		}
		situation* s = new situation(Cup_One, Cup_Two, father, situate);
		Situation_Stack.push_back(*s);
		delete(s);
		return true;
	}
	return false;
}

void pour_water::erase_last_situation() {//删除最后一个结点并且把杯子的水量改回父节点储存的状态
	Situation_Stack.erase(Situation_Stack.end() - 1);
	for (int i = 0; i < Cup_List.size(); i++) {
		Cup_List.at(i).int_NowContain = Situation_Stack.at(index).Contain_Situation.at(i);
	}
}

bool pour_water::Cin_Cup_List() {
	int i = 1, Temp_Contain, Temp_wanna;
	cout << "\u8bf7\u5206\u522b\u8f93\u5165\u7b2c"+to_string(i++)+"\u4e2a\u676f\u5b50\u7684\u5bb9\u91cf\u0020\u5f53\u524d\u6c34\u91cf\u0020\u76ee\u6807\u6c34\u91cf\u0028\u505c\u6b62\u8f93\u5165\u5219\u5728\u8f93\u5165\u676f\u5b50\u5bb9\u91cf\u7684\u65f6\u5019\u8f93\u5165\u0030\u0029\u0063\u006f\u0064\u0065\u0064\u0020\u0062\u0079\u0020\u0063\u0075\u006e\u007a\u0061\u006f" << endl;
	cup* Cup = new cup();
	cin >> *Cup;
	while (Cup->int_Capacity != 0) {
		Cup_List.push_back(*Cup);
		cout << endl << "\u8bf7\u5206\u522b\u8f93\u5165\u7b2c" + to_string(i++) + "\u4e2a\u676f\u5b50\u7684\u5bb9\u91cf\u0020\u5f53\u524d\u6c34\u91cf\u0020\u76ee\u6807\u6c34\u91cf\u0028\u505c\u6b62\u8f93\u5165\u5219\u5728\u8f93\u5165\u676f\u5b50\u5bb9\u91cf\u7684\u65f6\u5019\u8f93\u5165\u0030\u0029\u0063\u006f\u0064\u0065\u0064\u0020\u0062\u0079\u0020\u0063\u0075\u006e\u007a\u0061\u006f" << endl;
		cin >> *Cup;
	}
	Temp_Contain = Temp_wanna = 0;
	for (int j = 0; j < Cup_List.size(); j++) {
		Temp_Contain += Cup_List.at(j).int_NowContain;
		Temp_wanna += Cup_List.at(j).int_WannaCapacity;
	}
	if (Cup_List.size() > 0 && Temp_Contain == Temp_wanna) {
		return true;
	}
	Cup_List.clear();
	return false;
}

void pour_water::Cout_Situation_Stack() {//输出步骤
	if (Situation_Stack.size() > index) {
		vector<situation> s;
		s.push_back(Situation_Stack.at(0));
		int temp_index;
		temp_index = Situation_Stack.size() - 1;
		while (temp_index > 0) {
			s.insert(s.begin() + 1, Situation_Stack.at(temp_index));
			temp_index = Situation_Stack.at(temp_index).father;
		}
		cout << endl << "共" << s.size() - 1 << "个步骤" << endl;
		for (unsigned int i = 0; i < s.size(); i++) {
			cout << s.at(i) << endl;
		}
	}
	else {
		cout << "无解!" << endl;
	}
}

istream& operator>>(istream & in, cup & cup) {
	in >> cup.int_Capacity;
	if (cup.int_Capacity == 0) {
		cup.int_NowContain = 0;
		cup.int_WannaCapacity = 0;
	}
	else {
		in >> cup.int_NowContain;
		in >> cup.int_WannaCapacity;
	}
	return in;

}

ostream & operator<<(ostream & out, situation & s) {
	out << "从第" << s.Poured_Cup_One << "个杯子倒入" << s.Poured_Cup_Two << "个杯子" << endl;
	for (unsigned int i = 0; i < s.Contain_Situation.size(); i++) {
		out << "第 " << i << " 个杯子的水量是: " << s.Contain_Situation.at(i) << "\t" << endl;
	}
	return out;
}
void main() {
	/*vector<cup> new_list;
	cup* c = new cup(21, 21, 7);
	cup* d = new cup(11, 0, 7);
	cup* e = new cup(8, 0, 7);
	cup* f = new cup(5, 0, 0);
	new_list.push_back(*c);
	new_list.push_back(*d);
	new_list.push_back(*e);
	new_list.push_back(*f);
	pour_water* p = new pour_water(new_list);*/
	pour_water* p = new pour_water();
	p->pour(0);
	p->Cout_Situation_Stack();
	system("pause");
}