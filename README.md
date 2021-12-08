# myContacts
C++ 通讯录

定义数据结构 struct Person存储单个人的信息

定义数据结构 class Contacts存储整张通讯录表

业务层定义了xxx_entry函数实现具体业务逻辑

接口层实现了对Contacts的增加、删除、查找、遍历等功能，以及文件操作的保存和加载

解析函数parse实现了对单行数据解析出相应属性并返回Person对象，当Person结构体改变时，大抵只需要修改return语句
