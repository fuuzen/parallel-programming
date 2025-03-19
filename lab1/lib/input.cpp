#include "lib.h"

int input (int min, int max) {
  int num;
  while (true) {
    cout << "请输入一个范围在 " << min << " 到 " << max << " 之间的整数: ";
    if (cin >> num) {
      if (num < min || num > max) {
        cout << "输入无效，超出范围" << endl;
      } else {
        break;  // 输入有效，退出循环
      }
    } else {
      cout << "输入无效，请输入一个整数" << endl;
      cin.clear();  // 清除错误标志
      cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略错误输入
    }
  }
  return num;
}