#include <Windows.h>
#include <iostream>

using namespace std;

std::string_view PIPE_NAME = R"(\\.\pipe\rolling-test)";

class ClientPipe
{
	HANDLE hPipe;

public:
	ClientPipe() :hPipe(nullptr) {}
	~ClientPipe() {
		Disconnect();
	}

	void Connect() {
		while (hPipe == nullptr)
		{
			hPipe = CreateFile(
				PIPE_NAME.data(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL
			);
			if (hPipe == INVALID_HANDLE_VALUE)
			{
				auto errorCode = GetLastError();
				if (errorCode != ERROR_PIPE_BUSY)
				{
					cout << "Failed to connect to pipe " << errorCode << endl;
					break;
				}
			}
			cout << "Connected to pipe" << endl;
		}
	}

	void Disconnect() {
		cout << "Disconnect()" << endl;
		if (hPipe != nullptr)
		{
			CloseHandle(hPipe);
			hPipe = nullptr;
		}
	}

	void Send(const char* message) {
		DWORD bytesWritten;
		if (!WriteFile(hPipe, message, strlen(message), &bytesWritten, NULL))
		{
			cout << "Failed to write to pipe" << endl;
		}
		else
		{
			cout << "Sent: " << message << endl;
		}
	}
};

int main() {
	cout << "Hello from client!" << endl;
	
	ClientPipe pe;
	pe.Connect();
	pe.Send("Hello from client!");
	pe.Send("hello 2");
	pe.Send("hello 3");
	Sleep(20);
	pe.Send("hello 4");
	pe.Send("hello 5");

	system("pause");

	return 0;
}