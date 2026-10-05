#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
//建立 My_Stack 類別
//使用陣列動態記憶體配置模擬Stack
class My_Stack
{
private:
	int capacity=16;	//容量
	int top = -1;		//索引
	int* st;
public:
	My_Stack()
	{
		st = new int[capacity];
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
	void push(int val){
		if (top == capacity - 1) {
			capacity *= 2;
			int* new_st = new int[capacity];
			memcpy(new_st, st, (top + 1) * sizeof(int));	//memcpy用於從一個記憶體位置複製指定數量的位元組到另一個記憶體位置。
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

};

//阿克曼函式遞迴版
//預設配置的記憶體只有1 MB,當m>=4時超過了系統呼叫堆疊的預設上限。
//Ackermann(int m, int n) m=4 n=1 為65533
//Ackermann(int m, int n) m=4 n=2 理論數值龐大且超出 int 範圍，且目前的電腦記憶體容量無法算出如此龐大的數值。
int Ackermann(int m, int n)
{
	if (m < 0 || n < 0) {
		cout << "m或n其中一值為負值，此結果為防止無限遞迴。[原因:阿克曼函數不可用負值]" << endl;
		return -1;
	}
	if (m == 0) return n + 1;
	if (n == 0) return Ackermann(m - 1, 1);
	return Ackermann(m - 1, Ackermann(m, n - 1));
}

//阿克曼函式非遞迴版
//Stack的用途把還沒有完成的m保存起來，while迴圈下一次再取出繼續計算。
int Ackermann_nonrecursive(int m,int n) {
	My_Stack st;
	if (m < 0 || n < 0) {
		cout << "m或n其中一值為負值,此結果防止無窮迴圈(無限堆疊)。[原因:阿克曼函數不可用負值]" << endl;
		return -1;
	}
	st.push(m);
	while (!st.isEmpty()) {
		m = st.pop();
		if (m == 0) {
			n += 1;
		}
		else if (n == 0) {
			n = 1;
			st.push(m - 1);
		}
		else {
			/*	stack 後進先出
				Ackermann([外層]->m - 1,[內層]->Ackermann(m, n - 1))
				遞迴->內層先算->後推入->外層後算->先推入
			*/
		
			//外層
			st.push(m - 1);
			//內層
			st.push(m);
			n -= 1;
		}
	}
	return n;
}

int main(void) {
	int m, n;
	int datanum = 1;
	
	while (cout<<"請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:", cin >> m >> n) {
		cout << endl;
		cout << "遞迴版Ackermann結果:" << Ackermann(m, n)<<endl;
		cout << "非遞迴版Ackermann結果:" << Ackermann_nonrecursive(m,n) << endl;
		cout << endl;
		cout << "第" << datanum << "筆測資結束=====================================================\\" << endl;
		datanum++;
	}
}