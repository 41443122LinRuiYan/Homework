#include<iostream>
#include<cstdlib>
using namespace std;
//建立 My_Stack 類別
//使用陣列動態記憶體配置模擬Stack
class My_Stack
{
private:
	int capacity = 16;	//容量
	int top = -1;		//索引
	
public:
	char* st;
	My_Stack()
	{
		st = new char[capacity];
	}
	~My_Stack() {
		delete[] st;
	}
	bool isEmpty() {	//檢查堆疊是否為空
		if (top == -1) {
			return true;
		}
		else {
			return false;
		}
	}
	void push(char val) {
		if (top == capacity - 1) {
			capacity *= 2;
			char* new_st = new char[capacity];
			memcpy(new_st, st, (top + 1) * sizeof(char));	//memcpy用於從一個記憶體位置複製指定數量的位元組到另一個記憶體位置。
			delete[] st;
			st = new_st;	//重新指定位址,不可delete[] new_st會連st的位址一起釋放掉。
		}

		st[++top] = val;	//頂端索引加 1後,將數值放入堆疊頂端。
	}
	int pop() {
		if (isEmpty()) {
			return -1;
		}
		return st[top--];	//取出目前頂端的數值後，頂端索引減 1。
	}

	//從問題一的My_stack類別中，增加了以下函式
	// S: 原陣列, n: 總長度, i:目前看到第幾個字母(原陣列索引), st:暫存陣列, flag:逗號控制旗標
	void Powerset(char S[], int n, int i,bool &flag)
	{
		if (n == i) {//所有字母都問完:印出暫存陣列的內容
			//除了第一個括號前面不加逗號外，後續每個括號前都要補上 " , "。
			if (!flag) {
				cout << ", ";// 初始值
			}
			flag = false;
			cout << "(";
			for (int j = 0; j <= top; j++) {
				cout << st[j];
				if (j < top) cout << ",";
			}
			cout << ")";
			return;
		}
		// 選擇 1：不挑第 i 個元素
		Powerset(S, n, i + 1,flag);
		
		// 選擇 2：挑第 i 個元素（放進暫存陣列）
		push(S[i]);
		Powerset(S, n, i + 1,flag);
		pop();//拿出來

	}

};


int main(void) {
	My_Stack st;
	char S[] = { 'a','b','c' };
	bool flag = true;	//逗號控制旗標 初始值
	int n = 3;			//n: 總長度
	int i = 0;		    //目前看到第幾個字母(原陣列索引)
	cout << "{ ";
	st.Powerset(S, n, i,flag);
	cout << "}";
}