#include<stdio.h>
#include<windows.h>

typedef struct{
  char buffer[1024];
  int bufferSize;
}ENUM_WINDOW_PROC_PARAMETERS;
// define callback
BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lparam){ 
    //just do a simple retrival of the application title name
    //zerobuffer
  memset(((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->buffer,0,((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->bufferSize);
  int numRet = GetWindowTextA(hwnd, ((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->buffer,
                 ((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->bufferSize);
  if (numRet < ((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->bufferSize) {
    ((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->buffer[(numRet + 1)] = '\0';
  }

  DWORD processID = 0;
  GetWindowThreadProcessId(hwnd, &processID);
  
  printf("Process Name: %s - Process ID: %d\n",
         ((ENUM_WINDOW_PROC_PARAMETERS*)lparam)->buffer, processID);
}

int main() { 
  SECURITY_ATTRIBUTES security_attributes_handle = {
      .nLength = sizeof(SECURITY_ATTRIBUTES),
      .lpSecurityDescriptor = NULL,
      .bInheritHandle = FALSE
  };

  SECURITY_ATTRIBUTES security_attributes_thread = {
      .nLength = sizeof(SECURITY_ATTRIBUTES),
      .lpSecurityDescriptor = NULL,
      .bInheritHandle = FALSE
  };
  DWORD processCreationFlags = NORMAL_PRIORITY_CLASS;

  DWORD _dwFlags = STARTF_USESHOWWINDOW;

  STARTUPINFOA startupInfo = {
      .cb = sizeof(struct _STARTUPINFOA),
      .lpReserved = NULL,
      .lpDesktop = "Winsta0\\Default",
      .lpTitle = NULL,
      .dwX = 0,
      .dwY = 0,
      .dwXSize = 0,
      .dwXCountChars = 0,
      .dwYCountChars = 0,
      .dwFillAttribute = 0,
      .dwFlags = _dwFlags,
      .wShowWindow = SW_HIDE,//SW_SHOWNORMAL,
      .cbReserved2 = 0,
      .lpReserved2 = NULL,
      .hStdInput = stdin,
      .hStdOutput = stdout,
      .hStdError = stderr
  };


  PROCESS_INFORMATION processInformation = {0};

        BOOL retResult = CreateProcessA(
      "C:\\Users\\benja\\Desktop\\sysinternals\\winobj.exe", NULL,
      &security_attributes_handle, &security_attributes_thread, FALSE, processCreationFlags,NULL,
      NULL, &startupInfo, &processInformation);
        //C:\\Program Files\\WindowsApps\\Microsoft.WindowsNotepad_11.2607.14.0_x64__8wekyb3d8bbwe\\Notepad\\Notepad.exe
        //C:\\Users\benja\\Desktop\\sysinternals\\winobj.exe
        printf("Value of createprocess is: %d\n", retResult);
        if (retResult != 0) {
          printf("The process ID of the process is: %d\n",
                 processInformation.dwProcessId);
          //Lets acutually eumerate the top level windows
          ENUM_WINDOW_PROC_PARAMETERS params = {.buffer = {0},
                                                .bufferSize = 1024};
          EnumWindows(EnumWindowProc, &params);


          WaitForSingleObject(processInformation.hProcess, INFINITE);
        } else {
            //retResult is 0
          DWORD lastError = GetLastError();
            WCHAR errorBuffer[2024];
          printf("Number of stuff written to Buffer: %d\n",FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError,
                        LANG_SYSTEM_DEFAULT,
                            errorBuffer,(DWORD)sizeof(errorBuffer),NULL));
            wprintf(L"Formatted message: %s\n", errorBuffer);

        }
   }