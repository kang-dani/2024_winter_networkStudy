#include "lib.h"
#include <fcntl.h>
#include <fstream>

int main() {
    // 소켓 생성
    int servsock = socket(AF_INET, SOCK_STREAM, 0);
    if (servsock == INVALID_SOCKET) {
        cout << "socket() error" << endl;
        return 0;
    }

    // 논블로킹 모드로 설정
    int flags = fcntl(servsock, F_GETFL, 0);
    fcntl(servsock, F_SETFL, flags | O_NONBLOCK);

    // 서버 주소 설정
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(12341);

    // 소켓에 주소 바인딩
    if (bind(servsock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        cout << "bind() error" << endl;
        return 0;
    }

    // 클라이언트 연결 대기
    if (listen(servsock, SOMAXCONN) == SOCKET_ERROR) {
        cout << "listen() error" << endl;
        return 0;
    }

    while (true) {
        // 클라이언트 연결 수락
        sockaddr_in cliAddr;
        uint addrlen = sizeof(cliAddr);
        int clisock = accept(servsock, (sockaddr*)&cliAddr, &addrlen);
        if (clisock == INVALID_SOCKET) {
            if (errno == EWOULDBLOCK) {  // 논블로킹 모드에서 연결 요청이 없는 경우
                continue;
            } else {
                cout << "accept() error" << endl;
                return 0;
            }
        }

        // 클라이언트의 요청 받기
        char request[1024] = "";
        int recvlen = recv(clisock, request, sizeof(request), 0);
        if (recvlen == SOCKET_ERROR) {
            if (errno == EWOULDBLOCK) {  // 논블로킹 모드에서 데이터가 없는 경우
                continue;
            } else {
                cout << "recv() error" << endl;
                close(clisock);
                close(servsock);
                return 0;
            }
        }

        // HTML 파일 읽기
        ifstream file("linuxPage.html");  // linuxPage.html 파일 열기
        if (!file) {  // 파일 열기 실패 시
            cout << "File open error" << endl;
            close(clisock);
            close(servsock);
            return 0;
        }

        // 파일 내용을 문자열로 저장
        string response_body((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();  // 파일 닫기

        // HTTP 응답 생성
        string response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        string response = response_header + response_body;

        // 클라이언트에 응답 보내기
        int sendlen = send(clisock, response.c_str(), response.length(), 0);
        if (sendlen == SOCKET_ERROR) {
            if (errno == EWOULDBLOCK) {  // 논블로킹 모드에서 버퍼가 가득 찬 경우
                continue;
            } else {
                cout << "send() error" << endl;
                close(clisock);
                close(servsock);
                return 0;
            }
        }

        // 소켓 종료
        close(clisock);
    }

    close(servsock);

    return 0;
}
