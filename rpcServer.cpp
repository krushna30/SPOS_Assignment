#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <bits/stdc++.h>
using namespace std;

struct answers
{
	int ans;
};

struct objs
{
	int l, b, h;
};
void unpack(objs &p, int &l, int &b, int &h)
{
	l = p.l;
	b = p.b;
	h = p.h;
};
int calc(int l, int b, int h)
{
	int ans;
	ans = l * b * h;
	return ans;
};
void pack(int t, answers &obj)
{
	obj.ans = t;
}

int main()
{
	int s_sock, c_sock;
	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	socklen_t add;

	if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		printf("Binding failed\n");
		return 0;
	}

	listen(s_sock, 10);
	add = sizeof(other);
	// accepting the connect request from client
	c_sock = accept(s_sock, (struct sockaddr *)&other, &add);

	// accepting the number

	objs number;
	recv(c_sock, &number, sizeof(number), 0);
	int l, b, h;
	unpack(number, l, b, h);
	int check = calc(l, b, h);
	// sending the answer
	answers obj;
	pack(check, obj);
	send(c_sock, &obj, sizeof(obj), 0);

	cout << "Server is going to close\n";

	close(c_sock);
	close(s_sock);
	return 0;
}