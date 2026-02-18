# IJKL_to_ARROWs

<p align="center">
  <img width="300" alt="image" align="center" src="https://github.com/user-attachments/assets/9720beb8-62b9-4a7e-b2a5-ae492ae42fda" />
</p>

윈도에서 노트북의 작은 화살표 키 때문에 고통받는 게이머들을 위한 리매핑 프로그램이다.
IJKL 키 입력을 로우레벨에서 상하좌우 화살표 키 입력으로 바꿔준다.

This is a low-level remapping program for gamer struggling with small arrow keys on Windows laptops.
It remaps IJKL keys to the arrow keys.

---

### Features (주요 기능)

- **Low-Level Control**

  Windows API(SetWindowsHookEx)를 사용하여 게임 내에서도 입력이 잘 작동함

  Uses Windows API (SetWindowsHookEx) to ensure inputs work even inside games.

- **Lightweight**

  실행 중일 때만 동작하며, 프로그램을 종료하면 즉시 원래 키로 돌아옴

  Only active while running; original key functions are restored immediately upon exit.

- **Tray Icon**

  백그라운드에서 실행되며 트레이 아이콘 우클릭을 통해 간편하게 종료할 수 있음

  Runs in the background with a tray icon for easy exit via right-click.

---

### Build Instructions (빌드 방법; MSYS2)
```Bash
# 1. 리소스 컴파일 (Compile Resources)
windres resource.rc -O coff -o resource.res

# 2. 본체 빌드 (Build Executable)
g++ -O2 IJKL_2_ARROWS.cpp resource.res -o _IJKL_2_ARROWS.exe -mwindows -static -luser32 -lshell32
```

빌드하기 귀찮으면 Releases 코너에 가서 받으면 된다.

If you don't want to build it yourself, simply grab it from the Releases section.

---

### Usage & Notes (사용 및 주의사항)

- **Admin Privilege**

  게임이 관리자 권한으로 실행 중이라면, 이 프로그램도 관리자 권한으로 실행해야 함

  If your game runs with administrator privileges, you must run this program as an Administrator as well.

- **Key mapping**

  | Key | Mapped To |
  | :---: | :---: |
  | **I** | **Up Arrow** (↑) |
  | **J** | **Left Arrow** (←) |
  | **K** | **Down Arrow** (↓) |
  | **L** | **Right Arrow** (→) |

- **Exit**

  트레이 아이콘을 우클릭하고 '종료'를 누르면 프로그램이 닫히며 리매핑이 해제됨

  Right-click the tray icon and select 'Exit' to close the program and disable remapping.

---

### License

IDK WTFPL

---

### Author

Daanta https://github.com/daanta-real
