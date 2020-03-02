///* the variant , smart point, the contorl example */
//#include<iostream>
//#include<memory>
//#include "Control.h"
//
//using namespace std;
//
//
////���ͱ��3�ַ�ʽ:1.the stl 2.the varant 3.the void*
//enum var_type{
//	enum_int,
//	enum_double,
//	enum_string,
//};
////the variant
//typedef struct variant{
//	int type;	
//	union{
//		int iValue;	
//		double dValue;
//		char* pValue;
//	}var;
//}Var;
////set_value
//void set_value(struct variant& p,int type,const char* string){
//	p.type = type;	
//	p.var.iValue = *(int*)(char*)string;
//	return ;
//}
//	
////print value
//void print(struct variant* p, var_type type){
//	switch(type){
//	case enum_int:
//		printf("%d\n", p->var.iValue);
//		break;
//
//	}
//	return ;
//}
////the auto ptr of my own
//template <typename T>
//class my_auto_ptr{
//public:
//	my_auto_ptr() : ptr(NULL){
//	}
//	my_auto_ptr(my_auto_ptr<T>& auto_ptr){
//		ptr = Release();
//	}
//	my_auto_ptr(T* point){
//		ptr = point;
//	}
//	my_auto_ptr<T>& operator = (my_auto_ptr<T>& auto_ptr){
//		//need to consider the a = a situation
//		if(auto_ptr.ptr == Get()){
//			return (*this);
//		}else if(ptr != NULL){
//			delete ptr;
//			ptr = NULL;
//		}
//		ptr = auto_ptr.Release();	
//		return (*this);
//	}
//	T& operator * (){
//		return *ptr;
//	}
//	T* operator ->(){
//		return ptr;
//	}
//	~my_auto_ptr() throw (){//throw the whether the ptr is null
//		delete ptr;
//	}
//	T* Release(){// to hand over the point last 
//		T* tmp = ptr;
//		ptr = NULL;
//		return tmp;
//	}
//	T* Get(){
//		return ptr;
//	}
//
//	//�������������
//private:
//	T* ptr;
//};
//
////the shared ptr of my own
//template<typename T>
//class my_shared_ptr{
//public:
//	explicit my_shared_ptr() : ptr(NULL), count(NULL){
//
//	}
//	explicit my_shared_ptr(T* point) : ptr(NULL), count(NULL){
//		//first create
//		count = new int(1);
//		ptr = point;
//
//	}
//	explicit my_shared_ptr(my_shared_ptr<T>& shared_ptr) :ptr(NULL), count(NULL){
//		ptr = shared_ptr.get_ptr();	
//		count = shared_ptr.get_count();
//		(*count)++;
//	}
//	template<typename TT>
//	my_shared_ptr(my_shared_ptr<TT>& shared_ptr){
//		ptr = (T*)shared_ptr.get_ptr();
//		count = shared_ptr.get_count();
//		(*count)++;
//	}
//	~my_shared_ptr(){
//		if(count != NULL){
//			(*count)--;
//			if( *count <=0 ){
//				delete ptr;
//				delete count;
//			}
//		}
//	}
//	T* get_ptr(){
//		return ptr;
//	}
//	int* get_count(){
//		return count;
//	}
//private:
//	T* ptr;
//	int* count;
//};
//class Test_ptr{
//public:
//	void fun(){
//		cout << "fun()" << endl;
//	}
//
//};
//
//class Test :  public CControl<Test>{
//public:
//	BOOL Init(){
//		cout << "Test" << endl;
//		return TRUE;
//	}
//	void UnInit(){
//		cout << "Un test" << endl;
//		return ;
//	}
//
//};
//int main(){
//	//1.the variant myself
//	Var* a = new Var;
//	set_value(*a,enum_int,"14");
//	print(a,enum_int);
//	//2. the auto_ptr shared_ptr weak_ptr unique_ptr 
//	//the origin point have problems
//	//Ұָ��ͨ�����캯�����г�ʼ��
//	/* auto ptr ͨ��һ�����󱣻�һ��ָ��*/
//	Test_ptr* p ;
//	Test_ptr* pp;
//	auto_ptr<Test_ptr> ptr(p);
//	auto_ptr<Test_ptr> test_ptr;
//	ptr->fun();
//	//ͨ��=��ֵ�ӹ�����Ҫ��ָ��
//	test_ptr = ptr;
//	//ͨ����������ڴ���л���
//	//use of my own auto_ptr
//	my_auto_ptr<Test_ptr> what(pp);
//	my_auto_ptr<Test_ptr> the;
//	what->fun();
//	the = what;
//
//	/*3.shared_ptr ͨ��������󱣻�һ��ָ�룬ʹ�ü����������ָ��ʲôʱ�����
//	���������þ�����ں˶�������������ж��������в�����ͬʱ����ʹ�ü���*/
//	int* i = new int(12345);			
//	my_shared_ptr<int> tm(i);
//	my_shared_ptr<int> aa(tm);
//	my_shared_ptr<short> tmd(aa);
//
//
//	//4. the control class 
//	//CControl<Test>* p = CControl<Test>::getInstance();
//	//p->OpenControl();
//	//while(CControl<Test>::m_bQuit == FALSE){
//	//	Sleep(1);
//	//}
//	//p->CloseControl();
//	system("pasue");
//	return 0;
//
//
//}

#include <iostream>
#include "CKernel.h"
#include "Control.h"

using namespace std;

int main(){
	//��ȡ����
	CControl<CKernel> *pControl = CControl<CKernel>::getInstance();
	//open
	pControl->OpenControl();
	//�ȴ�����̨�˳�֪ͨ
	while(pControl->m_bQuit == FALSE){
		::Sleep(10);
	}
	//close
	pControl->CloseControl();
	//���ٵ���
	CControl<CKernel>::destoryInsstance();
	return 0;
}