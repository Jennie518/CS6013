#include <iostream>  // 确保包含了 iostream 头文件
#include <sys/time.h>
#include <cstring>
extern "C" {
    void sayHello();
    int myPuts(const char* s, int len);
    struct timeval myGTOD();
}

int main() {
    sayHello();  // 调用汇编函数打印 "Hello, world!"
    myPuts("Hello from myPuts!", 19);  // 调用汇编函数打印 "Hello from myPuts!"
    // 调用汇编函数 myGTOD 获取当前时间
    struct timeval tv = myGTOD();
    // 输出获取到的时间
    std::cout << "Seconds: " << tv.tv_sec << std::endl;
    std::cout << "Microseconds: " << tv.tv_usec << std::endl;
    timeval tv_std;
	gettimeofday(&tv_std, nullptr);
	// Check if the seconds are the same or off by 1 (accounting for execution time)
	if (tv.tv_sec == tv_std.tv_sec ||
	  std::abs(tv.tv_sec - tv_std.tv_sec) == 1) {
		std::cout << "Test Passed.\n";
	} else {
		std::cout << "Test Failed.\n";
	}
    return 0;
}


