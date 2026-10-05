### 41443122 林睿彥
Homework1

#### 解題說明:
##### 題目 1（Problem 1）:
阿克曼函數（Ackermann’s function）$A(m,n)$ 的定義如下：

$$
A(m,n) = \begin{cases} n + 1 & \text{若 } m = 0 \\ A(m - 1, 1) & \text{若 } n = 0 \\ A(m - 1, A(m, n - 1)) & \text{其他情況} \end{cases}
$$

研究此函數是因為即便在 $m$ 和 $n$ 數值很小時，它增長的速度仍非常快速。請撰寫一個計算此函數的遞迴函式。再寫出一個用來計算阿克曼函數的非遞迴演算法。

##### 題目 2（Problem 2）:
若 $S$ 是一個包含 $n$ 個元素的集合，則 $S$ 的冪集是指由 $S$ 的所有可能子集所組成的集合。例如：若 $S = (a, b, c)$，則 $\text{powerset}(S) = \{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)\}$。請撰寫一個遞迴函式來計算 $\text{powerset}(S)$。

##### 解題策略:
###### 題目 1（Problem 1）:
把題目給的數學遞迴式，改成if-else型式即可寫出遞迴函式。
使用堆疊手動模擬呼叫堆疊。

###### 題目 2（Problem 2）:
路徑 A（不挑它）：什麼都不做，直接問下一個字母。
路徑 B（挑它）：放進暫存陣列，接著問下一個字母。
問完後拿出來（復原狀態）。
從第1個字母開始問，直到所有字母問完。
由上述可知，運用堆疊或佇列來寫出此題。

#### 程式實作:
##### 題目 1（Problem 1）:
以下為主要程式碼：
```cpp
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
```

##### 題目 2（Problem 2）:
以下為主要程式碼：
```cpp
#include<iostream>
#include<cstdlib>
#include<cstring>
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
```

#### 效能分析:
##### 題目 1（Problem 1）:
時間複雜度:程式的時間複雜度為 $O(\text{Ackermann}(m, n))$。
空間複雜度: 程式的空間複雜度為 $O(\text{Ackermann}(m, n))$。

##### 題目 2（Problem 2）:
時間複雜度:程式的時間複雜度為 $O(n \times 2^n)$。
空間複雜度: 程式的空間複雜度為 $O(n)$。

#### 測試與驗證:
##### 題目 1（Problem 1）:
###### 測試案例表格
| 測試案例 | 輸入參數 $(m, n)$ | 預期輸出 | 實際輸出 |
| :--- | :--- | :--- | :--- |
| 測試一 | $m = 2, n = 1$ | 5 | 5 |
| 測試二 | $m = 3, n = 1$ | 13 | 13 |
| 測試三 | $m = 4, n = 1$ | 65533 | 65533 |
| 測試四 | $m = -1, n = -3$ | -1（防呆攔截） | -1（防呆攔截） |

###### 編譯與執行指令
```shell
$ g++ -std=c++17 -o ackermann ackermann.cpp
$ ./ackermann
請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:2 1

遞迴版Ackermann結果:5
非遞迴版Ackermann結果:5

第1筆測資結束=====================================================\
請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:3 1

遞迴版Ackermann結果:13
非遞迴版Ackermann結果:13

第2筆測資結束=====================================================\
請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:4 1

遞迴版Ackermann結果:65533
非遞迴版Ackermann結果:65533

第3筆測資結束=====================================================\
請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:-1 -3

m或n其中一值為負值，此結果為防止無限遞迴。[原因:阿克曼函數不可用負值]
遞迴版Ackermann結果:-1
m或n其中一值為負值,此結果防止無窮迴圈(無限堆疊)。[原因:阿克曼函數不可用負值]
非遞迴版Ackermann結果:-1

第4筆測資結束=====================================================\
請輸入阿克曼函數的變數Ackermann(m,n)[阿克曼函數不可用負值]:
```

###### 結論:
1. 程式能正確執行阿克曼函數。
2. 阿克曼函數不可輸入負值。
3. Ackermann(int m, int n) m=4 n=2 理論數值龐大且超出 int 範圍。
4. 專案屬性中給定堆疊預留大小67108864可以跑出Ackermann(4, 1)，不給定則記憶體不足無法產生結果。
5. 測試案例中的圖片，把定堆疊預留大小從1MB 改為 64MB。

##### 題目 2（Problem 2）:
###### 測試案例表格
| 測試案例 | 輸入集合 $S$ | 元素個數 $n$ | 預期子集個數 | 實際輸出內容 |
| :--- | :--- | :--- | :--- | :--- |
| 測試一 | {'a', 'b', 'c'} | 3 | 8 | { (), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c) } |

###### 編譯與執行指令
```shell
$ g++ -std=c++17 -o powerset powerset.cpp
$ ./powerset
{ (), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c)}
```

###### 結論:
1. 運用堆疊來製作Powerset，較好理解但輸出順序會與題目的{(), (a), (b), (c), (a,b), (a,c), (b,c), (a,b,c)}不相同。
2. 核心思維為挑與不挑，挑完之後拿出來。

#### 申論及開發報告:
##### 題目 1（Problem 1）:
###### 為何程式的時間複雜度為 $O(\text{Ackermann}(m, n))$?
在阿克曼函數的三個分支中，唯一真正推進數值的操作只有符合條件 $m == 0$ 時的 $n + 1$。最終結果值若為 Ackermann (m, n)，代表程式底層至少執行了約 Ackermann (m, n) 次的基底加法與對應的狀態轉移（Pop/Push），因此時間複雜度直接正比於函數計算結果。

因此可得出公式解:
- $m = 0$: $\text{Ackermann}(0, n) = n + 1$（後繼運算） $\to O(1)$
- $m = 1$: $\text{Ackermann}(1, n) = n + 2$（加法運算） $\to O(n)$
- $m = 2$: $\text{Ackermann}(2, n) = 2n + 3$（乘法運算） $\to O(n)$
- $m = 3$: $\text{Ackermann}(3, n) = 2^{n+3} - 3$（指數運算） $\to O(2^n)$
- $m = 4$: $\text{Ackermann}(4, n) = 2 \uparrow\uparrow (n+3) - 3$（迭代冪次運算） $\to O(2 \uparrow\uparrow n)$

由此可知即便在 $m$ 和 $n$ 數值很小時，它增長的速度仍非常快速。

###### 為何空間複雜度為 $O(\text{Ackermann}(m, n))$?
在處理巢狀運算 $\text{Ackermann}(m - 1, \text{Ackermann}(m, n - 1))$ 時，外層的狀態必須持續暫存在堆疊中等待內層求值。最深展開路徑上同時並存的狀態數量正比於計算過程中的數值大小。
- **遞迴版本**：使用系統呼叫堆疊，超過編譯器上限（如 Windows 預設 1 MB）。
- **非遞迴版本**：使用作業系統 Heap(堆積) 區，容量上限取決於實體記憶體。

##### 題目 2（Problem 2）:
###### 正解方案（DFS（深先搜尋）+ 遞迴）：
說明遞迴邊走邊印時順序會依決策路徑展開（例如先走不挑分支產生 ()、(c)），這是標準遞迴解法。

###### 申論/延伸探討（BFS(廣度優先搜尋) + Queue）：
補充說明若要嚴格達成題目長度排序的視覺效果，理論上需要透過 Queue 進行廣度優先搜尋，或先以遞迴產生全部子集後依長度排序輸出。