#include <iostream>

int main()
{
	TcpClient tcpClient;

	tcpClient.onConnect([](const Event &e) {
				e.getConnection().post("hello world\n");
				e.getconnection().putUserData(new string("hello world\n"));
			}).onMessage([](const Event &e) {
				Buffer &buf = e.getMessage();
				string str;
				while (buf.findCrlf()) {
					buf.readLine(str);
					e.getConnection().post(e.getConnection.getUserData());
				}
			}).onError([](const Event &e) {
				e.getconnection().close();
			});

	for (int i = 0; i < 10240; i++) {
		tcpClient.connect(InetAddr("127.0.0.1", 10000));
	}

	return tcpClient.loop();
}



