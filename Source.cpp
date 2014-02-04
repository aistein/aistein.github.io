#include<stdlib.h>
#include<cstdint>
#include<string>
#include<iostream>

using namespace std;

namespace epl
{
	template<typename T>
	class Vector {

	private:

		/*
			elem: points to the first constructed element (if any)
			begin: points to the beginning of allocated memory
		*/
		T* elem;
		//T** begin;
		size_t cap;
		uint64_t len;

	public:

		Vector(void){
			cap = size_t(8) * sizeof(T);
			len = 0;
			elem = (T*) ::operator new(cap);
			//begin = new T*(elem);
		} // end default constructor

		explicit Vector(uint64_t n){
			cap = size_t(n) * sizeof(T);
			len = n;
			elem = (T*) ::operator new(cap);
			//begin = new T*(elem);
			for (uint64_t i = 0; i < len; i++){
				new(elem + i)T;
			}
		} // end explicit constructor 

		Vector(const Vector& v) {
			cap = v.getCap();
			len = v.getLen();
			elem = (T*) ::operator new(cap);
			//begin = new T*(elem - v.frontCap());
			for (uint64_t i = uint64_t(0); i < len; i++){
				new(elem + i)T;
			}
			for (uint64_t i = uint64_t(0); i < len; i++){
				elem[i] = v.getElem(i);
			}
		} // end copy constructor

		void push_back(const T& x)
		{
			if (((size_t(len) * sizeof(T)) + sizeof(x)) > cap){
				cap *= size_t(2);
				T* buffer = (T*) ::operator new(cap);
				for (uint64_t i = uint64_t(0); i < len; i++){
					new(buffer + i)T(elem[i]);
					elem[i].~T();
				}
				::operator delete(elem);
				elem = buffer;
			} // only need to realloc if the cap is too small
			new(elem + len)T(x);
			len += 1;
		} // end push_back

		void push_front(const T& x)
		{
			if (((size_t(len) * sizeof(T)) + sizeof(x)) > cap){
				cap *= size_t(2);
			} // always need to realloc because shifting up
			T* buffer = (T*) ::operator new(cap);
			for (uint64_t i = uint64_t(0); i < len; i++){
				new(buffer + (i + 1))T(elem[i]);
				elem[i].~T();
			}
			::operator delete(elem);
			elem = buffer;
			new(elem)T(x);
			len += 1;
		} // end push_front

		void pop_back(void) {
			if (len == uint64_t(0)) throw std::out_of_range("vector is empty.");
			(elem + (len - 1))->~T();
			len--;
		} // end pop_back

		void pop_front(void) {
			if (len == uint64_t(0)) throw std::out_of_range("vector is empty.");
			elem->~T();
			elem++;
			len--;
		} // end pop_front

		T& operator[](uint64_t k)
		{
			if ((k < uint64_t(0)) || (len <= k)) throw std::out_of_range("subscript out of range");
			return elem[k];
		} // end [] operator

		const T operator[](uint64_t k) const
		{
			if ((k < uint64_t(0)) || (len <= k)) throw std::out_of_range("subscript out of range");
			return &elem[k];
		} // end const [] operator

		const uint64_t getLen(void) const {
			return len;
		} // end getLen

		const uint64_t getCap(void) const {
			return cap;
		} // end getCap

		const T getElem(uint64_t i) const {
			return elem[i];
		} // end getElem

		const T** getBegin(void) const {
			return begin;
		} // end getBegin

		const uint64_t frontCap(void) const {
			return elem - (*begin);
		}

		~Vector(void){
			for (uint64_t i = uint64_t(0); i < len; i++){
				(elem + i)->~T();
			}
			::operator delete(elem);
			//delete begin;
		} // end destructor

	}; // end template class Vector definition
} // end namespace epl


int main(void){
	try {
		epl::Vector<std::string> vc;
		epl::Vector<std::string> vcc(vc);
		
		// if you can pass the realloccopy test then you will get a good grade.

		vcc.push_back("hello");
		vcc.push_back("there");
		vcc.push_back("friend");
		//vcc.pop_front();
		vcc.push_back("why");
		vcc.push_front("why");
		vcc.push_back("don't");
		vcc.push_back("you");
		vcc.push_back("have");
		vcc.push_back("a");
		vcc.push_back("drink");
		
		cout << "Cap: " << vcc.getCap() << '\n';
		cout << "Len: " << vcc.getLen() << '\n';
		cout << "size of T: " << sizeof(string) << '\n';
		for (uint64_t i = uint64_t(0); i < vcc.getLen(); i++){
			cout << "vcc[" << i << "]: " << vcc.getElem(i) << '\n';
		}

		char DONE[5];
		cout << "Type 'exit' to close." << '\n';
		cin >> DONE;
		if (strcmp(DONE, "exit") !=0) return 0;
		
		cout << "Still Running...\n";
	}
	catch (std::out_of_range) {
		cerr << "Error: index out of range." << '\n';
	}
	return 0;
} // end main