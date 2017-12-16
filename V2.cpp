#include<iostream>
#include<vector>
#include<string>
using namespace std;

class cup {
public:
	int int_Capacity;//����
	int int_NowContain;//���ڵ�ˮ��
	int int_WannaCapacity;//Ŀ��ˮ��
	cup(int, int, int);
	cup();
	friend istream& operator>>(istream& in, cup& cup);//�������������
};

class situation {//״����
public:
	int Poured_Cup_One, Poured_Cup_Two, father;//��ˮ�����������Լ����ĸ��ڵ�
	vector<int> Contain_Situation;//�������ӵ�ˮ��
	situation(int, int, int, vector<int>);
	friend ostream& operator<<(ostream& out, situation& s);//������������
};

class pour_water {
public:
	void pour(int k);//��ˮ����
	bool Pour_Water(int, int, int);//��ˮ�������
	void erase_last_situation();//ɾ�����һ��״��
	bool Cin_Cup_List();
	void Cout_Situation_Stack();
	bool Is_Same();//����������Ƿ��ǰ����ֹ��������ͬ
	bool Can_Pour_Water(int, int);//�Ƿ���Ե�ˮ
	bool Is_Finished();//�ж��Ƿ��Ѿ���ɵ�ˮ
	pour_water(vector<cup>);
	pour_water();
private:
	int index;//���ڲ��ҵ�״���б��е�λ��
	vector<cup> Cup_List;//�����б�
	vector<situation> Situation_Stack;//״���б�
};

cup::cup(int Capacity, int NowContain, int WannaCapacity) : int_Capacity(Capacity), int_NowContain(NowContain), int_WannaCapacity(WannaCapacity) {}
cup::cup() : int_Capacity(0), int_NowContain(0), int_WannaCapacity(0) {}
situation::situation(int CupOne, int CupTwo, int ifather, vector<int> NowSituation) : Poured_Cup_One(CupOne), Poured_Cup_Two(CupTwo), father(ifather), Contain_Situation(NowSituation) {}

pour_water::pour_water(vector<cup> new_list) {
	vector<int> situate;
	Cup_List = new_list;
	for (unsigned int i = 0; i < Cup_List.size(); i++) {//�����б��ӵ�����ˮ������situate�����ʱ������
		situate.push_back(Cup_List.at(i).int_NowContain);
	}
	situation* s = new situation(0, 0, -1, situate);
	Situation_Stack.push_back(*s);
	delete(s);
	index = 0;
}

pour_water::pour_water() {
	while (!Cin_Cup_List()) {
		cout << "�������ݴ������������룡����" << endl << endl;
	}
	vector<int> situate;
	for (unsigned int i = 0; i < Cup_List.size(); i++) {//�����б��ӵ�����ˮ������situate�����ʱ������
		situate.push_back(Cup_List.at(i).int_NowContain);
	}
	situation* s = new situation(0, 0, 0, situate);
	Situation_Stack.push_back(*s);
	delete(s);
	index = 0;
}

bool pour_water::Is_Same() {//�ж��Ƿ��ظ�
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

bool pour_water::Can_Pour_Water(int i, int j) {//�����������ܷ�ˮ
	if (Situation_Stack.at(index).Poured_Cup_One == j&&Situation_Stack.at(index).Poured_Cup_Two == i) {//��ε�ˮ���������Ӳ��ܺ���һ�ε���ͬ
		return false;
	}
	if (Situation_Stack.at(index).Poured_Cup_One == i&&Situation_Stack.at(index).Poured_Cup_One == j) {//��ε�ˮ���������Ӳ��ܺ���һ�ε���ͬ
		return false;
	}
	if (Cup_List.at(j).int_Capacity == Cup_List.at(j).int_NowContain) {//�ڶ�����������
		return false;
	}
	if (Cup_List.at(i).int_NowContain == 0) {//��һ������Ϊ��z
		return false;
	}
	return true;
}

bool pour_water::Is_Finished() {//�Ƿ����
	for (unsigned int i = 0; i < Cup_List.size(); i++) {
		if (Cup_List.at(i).int_NowContain != Cup_List.at(i).int_WannaCapacity) {
			return false;
		}
	}
	return true;
}

void pour_water::pour(int k) {//��ˮ����
	//cout << k;
	unsigned int i, j;
	for (i = 0; i < Cup_List.size(); i++) {
		for (j = 0; j < Cup_List.size(); j++) {
			if (i != j) {
				if (Pour_Water(i, j, k)) {//��ˮ
					if (Is_Same()) {//�������ǰ���ظ�����ôɾ������ڵ�
						erase_last_situation();
					}
					else {
						if (Is_Finished()) {//����ﵽ����Ҫˮ��
							break;
						}
						else {
							for (int i = 0; i < Cup_List.size(); i++) {//�ѱ��ӵ�ˮ���Ļظ��ڵ㴢���״̬
								Cup_List.at(i).int_NowContain = Situation_Stack.at(index).Contain_Situation.at(i);
							}
						}
					}
				}
			}
		}
	}
	if (index < Situation_Stack.size() && !Is_Finished()) {//�ݹ飬�ظ��������
		pour(++index);
	}
	return;
}

bool pour_water::Pour_Water(int Cup_One, int Cup_Two, int father) {
	vector<int> situate;
	if (Can_Pour_Water(Cup_One, Cup_Two)) {//������������ӿ��Ե�ˮ
		if ((Cup_List.at(Cup_One).int_NowContain + Cup_List.at(Cup_Two).int_NowContain) < Cup_List.at(Cup_Two).int_Capacity) {//����������ӵ�ˮ�����С�ڵڶ������ӵ���ˮ��
			Cup_List.at(Cup_Two).int_NowContain = Cup_List.at(Cup_One).int_NowContain + Cup_List.at(Cup_Two).int_NowContain;//�ڶ�����������ˮ����ɵ�һ�����ӵ�ˮ�����ϵڶ������ӵ�
			Cup_List.at(Cup_One).int_NowContain = 0;//��ô��һ������ȫ��
		}
		else {
			Cup_List.at(Cup_One).int_NowContain = Cup_List.at(Cup_One).int_NowContain - (Cup_List.at(Cup_Two).int_Capacity - Cup_List.at(Cup_Two).int_NowContain);//���ߵ�һ�����ӵ�ˮ�������������ӵ���ˮ����ȥ�ڶ������ӵ�����
			Cup_List.at(Cup_Two).int_NowContain = Cup_List.at(Cup_Two).int_Capacity;//�ڶ������ӱ�װ��
		}
		for (unsigned int i = 0; i < Cup_List.size(); i++) {//�����б��ӵ�����ˮ������situate�����ʱ������
			situate.push_back(Cup_List.at(i).int_NowContain);
		}
		situation* s = new situation(Cup_One, Cup_Two, father, situate);
		Situation_Stack.push_back(*s);
		delete(s);
		return true;
	}
	return false;
}

void pour_water::erase_last_situation() {//ɾ�����һ����㲢�Ұѱ��ӵ�ˮ���Ļظ��ڵ㴢���״̬
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

void pour_water::Cout_Situation_Stack() {//�������
	if (Situation_Stack.size() > index) {
		vector<situation> s;
		s.push_back(Situation_Stack.at(0));
		int temp_index;
		temp_index = Situation_Stack.size() - 1;
		while (temp_index > 0) {
			s.insert(s.begin() + 1, Situation_Stack.at(temp_index));
			temp_index = Situation_Stack.at(temp_index).father;
		}
		cout << endl << "��" << s.size() - 1 << "������" << endl;
		for (unsigned int i = 0; i < s.size(); i++) {
			cout << s.at(i) << endl;
		}
	}
	else {
		cout << "�޽�!" << endl;
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
	out << "�ӵ�" << s.Poured_Cup_One << "�����ӵ���" << s.Poured_Cup_Two << "������" << endl;
	for (unsigned int i = 0; i < s.Contain_Situation.size(); i++) {
		out << "�� " << i << " �����ӵ�ˮ����: " << s.Contain_Situation.at(i) << "\t" << endl;
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