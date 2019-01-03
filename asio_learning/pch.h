// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H

// TODO: 添加要在此处预编译的标头
#define _CRT_SECURE_NO_WARNINGS
// Windows使用asio源码，不使用boost
// https://blog.csdn.net/qq_29846883/article/details/71515435
///#define ASIO_NO_WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h> /* #define _WIN32_WINNT */
#define ASIO_STANDALONE /* 单独使用asio */

#include <asio.hpp>

#include <iostream>
#include <iomanip> //std::put_time
inline std::string make_daytime_string()
{
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	static std::stringstream ss;
	ss.str("");
	ss << std::put_time(std::localtime(&t), "%Y年%m月%d日 %H点%M分%S秒");
	return ss.str();
}

#endif //PCH_H
