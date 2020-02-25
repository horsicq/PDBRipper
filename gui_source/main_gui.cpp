// copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "guimainwindow.h"
#include <QApplication>
#include <QStyleFactory>

struct __TEST
{
    char t1;
    union
    {
      struct
      {
        int t2;
        int t3;
      };
      int t4;
      char t5;
      struct
      {
        char t6;
        char t7;
        char t8;
        char t9;
        char t10;
        int t11;
      };

      struct
      {
          char t12;
          union
          {
              char t13;
              int t14;
          };
          int t15;
      };

      int t16;

      struct
      {
          int t17;
          int t18;
          int t19;
          int t20;
      };
    };
    int t1000;
    char **********t1500;
    int t2000;
    char t2500[10][20][30];
    int t3000;
    void  *Callback(void *);
    void ( *Callback2)(void *);
};

int main(int argc, char *argv[])
{
    __TEST test;
    test.t1=1;
    test.t1++;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    GuiMainWindow w;
    w.show();
    return a.exec();
}
