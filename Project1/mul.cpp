#include <iostream>
#include <string>

using namespace std;
static bool isNegative(string);
static bool isDigit(string);
static string removeHighDigitZeros(string);
static void multiplication(string, string, bool, bool);

int main(int argc, char *argv[])
{
    string num1, num2;
    if (argc < 3)
    {
        cout << "Please input two integers" << endl;
        cin >> num1 >> num2;
    }
    else
    {
        num1 = argv[1];
        num2 = argv[2];
    }

    bool num1Neg = isNegative(num1);
    bool num2Neg = isNegative(num2);

    // Remove the "-"
    if (num1Neg)
        num1 = num1.substr(1);
    if (num2Neg)
        num2 = num2.substr(1);

    bool num1IsDigit = isDigit(num1);
    bool num2IsDigit = isDigit(num2);

    while (!(num1IsDigit && num2IsDigit))
    {
        if (!num1IsDigit && !num2IsDigit)
        {
            cerr << "The inputs are not correct, please re-input two integers" << endl;
            cin >> num1 >> num2;
            num1Neg = isNegative(num1);
            num2Neg = isNegative(num2);
            if (num1Neg)
                num1 = num1.substr(1);
            if (num2Neg)
                num2 = num2.substr(1);
            num1IsDigit = isDigit(num1);
            num2IsDigit = isDigit(num2);
        }
        else if (!num1IsDigit)
        {
            cerr << "The first input is not correct, please re-input the first integer" << endl;
            cin >> num1;
            num1Neg = isNegative(num1);
            if (num1Neg)
                num1 = num1.substr(1);
            num1IsDigit = isDigit(num1);
        }
        else
        {
            cerr << "The second input is not correct, please re-input the second integer" << endl;
            cin >> num2;
            num2Neg = isNegative(num2);
            if (num2Neg)
                num2 = num2.substr(1);
            num2IsDigit = isDigit(num2);
        }
    }
    num1 = removeHighDigitZeros(num1);
    num2 = removeHighDigitZeros(num2);

    multiplication(num1, num2, num1Neg, num2Neg);
    return 0;
}

static bool isNegative(string num)
{
    return num[0] == '-';
}

static bool isDigit(string num)
{
    for (char m : num)
    {
        if (!(m >= 48 && m <= 57))
            return false;
    }
    return true;
}

static string removeHighDigitZeros(string num)
{
    if (num.length() == 1)
        return num;
    int flag = 0;
    for (char m : num)
    {
        if (m == '0')
            flag++;
        else
            break;
    }
    if (flag > 0)
        num = num.substr(flag);
    return num;
}

static void multiplication(string num1, string num2, bool num1Neg, bool num2Neg)
{
    int m[num1.length()], n[num2.length()];
    int resultMaxLength = num1.length() + num2.length();
    int result[resultMaxLength] = {};
    for (int i = 0; i < num1.length(); i++)
        m[num1.length() - i - 1] = num1[i] - '0';
    for (int j = 0; j < num2.length(); j++)
        n[num2.length() - j - 1] = num2[j] - '0';

    //calculate the result by the way people mainly used

    for (int j = 0; j < num2.length(); j++)
    {
        for (int i = 0; i < num1.length(); i++)
        {

            result[i + j] += m[i] * n[j];

            int counter = 0;
            while (result[i + j + counter] > 9)
            {
                result[i + j + 1] += result[i + j] / 10;
                result[i + j] %= 10;
                counter++;
            }
        }
    }

    int flag = 0;
    for (int k = resultMaxLength - 1; k >= 0; k--)
    {
        if (result[k] != 0)
        {
            flag = k;
            break;
        }
    }

    // check whether the result is 0
    bool resultIsZero = true;
    for (int l = flag; l >= 0; l--)
    {
        if (result[l] != 0)
        {
            resultIsZero = false;
            break;
        }
    }

    //print the result
    cout << "The result is: ";
    if (num1Neg != num2Neg && !resultIsZero)
        cout << "-";
    for (int l = flag; l >= 0; l--)
        cout << result[l];
    cout << endl;
}