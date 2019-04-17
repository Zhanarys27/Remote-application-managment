#pragma comment(lib, "WS2_32.lib")

#include <WinSock2.h>
#include <stdio.h>
#include <initguid.h>
#include <ws2bth.h>
#include <strsafe.h>
#include <stdint.h>
#include "Shellapi.h"
#include <windows.h>
#include <string.h> 
#include "ShellAPI.h"
#include "keyboard.cpp"

DEFINE_GUID(g_guidServiceClass, 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);

#define DEFAULT_LISTEN_BACKLOG 4
#define INSTANCE_STR L"BluetoothWindows"
#define LEN_RECV 256

POINT mouse;
BOOL buffer_handler(char *buff) {
	GetCursorPos(&mouse);
	if (!strcmp(buff, "mouseUp")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x,mouse.y - 3);
			
		}
		return 0;
	}
	if (!strcmp(buff, "mouseDown")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x, mouse.y + 3);
		
		}
		return 0;
	}

	if (!strcmp(buff, "mouseRigth")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x + 3, mouse.y);
		}
		return 0;

	}
	if (!strcmp(buff, "mouseLeft")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x - 3, mouse.y);
		}
		return 0;

	}

	if (!strcmp(buff, "mouseDownLeft")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x - 3, mouse.y + 3);
		}
		return 0;
	}

	if (!strcmp(buff, "mouseDownRight")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x + 3, mouse.y + 3);
		}
		return 0;
	}

	if (!strcmp(buff, "mouseUpLeft")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x - 3, mouse.y - 3);
		}
		return 0;
	}
	if (!strcmp(buff, "mouseUpRigth")) {
		for (int i = 0; i < 10; i++) {
			SetCursorPos(mouse.x + 3, mouse.y - 3);
		}
		return 0;
	}
 if (!strcmp(buff, "mouseRightClick")) {
		GetCursorPos(&mouse);
		mouse_event(MOUSEEVENTF_RIGHTDOWN, mouse.x, mouse.y, 0, 0);
		mouse_event(MOUSEEVENTF_RIGHTUP, mouse.x, mouse.y, 0, 0);
	}

 if (!strcmp(buff, "mouseLeftClick")) {
		GetCursorPos(&mouse);
		mouse_event(MOUSEEVENTF_LEFTDOWN, mouse.x, mouse.y, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, mouse.x, mouse.y, 0, 0);
	}
 
 if (!strcmp(buff, "mouseWheelUp")) {
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 120, 0);
	}

  if (!strcmp(buff, "mouseWheelDown")) {
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, (DWORD)-120, 0);
	}

	if (!strcmp(buff, "play")) {
		SendVirtualKey(0x20, FALSE);
		SendVirtualKey(0x20, TRUE);
		return 0;
	}
	if (!strcmp(buff, "stop")) {
		SendVirtualKey(0x20, FALSE);
		SendVirtualKey(0x20, TRUE);
		
		return 0;
	}

	if (!strcmp(buff, "next")) {
		SendVirtualKey(0x71, FALSE);
		SendVirtualKey(0x71, TRUE);
		
		

		return 0;
	}

	if (!strcmp(buff, "prev")) {
		SendVirtualKey(0x70, FALSE);
		SendVirtualKey(0x70, TRUE);

		return 0;
	}
	if (!strcmp(buff, "nextS")) {
		SendVirtualKey(0x58, FALSE);
		SendVirtualKey(0x58, TRUE);
		return 0;
	}

	if (!strcmp(buff, "prevS")) {
		SendVirtualKey(0x5A, FALSE);
		SendVirtualKey(0x5A, TRUE);
		return 0;
	}
	if (!strcmp(buff, "aimp")) {
		ShellExecute(NULL, TEXT("open"), TEXT("c:\\Program Files (x86)\\AIMP\\AIMP.EXE"), TEXT(""), NULL, SW_SHOWNORMAL);
		return 0;
	}
	
	return TRUE;
}

static void print_error(char const *where, int code)
{
	fprintf(stderr, "Error on %s: code %d\n", where, code);
}

static BOOL bind_socket(SOCKET local_socket, SOCKADDR_BTH *sock_addr_bth_local)
{
	int addr_len = sizeof(SOCKADDR_BTH);

	/* Setting address family to AF_BTH indicates winsock2 to use Bluetooth port. */
	sock_addr_bth_local->addressFamily = AF_BTH;
	sock_addr_bth_local->port = BT_PORT_ANY;

	if (bind(local_socket, (struct sockaddr *) sock_addr_bth_local, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR) {
		print_error("bind()", WSAGetLastError());
		return FALSE;
	}

	if (getsockname(local_socket, (struct sockaddr *)sock_addr_bth_local, &addr_len) == SOCKET_ERROR) {
		print_error("getsockname()", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

static LPCSADDR_INFO create_addr_info(SOCKADDR_BTH *sock_addr_bth_local)
{
	LPCSADDR_INFO addr_info = calloc(1, sizeof(CSADDR_INFO));

	if (addr_info == NULL) {
		print_error("malloc(addr_info)", WSAGetLastError());
		return NULL;
	}
	addr_info[0].LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	addr_info[0].LocalAddr.lpSockaddr = (LPSOCKADDR)sock_addr_bth_local;
	addr_info[0].RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	addr_info[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&sock_addr_bth_local;
	addr_info[0].iSocketType = SOCK_STREAM;
	addr_info[0].iProtocol = BTHPROTO_RFCOMM;
	return addr_info;
}

BOOL receive_data(SOCKET client_socket)
{
	char *buffer = NULL;
	int len_read = 0;
		
	buffer = calloc(LEN_RECV, sizeof(char*));
	if (buffer == NULL) {
		print_error("malloc(buffer)", WSAGetLastError());
		return FALSE;
	}

	len_read = recv(client_socket, buffer, LEN_RECV, 0);
	if (len_read == SOCKET_ERROR) {
		free(buffer);
		print_error("recv()", WSAGetLastError());
		return FALSE;
	}
	buffer_handler(buffer);
	/*if (len_read == 0) {
	free(buffer);
	fprintf(stderr, "Nothing read, end of communication\n");
	return FALSE;
	}*/
  //code_handler(buffer);
	free(buffer);
	return TRUE;
}

static BOOL advertise_service_accepted(LPCSADDR_INFO addr_info, wchar_t **instance_name)
{
	WSAQUERYSET wsa_query_set = { 0 };
	wchar_t computer_name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len_computer_name = MAX_COMPUTERNAME_LENGTH + 1;
	size_t instance_name_size = 0;
	HRESULT res;

	if (!GetComputerName(computer_name, &len_computer_name)) {
		print_error("GetComputerName()", WSAGetLastError());
		return FALSE;
	}


	res = StringCchLength(computer_name, sizeof(computer_name), &instance_name_size);
	if (FAILED(res)) {
		print_error("ComputerName specified is too large", WSAGetLastError());
		return FALSE;
	}

	instance_name_size += sizeof(INSTANCE_STR) + 1;
	*instance_name = malloc(instance_name_size);
	if (*instance_name == NULL) {
		print_error("malloc(instance_name)", WSAGetLastError());
		return FALSE;
	}

	/* If we got an address, go ahead and advertise it. */
	ZeroMemory(&wsa_query_set, sizeof(wsa_query_set));
	wsa_query_set.dwSize = sizeof(wsa_query_set);
	wsa_query_set.lpServiceClassId = (LPGUID)&g_guidServiceClass;

	StringCbPrintf(*instance_name, instance_name_size, L"%s %s", computer_name, INSTANCE_STR);
	wsa_query_set.lpszServiceInstanceName = *instance_name;
	wsa_query_set.lpszComment = L"Example of server on Windows expecting bluetooth connections";
	wsa_query_set.dwNameSpace = NS_BTH;
	wsa_query_set.dwNumberOfCsAddrs = 1; /* Must be 1. */
	wsa_query_set.lpcsaBuffer = addr_info; /* Req'd */

																				
	if (WSASetService(&wsa_query_set, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR) {
		free(instance_name);
		print_error("WSASetService()", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL run_server_mode()
{
	wchar_t *       instance_name = NULL;
	SOCKET          local_socket = INVALID_SOCKET;
	SOCKADDR_BTH    sock_addr_bth_local = { 0 };
	LPCSADDR_INFO   addr_info = NULL;
	//ring_buffer_t   *rb = NULL;
	BOOL ret = FALSE;

	/* Open a bluetooth socket using RFCOMM protocol. */
	local_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (local_socket == INVALID_SOCKET) {
		print_error("socket()", WSAGetLastError());
		return FALSE;
	}

	ret = bind_socket(local_socket, &sock_addr_bth_local);
	if (!ret) {
		return FALSE;
	}
	addr_info = create_addr_info(&sock_addr_bth_local);
	if (!addr_info) {
		return FALSE;
	}
	ret = advertise_service_accepted(addr_info, &instance_name);
	if (!ret) {
		free(addr_info);
		if (instance_name) {
			free(instance_name);
		}
		return FALSE;
	}

	if (listen(local_socket, DEFAULT_LISTEN_BACKLOG) == SOCKET_ERROR) {
		print_error("listen()", WSAGetLastError());
		free(addr_info);
		free(instance_name);
		return FALSE;
	}

	while (1) {
		SOCKET client_socket = accept(local_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET) {
			print_error("accept()", WSAGetLastError());
			return FALSE;
		}
		printf("\nClient connected !\n");	

		ret = TRUE;
		while (ret == TRUE) {
			ret = receive_data(client_socket);			
		}
		printf("\nCommunication over\n");
		closesocket(client_socket);	
	}

	free(addr_info);
	free(instance_name);
	closesocket(local_socket);
	return TRUE;
}

int main(int argc, char **argv)
{
	int i = 1000;
	MSG msg;
	BOOL bRet;

	int cas=0;
	/*HWND hwnd;
	ShellExecute(NULL, TEXT("open"), TEXT("c:\\Program Files (x86)\\Microsoft Office\\Office15\\POWERPNT.EXE"),	TEXT(""), NULL, SW_SHOWNORMAL);
	Sleep(5000);
	SendVirtualKey(0x74, FALSE);
	Sleep(3000);
	SendVirtualKey(0x0D, FALSE);
	Sleep(3000);
	SendVirtualKey(0x0D, FALSE);*/


	WSADATA WSAData = { 0 };
	int ret = 0;
	(void)argc;
	(void)argv;
	printf("Start the server...\n");
	ret = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (ret < 0) {
		print_error("WSAStartup()", GetLastError());
		return EXIT_FAILURE;
	}

	run_server_mode();

	WSACleanup();

	system("pause");

	


	return EXIT_SUCCESS;
}


