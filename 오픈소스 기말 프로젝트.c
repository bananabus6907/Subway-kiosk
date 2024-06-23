#define _CRT_SECURE_NO_WARNINGS  // 보안 경고 비활성화 (C 표준 라이브러리 함수 사용 시)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 100  // 최대 문자열 길이 정의

// 키 정의
typedef int Key;
enum Key_ {
    Key_none = 0,
    Key_up = 72,
    Key_down = 80,
    Key_left = 75,
    Key_right = 77,
    Key_enter = 13,
};

// 메뉴 항목 구조체
typedef struct MenuItem {
    int code;                       //메뉴 항목 코드
    char name[MAX_LINE_LENGTH];     //메뉴 항목 이름
    int price;                      //메뉴 항목 가격
    struct MenuItem* next;          //다음 메뉴 항목을 가리키는 포인터
} MenuItem;

// 주문 항목 구조체
typedef struct Order {
    MenuItem* menuItem;
    struct Order* next;
} Order;

// 색상 타입 열거형
enum ColorType {
    BLACK, darkBLUE, DarkGreen, darkSkyBlue, DarkRed, DarkPurple, DarkYellow,
    GRAY, DarkGray, BLUE, GREEN, SkyBlue, RED, PURPLE, YELLOW, WHITE
} COLOR;

// 커서 타입 열거형
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

// 커서 설정 함수
void setcursortype(CURSOR_TYPE c) {
    CONSOLE_CURSOR_INFO CurInfo;
    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

// 콘솔 커서 위치 설정 함수
bool gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
    return true;
}

// 텍스트 선택 함수
void Text_select(const char* _Format, bool selected, int x, int y) {
    if (gotoxy(x, y)) {
        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK);
        printf(_Format);
        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE);
    }
}

// 메뉴 항목 추가 함수
MenuItem* addMenuItem(MenuItem* head, int code, const char* name, int price) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));    //새 메뉴 항목을 위한 메모리 할당
    newItem->code = code;                                       //새 항목의 이름 설정
    strcpy(newItem->name, name);                                //새 항목의 가격 설정
    newItem->price = price;                                     //새 항목의 가격 설정
    newItem->next = NULL;                                       //새 항목의 다음 항목 포인터를 NULL로 초기화

    //연결 리스트의 정렬된 위치에 새 항목 삽입
    if (head == NULL || head->code > newItem->code) {
        newItem->next = head;
        return newItem;
    }

    MenuItem* current = head;
    while (current->next != NULL && current->next->code < newItem->code) {
        current = current->next;
    }

    newItem->next = current->next;
    current->next = newItem;
    return head;
}

// 키 입력 함수
Key input_key() {
    Key key = Key_none;
    char ch = _getch();

    if (ch == -32) {
        ch = _getch();
        key = ch;
    }
    else {
        if (ch == '\r' || ch == '\n') {
            key = Key_enter;
        }
    }

    return key;
}

// 메뉴 파일 로드 함수
MenuItem* loadMenu(const char* filename, MenuItem* sections[]) {
    FILE* file = fopen(filename, "r"); //파일을 읽기 모드로 열기
    if (!file) {
        perror("파일을 열 수 없습니다.");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int section = 0;

    // 파일을 한 줄씩 읽어와서 각 섹션에 맞게 메뉴 항목을 추가
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "1. 메뉴")) {
            section = 1;
            continue;
        }
        else if (strstr(line, "2. 빵 길이 선택")) {
            section = 2;
            continue;
        }
        else if (strstr(line, "3. 빵 종류 선택")) {
            section = 3;
            continue;
        }
        else if (strstr(line, "4. 치즈 종류 선택")) {
            section = 4;
            continue;
        }
        else if (strstr(line, "5. 재료 추가")) {
            section = 5;
            continue;
        }
        else if (strstr(line, "6. 빵 굽기 유무")) {
            section = 6;
            continue;
        }
        else if (strstr(line, "7. 야채 선택")) {
            section = 7;
            continue;
        }
        else if (strstr(line, "8. 소스 선택")) {
            section = 8;
            continue;
        }
        else if (strstr(line, "9. 세트 유무")) {
            section = 9;
            continue;
        }

        // 메뉴 항목을 파싱하여 각 섹션에 추가
        char name[MAX_LINE_LENGTH];
        int code, price;
        price = 0;  // 가격이 없는 경우를 위한 초기화
        if (sscanf(line, "%d\t%[^\t\n]%d원", &code, name, &price) == 3 || sscanf(line, "%d\t%[^\t\n]", &code, name) == 2) {
            if (section >= 1 && section <= 10) {
                sections[section - 1] = addMenuItem(sections[section - 1], code, name, price);
            }
        }
    }

    fclose(file);
    return NULL;
}

// 섹션 메뉴 출력 함수
void printSectionMenu(MenuItem* head, const char* title) {
    printf("\n%s\n", title);
    while (head) {
        printf("%d: %s (가격: %d원)\n", head->code, head->name, head->price);
        head = head->next;
    }
}

// 메뉴 항목 검색 함수
MenuItem* findMenuItem(MenuItem* head, int code) {
    while (head) {
        if (head->code == code) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

// 주문 항목 추가 함수
Order* addOrderItem(Order* head, MenuItem* menuItem) {
    Order* newOrder = (Order*)malloc(sizeof(Order));    //새 주문 항목을 위한 메모리 할당
    newOrder->menuItem = menuItem;                      //새 항목의 메뉴 항목 포인터 설정
    newOrder->next = NULL;                              //새 항목의 다음 항목 포인터를 NULL로 초기화

    if (head == NULL) {
        return newOrder;
    }

    Order* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newOrder;
    return head;
}

// 주문 항목 제거 함수
Order* removeOrderItem(Order* head, MenuItem* menuItem) {
    if (head == NULL) return NULL;

    Order* current = head;
    Order* prev = NULL;

    while (current != NULL && current->menuItem != menuItem) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return head;

    if (prev == NULL) {
        head = current->next;
    }
    else {
        prev->next = current->next;
    }

    free(current);
    return head;
}

// 총 가격 계산 함수
int calculateTotalPrice(Order* order, MenuItem* menuSection) {
    int total = 0;
    bool is30cmSelected = false;
    bool isMultiplierApplied = false; // 중복 방지를 위한 플래그

    // "30cm" 선택 여부 확인
    Order* tempOrder = order;
    while (tempOrder) {
        if (strcmp(tempOrder->menuItem->name, "30cm") == 0) {
            is30cmSelected = true;
            break;
        }
        tempOrder = tempOrder->next;
    }

    // 가격 계산
    while (order) {
        int price = order->menuItem->price;

        // '1. 메뉴' 섹션의 항목인지 확인 및 두 배로 증가 여부 확인
        if (is30cmSelected && !isMultiplierApplied) {
            MenuItem* currentItem = menuSection;
            while (currentItem) {
                if (currentItem->code == order->menuItem->code) {
                    price *= 2;
                    isMultiplierApplied = true; // 한 번만 두 배로 증가하도록 플래그 설정
                    break;
                }
                currentItem = currentItem->next;
            }
        }

        total += price;
        order = order->next;
    }

    return total;
}



// 주문 내역 출력 함수
void printOrder(Order* order, MenuItem* menuSection) {
    gotoxy(36, 0);
    printf("|   ====== 주문 내역 ======\n");
    Order* headOrder = order;
    int y = 0;
    while (order) {
        gotoxy(36, ++y);
        printf("|    - %s (코드: %d, 가격: %d원)\n", order->menuItem->name, order->menuItem->code, order->menuItem->price);
        order = order->next;
    }
    int totalPrice = calculateTotalPrice(headOrder, menuSection);

    gotoxy(36, ++y);
    printf("|   ============ 총 금액 : % d원\n", totalPrice);
}

// 주문 항목 메모리 해제 함수
void freeOrders(Order* order) {
    while (order) {
        Order* temp = order;
        order = order->next;
        free(temp);
    }
}

// 메뉴 화면 표시 함수 선언
void showMenu(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order);
void showModifyMenu(Order** order);

// 시작 화면 표시 함수
void showEntrance(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order) {
    int itemIndex = 0;
    Key key;

    while (1) {
        system("cls");
        Text_select("서브웨이 키오스크", false, 0, 0);      //제목 텍스트 출력
        Text_select("1. 주문하기", itemIndex == 0, 0, 1);   //메뉴 항목 선택시 해당 텍스트 출력
        Text_select("2. 종료하기", itemIndex == 1, 0, 2);   //메뉴 항목 선택시 해당 텍스트 출력

        key = input_key();                                  //키 입력받기
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + 2) % 2;            //위 방향키 입력 시 인덱스 변경
        }
        else if (key == Key_down) {                         //아래 방향키 입력 시 인덱스 변경
            itemIndex = (itemIndex + 1) % 2;
        }
        else if (key == Key_enter) {
            if (itemIndex == 0) {
                showMenu(sections, sectionTitles, sectionCount, order);     //주문하기 선택 시 메뉴 화면 표시
                return;
            }
            else if (itemIndex == 1) {
                exit(0);                                    //종료하기 선택 시 프로그램 종료 
            }
        }
    }
}

// 메뉴 선택 화면 표시 함수
void showMenu(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order) {
    int sectionIndex = 0;
    Key key;

    while (1) {
        system("cls");                                                  //화면 지우기
        for (int i = 0; i < sectionCount; ++i) {
            Text_select(sectionTitles[i], i == sectionIndex, 0, i);
        }
        printOrder(*order, sections[0]);                                //주문 내역 출력

        key = input_key();                                              //키 입력 받기
        if (key == Key_up) {
            sectionIndex = (sectionIndex - 1 + sectionCount) % sectionCount;    // 위 방향키 입력 시 섹션 인덱스 변경
        }
        else if (key == Key_down) {
            sectionIndex = (sectionIndex + 1) % sectionCount;                   //아래 방향키 입력시 섹션 인덱스 변경
        }
        else if (key == Key_enter) {
            if (sectionIndex == sectionCount - 1) {
                showModifyMenu(order);                                          //수정하기 선택 시 주문 수정 화면 표시
            }
            else if (sectionIndex == sectionCount - 2) {
                system("cls");                                                  //화면 지우기
                printOrder(*order, sections[0]);                                //주문 내역 출력
                printf("\n주문이 완료되었습니다. 감사합니다!\n");               //주문 완료 메시지 출력

                freeOrders(*order);
                *order = NULL;
                system("pause");
                return;
            }
            else break;
        }
    }

    MenuItem* currentSection = sections[sectionIndex];
    MenuItem* currentItem = currentSection;
    int itemIndex = 0;

    while (1) {                                                                 //화면 지우기
        system("cls");
        printf("%s\n", sectionTitles[sectionIndex]);                            //섹션 타이틀 출력
        int i = 0;
        for (MenuItem* item = currentSection; item != NULL; item = item->next, ++i) {
            if (gotoxy(0, i + 1)) {
                if (i == itemIndex) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK);
                printf("%s", item->name);
                if (item->price > 0) {
                    int n = item->price;
                    int count = 0;
                    while (n != 0) {
                        n = n / 10;
                        ++count;
                    }
                    gotoxy(32 - count, i + 1);
                    printf("%d원", item->price);
                }
                if (i == itemIndex) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE);
            }
        }
        Text_select("[뒤로가기]", i == itemIndex, 0, i + 1);                    //뒤로가기 옵션 출력

        printOrder(*order, sections[0]);                                        //주문 내역 출력

        key = input_key();                                                      //키 입력 받기
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + i + 1) % (i + 1);                      //위 방향키 입력 시 항목 인덱스 변경
        }
        else if (key == Key_down) {
            itemIndex = (itemIndex + 1) % (i + 1);                              //아래 방향키 입력 시 항목 인덱스 변경
        }
        else if (key == Key_enter) {
            if (itemIndex == i) {
                showMenu(sections, sectionTitles, sectionCount, order);         //뒤로 가기 선택시 메뉴 화면 표시
                return;
            }
            break;
        }
    }

    currentItem = currentSection;
    for (int i = 0; i < itemIndex; ++i) {
        currentItem = currentItem->next;
    }

    *order = addOrderItem(*order, currentItem);                                 //주문 항목 추가

    gotoxy(0, 0);
    showMenu(sections, sectionTitles, sectionCount, order);
}


// 주문 수정 화면 표시 함수
void showModifyMenu(Order** order) {
    int itemIndex = 0;
    Key key;

    while (1) {
        system("cls");                                                         //화면 지우기
        printf("수정할 항목을 선택해주세요:\n");
        int i = 0;
        for (Order* ord = *order; ord != NULL; ord = ord->next, ++i) {
            Text_select(ord->menuItem->name, i == itemIndex, 0, i + 1);
        }
        Text_select("[뒤로가기]", i == itemIndex, 0, i + 1);

        key = input_key();
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + i + 1) % (i + 1);                      //위 방향키 입력시 인덱스 변경
        }
        else if (key == Key_down) {
            itemIndex = (itemIndex + 1) % (i + 1);                              //아래 방향키 입력 시 인덱스 변경
        }
        else if (key == Key_enter) {
            if (itemIndex == i) {
                return;
            }
            else {
                Order* current = *order;
                for (int j = 0; j < itemIndex; ++j) {
                    current = current->next;
                }
                *order = removeOrderItem(*order, current->menuItem);            //주문 항목 제거
                break;
            }
        }
    }
    gotoxy(0, 0);
    showModifyMenu(order);
}

// 메인 함수
int main() {
    //초기 설정 
    setcursortype(NOCURSOR);                // 커서 비활성화
    MenuItem* sections[10] = { NULL };      //각 세션의 메뉴 항목을 저장할 포인터 배열
    Order* order = NULL;                    //주문 항목을 저장할 포인터
    const char* sectionTitles[] = {
        "1. 메뉴",
        "2. 빵 길이 선택",
        "3. 빵 종류 선택",
        "4. 치즈 종류 선택",
        "5. 재료 추가",
        "6. 빵 굽기 유무",
        "7. 야채 선택",
        "8. 소스 선택",
        "9. 세트 유무",
        "[선택 완료]",
        "[수정 하기]"
    };
    int sectionCount = sizeof(sectionTitles) / sizeof(sectionTitles[0]);


    loadMenu("Subwaydata.txt", sections);                               //메뉴 데이터 파일 로드
    showEntrance(sections, sectionTitles, sectionCount, &order);        //시작화면 표시

    return 0;
}