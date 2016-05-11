/*
   监听服务器2，建立连接后，接收询问字符串，进行[处理]，通过Protocol_2返回结果，然后关闭连接。
*/

#include <iostream>

#include "Configure.h"
#include "Listen.h"
#include "Process.h"

int main() {
	Configure conf("Query.conf");
	Listen host(conf.getString("PortListen"));
	Process pro(conf.getInt("MaxLenQueryText"),conf.getInt("MaxLenTitle"));
	host.Solve(pro);

	return 0;
}
