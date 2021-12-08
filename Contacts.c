#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <set>
#include <fstream>
#include <utility>

using namespace std;

struct Person
{
	friend std::ostream& operator<< (std::ostream&, const Person&);
	friend bool operator<(const Person&, const Person&);
	friend bool operator==(const Person&, const Person&);
	explicit Person(const std::string name = " ", const std::string& phone = ""):_name(name),_phone(phone) {  }
	Person(const Person& p):_name(p._name),_phone(p._phone) { }
	std::string _name;
	std::string _phone;
};
Person parse(const std::string& str)
{
	unordered_map<std::string, std::string> property_map;
	size_t start = 0, end = 0;
	while((end = str.find(start,';')) != std::string::npos)
	{
		std::string property = str.substr(start,end);
		size_t i = property.find(":");
		if(i != std::string::npos)
		{
			property[i] = ' ';
		}
		istringstream is(property);
		string key;
		string value;
		is >> key >> value;
		property_map[key] = value;
		start = end+1;
	}
	{
		std::string property = str.substr(start,str.size());
		size_t i = property.find(":");
		if(i != std::string::npos)
		{
			property[i] = ' ';
		}
		istringstream is(property);
		string key;
		string value;
		is >> key >> value;
		property_map[key] = value;
	}
	return Person(property_map["name"],property_map["phone"]);
}
std::ostream& operator<< (std::ostream& out, const Person& person)
{
	cout << "姓名：" << person._name << ";手机：" << person._phone;
	return out;
}
bool operator<(const Person& p1, const Person& p2)
{
	return p1._name < p2._name;
}
bool operator==(const Person& p1, const Person& p2)
{
	return p1._name == p2._name;
}


class Contacts
{
	typedef std::map<std::string,Person> container;
public:
	Contacts() = default;
	Contacts(const Contacts&) = default;
	Contacts& operator=(const Contacts&) = default;

	Contacts& add(const Person&);
	Contacts& delete_with_name(Person&);
	std::pair<bool,Person> find(const std::string& name);
	std::set<Person> traverse();
private:
	container psMap;
};
inline Contacts& Contacts::add(const Person& p)
{
	psMap[p._name] = p;
	return *this;
}
inline Contacts& Contacts::delete_with_name(Person& p)
{
	std::pair<bool,Person> ret = find(p._name);
	if(ret.first)
	{
		p = ret.second;
		psMap.erase(p._name);
	}
	return *this;
}
inline std::pair<bool,Person> Contacts::find(const std::string& name)
{
	container::iterator pit = psMap.find(name);
	if(pit != psMap.end())
		return make_pair(true,Person(pit->second));
	else
		return make_pair(false,Person(name));
}
inline std::set<Person> Contacts::traverse()
{
	std::set<Person> pset;
	for(container::const_iterator pit = psMap.cbegin(); pit != psMap.cend(); ++pit)
	{
		pset.insert(pit->second);
	}
	return pset;
}

void Add_entry(Contacts& cts)
{
	string name;
	string phone;
	cout << "请输入联系人的姓名：";
	cin >> name;
	cout << "请输入联系人的手机号码：";
	cin >> phone;
	cts.add(Person(name,phone));
	cout << "联系人添加成功！" << endl;
}
void Delete_entry(Contacts& cts)
{
	string name;
	cout << "请输入要删除的联系人的姓名：";
	cin >> name;
	Person p = cts.find(name).second;
	cout << "要删除的联系人的信息为："<< endl;
	cout << p << endl;
	cout << "是否确认删除：1. 是\t 2. 否" << endl;
	int select;
	cin >> select;
	if(select == 1)
	{
		cts.delete_with_name(p);
		cout << "联系人删除成功！" << endl;
	}
}
void Find_entry(Contacts& cts)
{
	string name;
	cout << "请输入要查找的联系人姓名：" ;
	cin >> name;
	pair<bool,Person> ret = cts.find(name);
	if(ret.first)
	{
		cout << "联系人的信息如下：" << endl;
		cout << ret.second << endl;
	}else
	{
		cout << "联系人不存在！" << endl;
	}
}
void Show_entry(Contacts& cts)
{
	set<Person> ps = cts.traverse();
	cout << "通讯录如下：" << endl;
	for(auto &p:ps)
		cout << p << endl;

}
void Save_entry(Contacts& cts)
{
	string path;
	cout << "请输入要存入文件：";
	cin >> path;
	ofstream fout;
	int select = 0;
	while(select != 1 && select != 2)
	{
		cout << "请选择写入方式：1. 覆盖写入	2. 追加写入" << endl;
		cin >> select;
		if(select == 1 )
		{
			fout.open(path,ios::out);
		}else if(select == 2)
		{
			fout.open(path,ios::app);
		}
		cout << "请输入正确的选项！" << endl;
	}
	auto ret = cts.traverse();
	for(auto& p:ret)
	{
		fout << "name:" <<p._name << ";phone:" << p._phone << endl;
	}
	cout << "通讯录保存成功，保存在文件 " << path << " 下" << endl;
}
void Load_entry(Contacts& cts)
{
	string path;
	cout << "请输入加载的通讯录文件名：";
	cin >> path;
	ifstream fin(path,ios::in);
	if(!fin)
	{
		cout << "文件打开失败，请确认文件是否存在！" << endl;
		return;
	}
	string line;
	while(!fin.eof() && getline(fin,line))
	{
		cts.add(parse(line));
	}
	cout << "通讯录加载完成！" << endl;
}
void Menu()
{
	cout << "-----------------通讯录-------------" << endl;
	cout << "--1. 添加联系人      2. 删除联系人--" << endl;
	cout << "--3. 查找联系人      4. 显示通讯录--" << endl;
	cout << "--5. 加载通讯录      6. 保存通讯录--" << endl;
	cout << "------------------------------------" << endl;
	cout << "请输入您的选项：" << endl;
}

#define ADD 1
#define DELETE 2
#define FIND 3
#define SHOW 4
#define LOAD 5
#define SAVE 6

#define CLAER() printf("\033[2J")

int main()
{
	Contacts cts;
	while(1)
	{
		Menu();
		int select;
		cin >> select;
		switch(select)
		{
			case ADD:
				Add_entry(cts);
				break;
			case DELETE:
				Delete_entry(cts);
				break;
			case FIND:
				Find_entry(cts);
				break;
			case SHOW:
				Show_entry(cts);
				break;
			case LOAD:
				Load_entry(cts);
				break;
			case SAVE:
				Save_entry(cts);
				break;
			default:
				cout << "输入选项有误，请重新输入！"<< endl;
				break;
		}
		cout << "按任意键继续" << endl;
		getchar();
		getchar();
		CLAER();  //自定义的清屏
	}

}
