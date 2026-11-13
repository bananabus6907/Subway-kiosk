# 🥪 Subway Kiosk Order System

서브웨이 키오스크 주문 시스템을 C 언어로 구현한 프로젝트입니다. 연결 리스트 자료구조와 Windows Console API를 활용하여 실제 키오스크와 유사한 사용자 경험을 제공합니다.

## 프로젝트 개요

실제 서브웨이 매장의 키오스크 주문 프로세스를 콘솔 환경에서 재현한 프로그램입니다. 메뉴 선택부터 주문 완료까지의 전 과정을 구현했습니다.

### 주요 기능

- **메뉴 시스템**
  - 섹션별 메뉴 분류 (빵, 메인, 치즈, 야채, 소스 등)
  - 계층적 메뉴 네비게이션
  - 메뉴 코드 및 가격 관리

- **주문 관리**
  - 실시간 주문 내역 표시
  - 메뉴 추가/제거
  - 총 금액 자동 계산

- **사용자 인터페이스**
  - 방향키 기반 메뉴 선택
  - 선택 항목 하이라이트
  - 직관적인 화면 레이아웃

## 기술 스택

### 언어 및 라이브러리
- **C (C11 표준)**
- **Windows API** (Console 제어)
- **conio.h** (키보드 입력)

### 자료구조

```c
// 메뉴 항목 (연결 리스트)
typedef struct MenuItem {
    int code;                    // 메뉴 코드
    char name[100];              // 메뉴 이름
    int price;                   // 가격
    struct MenuItem* next;       // 다음 항목
} MenuItem;

// 주문 항목 (연결 리스트)
typedef struct Order {
    MenuItem* menuItem;          // 선택한 메뉴
    struct Order* next;          // 다음 주문
} Order;
```

## 파일 구조

```
Subway-kiosk/
├── 오픈소스 기말 프로젝트.c   # 메인 소스 코드 (527줄)
├── Subwaydata.txt            # 메뉴 데이터
└── README.md
```

## 핵심 기능 구현

### 1. 연결 리스트 (Linked List)

```c
// 메뉴 추가 (정렬된 삽입)
MenuItem* addMenuItem(MenuItem* head, int code, 
                      const char* name, int price) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->code = code;
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->next = NULL;
    
    // 정렬된 위치에 삽입
    if (head == NULL || head->code > code) {
        newItem->next = head;
        return newItem;
    }
    
    MenuItem* current = head;
    while (current->next && current->next->code < code) {
        current = current->next;
    }
    newItem->next = current->next;
    current->next = newItem;
    
    return head;
}
```

### 2. 주문 시스템

```c
// 주문 추가
void addOrder(Order** headOrder, MenuItem* menuItem) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->menuItem = menuItem;
    newOrder->next = *headOrder;
    *headOrder = newOrder;
}

// 총 금액 계산
int calculateTotalPrice(Order* order, int section) {
    int total = 0;
    while (order) {
        total += order->menuItem->price;
        order = order->next;
    }
    return total;
}
```

### 3. 콘솔 UI

```c
// 텍스트 선택 하이라이트
void Text_select(const char* text, bool selected, int x, int y) {
    gotoxy(x, y);
    if (selected) {
        // 흰색 배경 + 검은색 글자
        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            (WHITE << 4) + BLACK
        );
    }
    printf("%s", text);
    if (selected) {
        // 원래대로 복구
        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            (BLACK << 4) + WHITE
        );
    }
}
```

## 빌드 및 실행

### Visual Studio

```cmd
1. Visual Studio 실행
2. 새 프로젝트 → C++ 콘솔 앱
3. "오픈소스 기말 프로젝트.c" 추가
4. Subwaydata.txt를 프로젝트 폴더에 복사
5. 빌드 및 실행 (F5)
```

### 명령줄 컴파일

```cmd
# MSVC
cl "오픈소스 기말 프로젝트.c"

# 실행
"오픈소스 기말 프로젝트.exe"
```

## 사용 방법

### 1. 프로그램 시작

```
서브웨이 키오스크
> 1. 주문하기
  2. 종료하기
```

### 2. 메뉴 선택

- **방향키 (↑/↓)**: 메뉴 항목 이동
- **Enter**: 선택
- **메뉴 구성**:
  1. 빵 선택
  2. 메인 재료 선택
  3. 치즈 선택
  4. 야채 선택
  5. 소스 선택
  6. 사이드 메뉴

### 3. 주문 확인

- 선택한 메뉴 목록 표시
- 각 항목의 코드 및 가격
- 총 금액 자동 계산

### 4. 주문 수정

- 메뉴 추가
- 메뉴 삭제
- 주문 완료

## 데이터 형식

### Subwaydata.txt

```
메뉴코드,메뉴이름,가격
101,허니오트,4500
102,화이트,4500
201,에그마요,5500
202,참치,6000
...
```

## 주요 함수

| 함수명 | 설명 |
|--------|------|
| `addMenuItem()` | 메뉴 항목 추가 (정렬) |
| `addOrder()` | 주문에 메뉴 추가 |
| `calculateTotalPrice()` | 총 금액 계산 |
| `showMenu()` | 메뉴 화면 표시 |
| `showEntrance()` | 시작 화면 |
| `Text_select()` | 선택 하이라이트 |
| `gotoxy()` | 커서 위치 이동 |

## 학습 내용

### 1. 자료구조
- **연결 리스트 (Linked List)**
  - 동적 메모리 할당
  - 노드 삽입/삭제
  - 정렬된 삽입
- **포인터 체이닝**
  - 이중 포인터 활용
  - 메모리 관리

### 2. 시스템 프로그래밍
- **Windows Console API**
  - 커서 제어
  - 색상 제어
  - 화면 지우기
- **파일 I/O**
  - CSV 파싱
  - 데이터 로드

### 3. UI/UX 설계
- **키보드 인터페이스**
  - 방향키 처리
  - 이벤트 루프
- **시각적 피드백**
  - 선택 하이라이트
  - 메뉴 구조화

## 기술적 특징

### 동적 메모리 관리

```c
// 메뉴 리스트 생성
MenuItem* menu = NULL;
menu = addMenuItem(menu, 101, "허니오트", 4500);

// 주문 리스트 관리
Order* order = NULL;
addOrder(&order, selectedMenu);

// 메모리 해제
freeOrders(order);
```

### 정렬된 삽입 알고리즘

메뉴 코드 순서대로 자동 정렬하여 삽입

### 계층적 메뉴 시스템

섹션별로 메뉴를 분류하여 관리

## 개선 가능 사항

- [ ] 메뉴 수량 선택 기능
- [ ] 할인/쿠폰 시스템
- [ ] 영수증 출력
- [ ] 다국어 지원
- [ ] GUI 버전 개발

## 프로젝트 의의

실제 서비스의 키오스크 시스템을 모방하여:
- 자료구조의 실용적 활용 학습
- 사용자 인터페이스 설계 경험
- 완성도 있는 프로그램 개발 경험

## 라이선스

Educational Project - 오픈소스 소프트웨어 기말 프로젝트

## 작성자

이승준
