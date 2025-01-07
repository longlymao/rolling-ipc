#include <Windows.h>
#include <iostream>

using namespace std;

static std::string_view PIPE_NAME = R"(\\.\pipe\rolling-test)";

void CreatePipe()
{
	HANDLE hPipe = CreateNamedPipe(
		PIPE_NAME.data(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		4096,
		4096,
		0,
		NULL
	);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to create pipe" << endl;
		return;
	}
	cout << "Pipe created" << endl;
	if (!ConnectNamedPipe(hPipe, NULL))
	{
		cout << "Failed to connect to pipe" << endl;
		CloseHandle(hPipe);
		return;
	}
	cout << "Connected to pipe" << endl;
	char buffer[4096];
	DWORD bytesRead;
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		LARGE_INTEGER fileSize;
		GetFileSizeEx(hPipe, &fileSize);
		cout << "File size: " << fileSize.QuadPart << endl;
		if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL))
		{
			cout << "Failed to read from pipe" << endl;
			break;
		}
		if (bytesRead == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}
		cout << "Received: " << buffer << endl;
		Sleep(2000);
	}
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
}

int main() {
	cout << "Hello from server!" << endl;

	CreatePipe();

	system("pause");

	return 0;
}