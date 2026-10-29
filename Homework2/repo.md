### 41443122 林睿彥
作業二

#### 解題說明
#### 圖一:
![image alt](https://github.com/41443122LinRuiYan/Homework/blob/3febce75d2697e21e7ec01b39c00d6766687233a/Homework2/%E5%9C%96%E7%89%87%E4%B8%80.png)

#### 圖二:
![image alt](https://github.com/41443122LinRuiYan/Homework/blob/3febce75d2697e21e7ec01b39c00d6766687233a/Homework2/%E5%9C%96%E7%89%87%E4%BA%8C.png)

1. 實作多項式(Polynomial) 類別,其抽象資料型別(ADT)和私有資料成員分別如圖1和圖2所示。
2. 撰寫 C++ 函式來輸入和輸出表示為圖2的多項式。你的函式應該多載(overload) `<<`和`>>` 運算子。

##### 解題策略:
1. 函式實作順序:NewTerm(擴增空間大小)>Add>Mulit,而Eval 先做或者後做皆可。
2. 優先把類別 Polynomial 製作完成,在設計多載運算子。
3. 主程式運用多載運算子以及上述所設計的函式,來輸出結果。

#### 程式實作
以下為主要程式碼:
```cpp
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;//前向宣告
class Term {
    friend Polynomial;
    friend istream& operator>>(istream& input, Polynomial& a);
    friend ostream& operator<<(ostream& output, const Polynomial& a);
private:
    float coef; //紀錄係數
    int exp; //紀錄指數
};

class Polynomial
{
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
private:
    Term* termArray; //儲存非零項的陣列。
    int capacity;//紀錄 termArry的陣列空間容量
    int terms;//記錄非零項的數量。
public:
    Polynomial() {
        capacity = 1;
        terms = 0;
        termArray = new Term [capacity];
    };
    Polynomial Add (Polynomial poly);//多項式加法
    void NewTerm(const float theCoeff, const int theExp);//termArry 擴充空間大小
    Polynomial Mulit(Polynomial poly);//多項式乘法
    float Eval(float f); //將數值f代入多項式中進行計算,並回傳其結果
    ~Polynomial() {};
};

void Polynomial::NewTerm (const float theCoeff, const int theExp) {
    if (terms == capacity) {//檢測空間是否充足
        capacity *= 2;//termArray的容量加倍
        Term* temp = new Term [capacity];
        copy(termArray, termArray + terms, temp);//把舊陣列內容複製到新陣列
        delete[] termArray; //釋放舊的陣列記憶體
        termArray = temp; //把新的記憶體位址指向原陣列
    }
    termArray[terms].coef = theCoeff; //把係數傳入,係數陣列
    termArray[terms++].exp = theExp; //把指數傳入,指數陣列後把 terms+1;
}

Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;//相加後的多項式
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms)) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            //對比指數是否一樣
            float t = termArray[aPos].coef + b.termArray[bPos].coef;//係數相加
            if (t) {//判斷此項係數是否為0,如果是則不執行
                c.NewTerm(t, termArray[aPos].exp); //傳入(*this)函數此項的係數與指數,藉此函式來傳入資料進對應矩陣以及判斷使否擴充大小
            }
            aPos++;
            bPos++;
        }
        else if (termArray[aPos].exp < b.termArray[bPos].exp) {//判斷是否(*this)的指數小於b(x)
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);//直接傳入b(x)的指數與係數
            bPos++;
        }
        else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);//直接傳入(*this)的指數與係數
            aPos++;
        }
    }
    //把(*this)與b(x)剩下的項給加進來
    for (; aPos < terms; aPos++) {
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    }
    for (; bPos < b.terms; bPos++) {
        c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    }
    return c;//回傳相加後的多項式
}

Polynomial Polynomial::Mulit(Polynomial b) {
    Polynomial c;//相乘後的多項式
    //多項式相乘後的總項數為aPos*bPos,所以使用for-for,使用前面所寫的Add 函式把每部分相乘的多項式相加
    for (int aPos = 0; aPos < terms; aPos++) {//走訪目前多項式(*this)的每一項
        Polynomial temp;//設回初值
        for (int bPos = 0; bPos < b.terms; bPos++) {//走訪目前多項式b(x)的每一項
            float cf = termArray[aPos].coef * b.termArray[bPos].coef;
            int ep = termArray[aPos].exp + b.termArray[bPos].exp;
            if (cf != 0) {//如果係數是0,不執行
                temp.NewTerm(cf, ep);
                c = c.Add(temp);//將c矩陣與 temp矩陣相加後的值回傳給c類似c=c+temp
            }
        }
    }
    return c;
}

float Polynomial::Eval(float f) {
    float c = 0;
    for (int i = 0; i < terms; i++) {
        c += (termArray[i].coef * pow(f, termArray[i].exp));
    }
    return c;
}

istream& operator>>(istream& input, Polynomial& a) {
    int numTerms;
    cout << "請輸入多項式項數:";
    input >> numTerms;
    for (int i = 0; i < numTerms; i++) {
        float coef;
        int exp;
        cout << "輸入第" << i+1 << "項的係數與指數(用空格隔開):";
        input >> coef >> exp;
        a.NewTerm(coef, exp);
    }
    return input;
}

ostream& operator<<(ostream& output, const Polynomial& a) {
    if (a.terms == 0) {
        output << "0";
        return output;
    }
    for (int i = 0; i < a.terms; i++) {
        //處理連接的正負號
        if ((i > 0) && a.termArray[i].coef > 0) output << "+";//如果是第一項係數則不需要加上正號
        else if (a.termArray[i].coef < 0) output << " - ";
        //印出係數的數字,使用abs 取絕對值去除多餘的負號)
        output << abs(a.termArray[i].coef);
        //判斷指數是否大於0
        if (a.termArray[i].exp > 0) {
            output << "x^" << a.termArray[i].exp;
        }
    }
    return output;
}

int main(void) {
    Polynomial a, b, c, d;
    float e, f;
    cout << "--- 輸入a(x)多項式---" << endl;
    cin >> a;
    cout << "a(x)=" << a << endl;
    cout << endl;
    
    cout << "--- 輸入b(x)多項式---" << endl;
    cin >> b;
    cout << "b(x)=" << b << endl;
    cout << endl;
    
    cout << "--- 輸入帶入a(x)多項式的數字f---" << endl;
    cin >> f;//多載運算子已經定義輸入資料型態需為 Polynomial
    cout << endl;
    
    c = a.Add(b);
    d = a.Mulit(b);
    e = a.Eval(f);
    
    cout << "a(x)帶入" << f << "得:" << e << endl;
    cout << "相加結果 a(x)+b(x)=" << c << endl;
    cout << "相乘結果 a(x)*b(x)=" << d << endl;
    return 0;
}
```

#### 效能分析
| 函式名稱 | 時間複雜度 | 空間複雜度 |
| --- | --- | --- |
| NewTerm | 空間足夠時 $O(1)$/最差 $O(m)$ | 一般 $O(1)$/擴充時 $O(m)$ |
| Add | $O(m+n)$ 跟 terms, b.terms 有關 | $O(m+n)$ |
| Mulit | 最差 $O(m^2n)$ 會有 $m^2$ 跟函式中運用到Add() 有關/最佳 $O(mn)$ | $O(mn)$(未釋放記憶體為 $O(m^2n)$) |
| Eval | $O(m)$ 單迴圈 | $O(1)$:回傳的c值 |
| operator>> | $O(m)$ 單迴圈 | $O(m)$:有幾項就多大 |
| operator<< | $O(m)$ 單迴圈  | $O(1)$:回傳的 output |

#### 測試與驗證
##### 測試案例
##### 小黑窗
![image alt](https://github.com/41443122LinRuiYan/Homework/blob/7352e9f582af3db7a708dfe0dc375b4f03c03524/Homework2/%E5%9C%96%E7%89%87%E5%9B%9B.png)
##### 編譯與執行指令
```shell
$ g++ Polynomial.cpp -std=c++17 -o Polynomial.exe
$ .\Polynomial.exe

結論
1. 程式能夠存入多項式的係數與指數。
2. 程式能夠進行加法以及乘法。
3. 程式能夠輸入數值f帶入多項式a(x)。
4. 此程式輸入多載運算子,不可判別輸入為2x^2+x+1,如果需要判別此種輸入需引入<string>與 <sstream>來幫助我們重新改寫輸入多載運算子。

申論及開發報告
類別 Polynomial 函式的開發細節

1. 函式 void NewTerm(const float theCoeff, const int theExp);

此函式主要處理新增多項式項目的邏輯與動態陣列的擴充：

if (terms == capacity)：利用此段程式檢測空間是否充足。

capacity *= 2;：使 termArray 的容量加倍。

Term* temp = new Term[capacity];：定義新的動態空間。

copy(termArray, termArray + terms, temp);：把舊陣列內容複製到新陣列。

delete[] termArray;：釋放舊的陣列記憶體。

termArray = temp;：把新的記憶體位址指向原陣列。

termArray[terms].coef = theCoeff;：把係數傳入係數陣列。

termArray[terms++].exp = theExp;：把指數傳入指數陣列後，把 terms 加 1。

2. 函式 Polynomial Add(Polynomial poly);

此函式處理兩個多項式的相加：

while 迴圈：當 a 和 b 的 terms 其中一個走訪完時結束。代表多項式較短的一方會讓此迴圈結束。

if 條件：當指數一樣時，係數相加。如果此項係數為零，則不把此項列入物件 c。

else if 條件：判斷是否 (*this) 的指數小於 `b(x)`，如果小於，則把此項係數與指數加進物件 c。

else 條件：判斷是否 (*this) 的指數大於 b(x)，如果大於，則把此項係數與指數加進物件 c。

接著處理剩餘項目：因多項式較短一方使 `while` 迴圈提前結束，須將所剩下的項加入物件。

範例說明 (Ex)

假設有兩個多項式：
a(x) = 3x^3 + 2x^2 + 5
b(x) = -3x^3

1. 當指數一樣時，係數相加。如果此項係數為零，則不把此項列入物件 c。

2. 因 bPos == 1 所以跳出迴圈。

3. A (*this) 還有剩下的項（2x^2 和 5）還沒處理。

4. 程式往下走，進入第一個 for 迴圈（for (; aPos < terms; aPos++)），把 A 剩下的 2x^2 跟 5 直接加進結果多項式 c 裡面。

5. 第二個 for 迴圈不滿足條件，直接跳過。

3. 函式 Polynomial Mulit(Polynomial poly);

多項式相乘後的總項數為 aPos * bPos，所以使用巢狀迴圈 (for-for)，並使用前面所寫的 Add 函式把每部分相乘的項式相加（運用 c = c.Add(temp)，類似 c = c + temp）。

#### 圖三：
```
<img width="580" height="805" alt="image" src="https://github.com/user-attachments/assets/282e7a8d-f079-40cf-979d-3f487a8f602c" />
```

4. 函式 float Eval(float f);

此函式用於計算多項式代入數值後的結果：

1. 設定初值 c = 0;

2. 運用 for 迴圈走訪 `(*this)` 的每一項。

3. 運用 <cmath> 裡的 pow 來解決次方問題。

4. 用 c += (termArray[i].coef * pow(f, termArray[i].exp)); 儲存累加結果。

5. 回傳結果。

5. 多載運算子的細節

輸入 >> 運算子

1. 先問使用者要輸入幾項。

2. 再依需輸入此項係數與指數，以空格隔開。

3. 且把輸入的數值存進物件 a。

輸出 << 運算子

1. 先問使用者要輸入幾項。

2. 再依需輸入此項係數與指數，以空格隔開。

3. 且把輸入的數值存進物件 a。
