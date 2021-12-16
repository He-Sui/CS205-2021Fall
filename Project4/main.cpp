#include "matrix.hpp"
#include <chrono>
#define TIME_START start = std::chrono::steady_clock::now();
#define TIME_END                                                                           \
    end = std::chrono::steady_clock::now();                                                \
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
    cout << "duration = " << duration << "ms" << endl;
int main()
{
    {
        //Test case1:
        cout << "Test case1:" << endl;
        Matrix<unsigned char> A(2, 4);
        Matrix<short> B(3, 2);
        Matrix<int> C(3, 4, 2);
        Matrix<float> D(2, 4, 3);
        Matrix<double> E(3, 5, 1);
        A.at(1, 3) = 'a';
        B.at(2, 1) = 3;
        C.at(2, 3, 2) = 100;
        D.at(2, 4, 2) = 3.14;
        E.at(2, 4, 1) = 5.56;
        cout << A << B << C << D << E;
    }

    //Test case2:
    {
        cout << "Test case2:" << endl;
        Matrix<int> A(3, 5);
        Matrix<int> B(A, 1, 2, 2, 2);
        Matrix<int> C(A, 1, 1, 3, 2);
    }

    //Test case3:
    {
        cout << "Test case3:" << endl;
        Matrix<int> B;
        Matrix<int> C;
        {

            Matrix<int> A(3, 5);
            B = A.subMatrix(1, 2, 2, 2);
            C = A.subMatrix(1, 1, 3, 2);
        }
    }

    Matrix<int> A(6, 7, 3);
    Matrix<int> B(5, 8, 3);
    for (int i = 1; i <= A.getRows(); ++i)
        for (int j = 1; j <= A.getCols(); ++j)
            for (int k = 1; k <= A.getChannel(); ++k)
                A.at(i, j, k) = 3 * i - j + 5 * k;
    for (int i = 1; i <= B.getRows(); ++i)
        for (int j = 1; j <= B.getCols(); ++j)
            for (int k = 1; k <= B.getChannel(); ++k)
                B.at(i, j, k) = 2 * i + j + 3 * k;

    //Test case4:
    {
        cout << "Test case4:" << endl;
        Matrix<int> C(A, 2, 4, 3, 2);
        Matrix<int> D = B.subMatrix(3, 5, 3, 3);
        cout << C << D;
    }

    //Test case5:
    {
        cout << "Test case5:" << endl;
        Matrix<int> C(A, 2, 5, 3, 4);
        cout << C;
    }

    //Test case6:
    {
        cout << "Test case6:" << endl;
        Matrix<int> C(A, 2, 2, 5, 5);
        cout << C;
        Matrix<int> D;
        D = C.subMatrix(3, 2, 2, 2);
        cout << D;
    }

    //Test case7:
    {
        cout << "Test case7:" << endl;
        Matrix<int> C(A, 3, 4, 2, 2);
        cout << C;
        C.adjustROI(1, 2, 1, 4);
        cout << C;
        C.adjustROI(-1, 3, 0, 0);
        cout << C;
    }

    //Test case8:
    {
        cout << "Test case8:" << endl;
        Matrix<int> C(A, 3, 4, 2, 2);
        cout << C;
        C.adjustROI(-2, -1, 0, 0);
        cout << C;
        C.adjustROI(0, 0, -1, -3);
        cout << C;
    }

    //Test case9:
    {
        cout << "Test case9:" << endl;
        Matrix<int> C(A, 2, 3, 3, 4);
        Matrix<int> D(B, 1, 1, 3, 4);
        cout << C << D;
        Matrix<int> E = C + D;
        cout << E;
        C.adjustROI(-1, 0, 0, 0);
        cout << C << D;
        cout << C + D;

        Matrix<int> F = 5 + C;
        cout << F;
    }

    //Test case10:
    {
        cout << "Test case10:" << endl;
        Matrix<int> C(A, 2, 3, 3, 4);
        Matrix<int> D(B, 1, 1, 3, 4);
        cout << C << D;
        Matrix<int> E = C - D;
        cout << E;
        C.adjustROI(-1, 0, 0, 0);
        cout << C << D;
        cout << C - D;

        Matrix<int> F = 30 - C;
        cout << F;
    }

    //Test case11:
    {
        cout << "Test case11:" << endl;
        Matrix<int> C(A, 2, 1, 3, 4);
        Matrix<int> D(B, 2, 3, 4, 5);
        cout << C << D;
        cout << C * D;
        D.adjustROI(0, -1, 0, 0);
        cout << C * D;
        cout << C * 2;
    }

    //Test case12:
    {
        cout << "Test case12:" << endl;
        Matrix<int> C(2, 3);
        C = 3;
        Matrix<int> D(2, 3);
        D = 3;
        cout << (C == D) << endl;
        cout << (C != D) << endl;
        cout << (A == B) << endl;
        cout << (A != B) << endl;
    }

    //Test case13:
    {
        cout << "Test case13:" << endl;
        Matrix<int> C(A, 2, 1, 3, 4);
        cout << C;
        Matrix<int> D = C.clone();
        cout << D;
        C.at(1, 1, 1) = 0;
        cout << C;
        cout << D;
    }

    // //Test speed:
    // auto start = std::chrono::steady_clock::now();
    // auto end = std::chrono::steady_clock::now();
    // auto duration = 0L;

    // Matrix<float> M(1024, 2048, 3);
    // Matrix<float> N(2048, 1024, 3);
    // M.readFile("1024-2048-3.txt");
    // N.readFile("2048-1024-3.txt");
    // TIME_START
    // Matrix<float> result = M * N;
    // TIME_END
    // ofstream ofs("out.txt");
    // ofs << result;
    // ofs.close();
    return 0;
}