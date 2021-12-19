#include "cnn.hpp"
#include "matrix.hpp"

int main()
{
    cout << "Please input the path of the image: ";
    string path;
    char c;
    while (true)
    {
        cin >> path;
        try
        {
            faceDetect(path);
        }
        catch (cv::Exception)
        {
            cerr << "Invalid image path!" << endl;
        }
        catch (const char msg)
        {
            cerr << msg << endl;
        }
        cout << "Enter 'y' to detect another image, 'q' to exit! ";
        cin >> c;
        if (c != 'y')
            break;
        cout << endl;
        cout << "Please input the path of the image: ";
    }
    return 0;
}
