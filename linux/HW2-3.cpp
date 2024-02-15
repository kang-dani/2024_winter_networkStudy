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
    servAddr.sin_port = htons(12342);

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

        // 요청 분석하기
        string request_str(request);
        size_t method_end = request_str.find(' ');  // 첫 번째 공백의 위치 찾기
        if (method_end == string::npos) {   // 잘못된 양식에 대한 오류 처리
            close(clisock);
            continue;
        }
        size_t url_start = method_end + 1;  // URL의 시작 위치
        size_t url_end = request_str.find(' ', url_start);  // 두 번째 공백의 위치 찾기
        if (url_end == string::npos) {
            close(clisock);
            continue;
        }
        string url = request_str.substr(url_start, url_end - url_start);  // URL 추출

        // URL에 따라 다른 내용 읽기
        string response_body;
        string response_header;
        if (url == "/linuxPage") {
            ifstream file("linuxPage.html");
            response_body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        } else if (url == "/hi") {
            ifstream file("hi.html");
            response_body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        } else if (url == "/api") {
            response_body = "[{\"id\":1,\"title\":\"First Post\",\"content\":\"Hello, this is the first post.\"},{\"id\":2,\"title\":\"Second Post\",\"content\":\"Hello, this is the second post.\"},{\"id\":3,\"title\":\"Third Post\",\"content\":\"Hello, this is the third post.\"}]";
            response_header = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
        } else {
            response_body = "404 Not Found";
            response_header = "HTTP/1.1 404 Not Found\r\n\r\n";
        }

        // 클라이언트에 응답 보내기
        string response = response_header + response_body;
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