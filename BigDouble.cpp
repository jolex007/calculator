#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>

using namespace std;

/***************/
/** BigDouble **/
/***************/

unsigned int PRES_BIGDOUBLE = 20;

class BigDouble
{
    friend int main();
    friend void Normalize (BigDouble &);
    friend char comp(BigDouble &, BigDouble &);
    friend char comp_without_sign(BigDouble &, BigDouble &);
    friend char comp_without_sign_2(BigDouble, BigDouble &);
    friend istream& operator >> (istream &, BigDouble &);
    friend ostream& operator << (ostream &, const BigDouble);
    friend BigDouble operator + (BigDouble, BigDouble);
    friend BigDouble operator - (BigDouble, BigDouble);
    friend BigDouble operator * (BigDouble, BigDouble);
    friend BigDouble operator / (BigDouble, BigDouble);
    friend BigDouble operator - (BigDouble);
    friend BigDouble exp(BigDouble);

protected:

    bool sign;
    string number;
    unsigned int unit;

public:
    BigDouble ()
    {
        sign = true;
        number = "0";
        unit = 0;
    }
    BigDouble (string input)
    {
        if (input[0] == '-'){
            sign = false;
            input.erase(input.begin());
        }
        else sign = true;

        unsigned int coun = 0;
        number.clear();
        bool ex = true;

        for (int i=0; i<input.size(); i++){
            if (input[i] == ',' || input[i] == '.'){
                unit = coun - 1;
                ex = false;
            }
            else {
                number.insert(number.begin(),input[i]);
                coun++;
            }
        }
        if (ex) unit = coun - 1;
        Normalize(*this);
        if (number == "0") sign = true;
    }
    BigDouble (string input, bool sign_)
    {
        sign = sign_;
        if (input == "0") sign = true;

        unsigned int coun = 0;
        number.clear();
        bool ex = true;

        for (int i=0; i<input.size(); i++){
            if (input[i] == ',' || input[i] == '.'){
                unit = coun - 1;
                ex = false;
            }
            else {
                number.insert(number.begin(),input[i]);
                coun++;
            }
        }
        if (ex) unit = coun - 1;
        Normalize(*this);
        if (number == "0") sign = true;
    }
    BigDouble (string input, bool sign_, unsigned int unit_)
    {
        sign = sign_;
        unit = unit_;
        if (input == "0") sign = true;

        number.clear();
        for (int i=0; i < input.size(); i++){
            number.push_back(input[i]);
        }
        Normalize(*this);
        if (number == "0") sign = true;
    }
    BigDouble (string input, unsigned int un_unit_, bool sign_)
    {
        //cout << "!!!" << un_unit_ << "!!! ";

        if (un_unit_ > PRES_BIGDOUBLE) {
            input.erase(input.begin(), input.begin() + un_unit_ - PRES_BIGDOUBLE);
            un_unit_ = PRES_BIGDOUBLE;
        }

        sign = sign_;
        number.clear();
        for (int i=0; i < input.size(); i++){
            number.push_back(input[i]);
        }
        unsigned int i;
        for (i = number.size()-1; i>0 && i>un_unit_; i--){
            if (number[i] != '0') break;
        }
        number.resize(i+1);

        unit = number.size() - 1 - un_unit_;
        Normalize(*this);
        if (number == "0") sign = true;
    }
    BigDouble (pair <string, unsigned int> input, bool sign_)
    {
        sign = sign_;
        number.clear();
        for (int i=0; i < input.first.size(); i++){
            number.push_back(input.first[i]);
        }

        unsigned int i;

        unit = input.second;

        for (i = number.size()-1; i>0 && unit > 0; i--){
            if (number[i] != '0') break;
            unit--;
        }
        number.resize(i+1);

        Normalize(*this);
        if (number == "0") sign = true;
    }
    BigDouble operator = (BigDouble b)
    {
        sign = b.sign;
        number = b.number;
        unit = b.unit;
        return *this;
    }
};

/***************/
/** fucntions **/
/***************/

void Compare (string &a, unsigned int &a_unit, string &b, unsigned int &b_unit)
{
    int len = max(a_unit, b_unit);

    while(a_unit < len){
        a.push_back('0');
        a_unit++;
    }
    while(b_unit < len){
        b.push_back('0');
        b_unit++;
    }

    int len_2 = max(a.size() - len, b.size() - len);

    while(a.size() - len < len_2) a.insert(a.begin(), '0');
    while(b.size() - len < len_2) b.insert(b.begin(), '0');
}

void Normalize (BigDouble &a)
{
    while (a.number[0] == '0' && a.number.size() > a.unit + 1) {
        a.number.erase(a.number.begin());
    }
    while (a.number[a.number.size()-1] == '0' && a.number.size() > 1 && a.unit > 0) {
        a.number.erase(a.number.end()-1);
        a.unit--;
    }
}

char comp(BigDouble &a, BigDouble &b)
{
    Normalize(a);
    Normalize(b);
    if (a.sign ^ b.sign){
        if (a.sign) return 1;
        else return -1;
    }
    else{
        if (a.sign){
            if (a.unit > b.unit) return 1;
            if (a.unit < b.unit) return -1;
            int i, j;
            for (i=a.number.size()-1, j=b.number.size()-1; i >= 0 && j >= 0; i--, j--){
                if (a.number[i] > b.number[j]) return 1;
                if (a.number[i] < b.number[j]) return -1;
            }
            if (i == -1 && j != -1) return -1;
            if (j == -1 && i != -1) return 1;
            return 0;
        }
        else {
            if (a.unit > b.unit) return -1;
            if (a.unit < b.unit) return 1;
            int i, j;
            for (i=a.number.size()-1, j=b.number.size()-1; i >= 0 && j >= 0; i--, j--){
                if (a.number[i] > b.number[j]) return -1;
                if (a.number[i] < b.number[j]) return 1;
            }
            if (i == -1 && j != -1) return 1;
            if (j == -1 && i != -1) return -1;
            return 0;
        }
    }
}

char comp_without_sign(BigDouble &a, BigDouble &b)
{
    if (a.unit > b.unit) return 1;
    if (a.unit < b.unit) return -1;
    int i, j;
    for (i=a.number.size()-1, j=b.number.size()-1; i >= 0 && j >= 0; i--, j--){
        if (a.number[i] > b.number[j]) return 1;
        if (a.number[i] < b.number[j]) return -1;
    }
    if (i == -1 && j != -1) return -1;
    if (j == -1 && i != -1) return 1;
    return 0;
}

char comp_without_sign_2(BigDouble a, BigDouble &b)
{
    if (a.unit > b.unit) return 1;
    if (a.unit < b.unit) return -1;
    int i, j;
    for (i=a.number.size()-1, j=b.number.size()-1; i >= 0 && j >= 0; i--, j--){
        if (a.number[i] > b.number[j]) return 1;
        if (a.number[i] < b.number[j]) return -1;
    }
    if (i == -1 && j != -1) return -1;
    if (j == -1 && i != -1) return 1;
    return 0;
}

char comp_string(string &a, string &b)
{
    if (a.size() > b.size()) return 1;
    if (a.size() < b.size()) return -1;
    for (int i=a.size()-1; i >= 0; i--){
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

pair<string, unsigned int> Summ (string a, unsigned int &a_unit, string &b, unsigned int &b_unit)
{
    Compare(a, a_unit, b, b_unit);
    for (unsigned int i=0; i<a.size()-1; i++){
        a[i] += b[i] - '0';
        if (a[i] > '9'){
            a[i] -= 10;
            a[i+1]++;
        }
    }
    unsigned int add = 0;
    a[a.size()-1] += b[b.size()-1] - '0';
    if (a[a.size()-1] > '9'){
        a[a.size()-1] -= 10;
        a.push_back('1');
        add++;
    }
    return make_pair(a, max(a_unit, b_unit) + add);
}

string Minus (string a, unsigned int &a_unit, string &b, unsigned int &b_unit)
{
    Compare(a, a_unit, b, b_unit);
    for (unsigned int i=0; i<a.size(); i++){
        if (a[i] < b[i]){
            a[i+1]--;
            a[i] += 10;
        }
        a[i] -= b[i] - '0';
    }

    return a;
}

string Minus (string a, string &b)
{
    a.resize(max(a.size(), b.size()), '0');
    b.resize(max(a.size(), b.size()), '0');
    for (unsigned int i=0; i<a.size(); i++){
        if (a[i] < b[i]){
            a[i+1]--;
            a[i] += 10;
        }
        a[i] -= b[i] - '0';
    }
    unsigned int i;
    for (i = a.size()-1; i>0; i--){
        if (a[i] != '0') break;
    }
    a.resize(i+1);
    return a;
}

string Naive_mult (string &a, string &b, int L, int R)
{
    string result;
    result.resize(a.size()*2+1, 0);
    for (int i = L; i < R; i++){
        for (int j = L; j < R; j++){
            result[i+j-L] += (a[i] - '0') * (b[j] - '0');
            result[i+j-L+1] += result[i+j-L] / 10;
            result[i+j-L] %= 10;
        }
    }
    for (int i = 0; i < result.size(); i++){
        while (result[i] > 9){
            result[i]-=10;
            result[i+1]++;
        }
        result[i] += '0';
    }
    return result;
}

string Naive_mult_1 (string a, string b, int L, int R) // I think that this will be faster, but it's not true
{
    string result;
    int len = a.size()*2+2;
    int c[len];
    for (int i=0; i<len; i++){
        c[i]=0;
    }
    for (int i = L; i < R; i++){
        for (int j = L; j < R; j++){
            c[i+j-L] += (a[i] - '0') * (b[j] - '0');
        }
    }
    for (int i = 0; i < len-1; i++){
        c[i+1] += c[i]/10;
        c[i] %= 10;
    }
    for (int i = 0; i < len; i++){
        result.push_back(c[i] + '0');
    }
    unsigned int i;
    for (i = result.size()-1; i>0; i--){
        if (result[i] != '0') break;
    }
    result.resize(i+1);

    return result;
}

string Mult (string a, string b)
{
    a.resize(max(a.size(), b.size()), '0');
    b.resize(max(a.size(), b.size()), '0');

    return Naive_mult(a, b, 0, a.size());
}

pair <string, unsigned int> Mod (string a, unsigned int a_unit, string b, unsigned int b_unit)
{
    if (a == "0"){
        return make_pair("0", 0);
    }
    string answer;
    unsigned int answer_unit = 0;
    /*
    if (a.size() - a_unit < b.size() - b_unit){
        unsigned int need_size = (b.size() - b_unit) - (a.size() - a_unit);
        for (unsigned int i=0; i < need_size; i++) {
            a.insert(a.begin(), '0');
        }
    }
    else {
        unsigned int need_size = (a.size() - a_unit) - (b.size() - b_unit);
        for (unsigned int i=0; i < need_size; i++) {
            b.insert(b.begin(), '0');
        }
    }
    */
    unsigned int un = b.size() - b_unit - 1;
    for (unsigned int i=0; i < un; i++){
        if (a[a.size()-1] == '0'){
            a.erase(a.end()-1);
        }
        else {
            a_unit++;
            if (a_unit + 1 > a.size()) a.insert(a.begin(), '0');
        }
    }
    un = a.size() - a_unit - 1;
    for (unsigned int i=0; i < un; i++){
        b.insert(b.begin(), '0');
    }
    while (b[b.size()-1] == '0'){
        b.erase(b.end()-1);
    }


    int length = b.size();
    while (b.size() < a.size()){
        b.insert(b.begin(), '0');
    }
    while (b.size() >= length){
        int coun;
        for (coun = 0; comp_string(a, b) >= 0; coun++){
            a = Minus(a, b);
            while (b[b.size()-1] == '0'){
                b.erase(b.end()-1);
            }
        }
        answer.insert(answer.begin(), (char)(coun + '0'));
        if (b.size() <= length) break;
        answer_unit++;
        b.erase(b.begin());
    }
    while (a[a.size()-1] == '0') a.erase(a.end()-1);
    for (unsigned int i=0; i<PRES_BIGDOUBLE && a!="0"; i++) {
        a.insert(a.begin(), '0');
        int coun;
        for (coun = 0; comp_string(a, b) >= 0; coun++){
            a = Minus(a, b);
            while (b[b.size()-1] == '0'){
                b.erase(b.end()-1);
            }
        }
        answer.insert(answer.begin(), (char)(coun + '0'));
    }
    while (answer[answer.size()-1] == '0'){
        if (answer.size() == 1 || answer_unit == 0) break;

        answer_unit--;
        answer.erase(answer.end()-1);
    }
    return make_pair (answer, answer_unit);
}

/*****************/
/** overloading **/
/*****************/

istream& operator >> (istream &in, BigDouble &answer)
{
    string input;
    in >> input;
    if (input[0] == '-'){
        answer.sign = false;
        input.erase(input.begin());
    }
    else answer.sign = true;

    unsigned int coun = 0;
    answer.number.clear();
    bool ex = true;

    for (int i=0; i<input.size(); i++){
        if (input[i] == ',' || input[i] == '.'){
            answer.unit = coun - 1;
            ex = false;
        }
        else {
            answer.number.insert(answer.number.begin(),input[i]);
            coun++;
        }
    }
    if (ex) answer.unit = coun - 1;
    Normalize(answer);
	return in;
}

ostream& operator << (ostream &out, const BigDouble input)
{
    if (!input.sign) out << '-';
    int coun = 0;
    for (int i=input.number.size()-1; i>=0; i--, coun++){
        out << input.number[i];
        if (coun == input.unit && i != 0) out << '.';
    }
    return out;
}

bool operator == (BigDouble &a, BigDouble &b)
{
    if (comp(a, b) == 0) return true;
    else return false;
}

bool operator != (BigDouble &a, BigDouble &b)
{
    if (comp(a, b) != 0) return true;
    else return false;
}

bool operator > (BigDouble a, BigDouble b)
{
    if (comp(a, b) == 1) return true;
    else return false;
}

bool operator >= (BigDouble a, BigDouble b)
{
    if (comp(a, b) >= 0) return true;
    else return false;
}

bool operator < (BigDouble a, BigDouble b)
{
    if (comp(a, b) == -1) return true;
    else return false;
}

bool operator <= (BigDouble a, BigDouble b)
{
    if (comp(a, b) <= 0) return true;
    else return false;
}

BigDouble operator + (BigDouble a, BigDouble b)
{
    if (a.sign ^ b.sign){
        if (comp_without_sign(a, b) >= 0){
            return BigDouble(Minus(a.number, a.unit, b.number, b.unit), a.sign, max(a.unit, b.unit));
        }
        else {
            return BigDouble(Minus(b.number, b.unit, a.number, a.unit), b.sign, max(a.unit, b.unit));
        }
    }
    else {
        return BigDouble(Summ(a.number, a.unit, b.number, b.unit), a.sign);
        //return BigDouble(Summ(a.number, a.unit, b.number, b.unit), a.sign, max(a.unit, b.unit));
    }
}

BigDouble operator += (BigDouble &a, BigDouble b)
{
    return (a = a + b);
}

BigDouble operator - (BigDouble a, BigDouble b)
{
    if (a.sign ^ b.sign){
        return BigDouble(Summ(a.number, a.unit, b.number, b.unit), a.sign);
    }
    else {
        if (comp_without_sign(a, b) >= 0){
            return BigDouble(Minus(a.number, a.unit, b.number, b.unit), a.sign, max(a.unit, b.unit));
        }
        else {
            return BigDouble(Minus(b.number, b.unit, a.number, a.unit), !a.sign, max(a.unit, b.unit));
        }
    }
}

BigDouble operator -= (BigDouble &a, BigDouble b)
{
    return (a = a - b);
}

BigDouble operator * (BigDouble a, BigDouble b)
{
    if (a.sign ^ b.sign){
        return BigDouble(Mult(a.number, b.number), a.number.size() + b.number.size() - a.unit - b.unit - 2, false);
    }
    else {
        return BigDouble(Mult(a.number, b.number), a.number.size() + b.number.size() - a.unit - b.unit - 2, true);
    }
}

BigDouble operator *= (BigDouble &a, BigDouble b)
{
    return (a = a * b);
}

BigDouble operator / (BigDouble a, BigDouble b)
{
    if (a.sign ^ b.sign){
        return BigDouble(Mod(a.number, a.unit, b.number, b.unit), false);
    }
    else {
        return BigDouble(Mod(a.number, a.unit, b.number, b.unit), true);
    }
}

BigDouble operator /= (BigDouble &a, BigDouble b)
{
    return (a = a / b);
}

BigDouble operator - (BigDouble a)
{
    a.sign = a.sign ^ true;
    return a;
}

/***************/
/** constants **/
/***************/

const BigDouble pi("3,14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196");
const BigDouble e ("2,71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200305992181741359662904357290033429526059563073813232862794349076323382988075319525101901");

/*********************/
/** other functions **/
/*********************/

BigDouble sin (BigDouble input)
{
    while (input < -pi){
        input += pi;
    }
    while (input > pi){
        input -= pi;
    }
    BigDouble answer("0");
    BigDouble up_drob;
    up_drob = input;
    BigDouble down_drob ("1");
    BigDouble multfac ("2");
    BigDouble one ("1");
    for (int i=0; i<10; i++){
        answer += up_drob / down_drob;
        up_drob *= input * input;
        down_drob = down_drob * multfac;
        multfac += one;
        down_drob = down_drob * multfac;
        multfac += one;

        answer -= up_drob / down_drob;
        up_drob *= input * input;
        down_drob = down_drob * multfac;
        multfac += one;
        down_drob = down_drob * multfac;
        multfac += one;
    }
    return answer;
}

BigDouble cos (BigDouble input)
{
    while (input < -pi){
        input += pi;
    }
    while (input > pi){
        input -= pi;
    }
    BigDouble answer("1");
    BigDouble up_drob;
    up_drob = input * input;
    BigDouble down_drob ("2");
    BigDouble multfac ("3");
    BigDouble one ("1");
    for (int i=0; i<10; i++){
        answer -= up_drob / down_drob;
        up_drob *= input * input;
        down_drob = down_drob * multfac;
        multfac += one;
        down_drob = down_drob * multfac;
        multfac += one;

        answer += up_drob / down_drob;
        up_drob *= input * input;
        down_drob = down_drob * multfac;
        multfac += one;
        down_drob = down_drob * multfac;
        multfac += one;
    }
    return answer;
}

BigDouble sqrt(BigDouble input)
{
    BigDouble a;
    BigDouble a_prev;
    BigDouble need("0.5");
    BigDouble four("4");
    if (input >= four){
        a_prev = input;
        a = input*need;
    }
    else {
        a_prev = input*need;
        a = input;
    }
    BigDouble for_comp("0.000000001");

    while (comp_without_sign_2(a_prev - a, for_comp) >= 0) {
        a_prev = a;
        a = (a + (input / a)) * need;
    }

    return a;
}

BigDouble exp_1(BigDouble input)
{
    BigDouble answer("1");
    BigDouble for_mult("2");
    BigDouble down_drob("1");
    BigDouble one("1");
    BigDouble up_drob;
    up_drob = input;
    for (int i=0; i<18; i++){
        answer += up_drob / down_drob;
        down_drob *= for_mult;
        for_mult += one;
        up_drob *= input;
    }

    return answer;
}

BigDouble log_1(BigDouble input)
{
    BigDouble one("1");
    //if (input <= one-one) return BigDouble("3");
    input -= one;
    BigDouble answer("0");
    BigDouble up_drob;
    up_drob = input;
    BigDouble down_drob("1");
    for (int i=0; i<20; i++){
        answer += up_drob / down_drob;
        up_drob *= input;
        down_drob += one;
        answer -= up_drob / down_drob;
        up_drob *= input;
        down_drob += one;
    }

    return answer;
}

BigDouble log(BigDouble input)
{
    BigDouble one("1");

    if (input <= one) {
        return log_1(input);
    }
    else {
        BigDouble x_prev("0");
        BigDouble x("10");
        BigDouble for_comp("0.0001");

        while (comp_without_sign_2(x_prev - x, for_comp) >= 0) {
            x_prev = x;
            x = x - one + (input / (exp_1(x)));
        }
        return x;
    }
}

BigDouble exp(BigDouble input) // TODO: exp
{
    BigDouble one("1");
    if (input.sign == false) {
        BigDouble x_prev("0.1");
        BigDouble x("0.000001");
        BigDouble for_comp("0.00000001");

        while (comp_without_sign_2(x_prev - x, for_comp) >= 0) {
            x_prev = x;
            cout << x << " " << input + one - log(x) << " " << log(x) << '\n';
            x = x * (input + one - log_1(x));
            //cout << x << '\n';
        }
        return x;
    }
    else {
        return exp_1(input);
    }
}

/**********/
/** main **/
/**********/

int main()
{
    BigDouble a("-5");
    BigDouble b("0.0000001");
    cout << log(0.0000001) << '\n';

    //cout << exp(a);
    cout << log(b);
    //cout << a*b;
    return 0;
}
