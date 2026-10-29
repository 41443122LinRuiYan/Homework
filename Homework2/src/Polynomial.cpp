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
        termArray = new Term[capacity];
    };
    Polynomial Add(Polynomial poly);//多項式加法
    void NewTerm(const float theCoeff, const int theExp);//termArry 擴充空間大小
    Polynomial Mulit(Polynomial poly);//多項式乘法
    float Eval(float f); //將數值f代入多項式中進行計算,並回傳其結果
    ~Polynomial() {};
};

void Polynomial::NewTerm(const float theCoeff, const int theExp) {
    if (terms == capacity) {//檢測空間是否充足
        capacity *= 2;//termArray的容量加倍
        Term* temp = new Term[capacity];
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
        
        for (int bPos = 0; bPos < b.terms; bPos++) {//走訪目前多項式b(x)的每一項
            if (b.termArray[bPos].coef == 0) continue;//如果係數為0則跳過此次迴圈，否則多項式結構可能會壞例如(5)(3x^2+0x^1+5)
            Polynomial temp;//設回初值
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
        cout << "輸入第" << i + 1 << "項的係數與指數(用空格隔開):";
        input >> coef >> exp;
        a.NewTerm(coef, exp);
    }
    return input;
}

ostream& operator<<(ostream& output, const Polynomial& a) {
    bool First = false;
    int temp = 0;
   
    for (int i = 0; i < a.terms; i++) {
        if (a.termArray[i].coef == 0) {//如果係數是 0，直接計數並跳過這一項。
            temp++;
            continue;
        }
        //處理連接的正負號
        if (First) {//判斷前面有沒有印出東西(是不是0)
            if (a.termArray[i].coef > 0) output << "+";//如果是第一項係數則不需要加上正號
            else if (a.termArray[i].coef < 0) output << "-";
        }
        else {//如果前面沒有印出東西(為0)且係數為負，需補上負號
            if (a.termArray[i].coef < 0) output << "-";
            First = true;
        }
        //印出係數的數字,使用abs 取絕對值去除多餘的負號
        output << abs(a.termArray[i].coef);
       
        //判斷指數是否大於0,如果指數<0則切換輸出樣式
        if (a.termArray[i].exp > 0) {
            output << "x^" << a.termArray[i].exp;
        }
        else if (a.termArray[i].exp < 0) {
            output << "/" << "x^" << abs(a.termArray[i].exp);
        }
    }
    if (temp == a.terms) {//迴圈全部跑完後，檢查是不是每一項都是 0
        output << "0";
    }
    return output;
}

int main(void) {
    Polynomial a, b, c, d;
    float e, f, g, h;
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
    g = c.Eval(f);
    h = d.Eval(f);
    cout << "a(x)帶入" << f << "得:" << e << endl;
    cout << "相加結果 a(x)+b(x)=" << c << endl;
    cout << "相乘結果 a(x)*b(x)=" << d << endl;
    cout << "相加結果帶入f=" << g << endl;
    cout << "相乘結果帶入f=" << h << endl;
    return 0;
}
