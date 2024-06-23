#define _CRT_SECURE_NO_WARNINGS  // ���� ��� ��Ȱ��ȭ (C ǥ�� ���̺귯�� �Լ� ��� ��)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 100  // �ִ� ���ڿ� ���� ����

// Ű ����
typedef int Key;
enum Key_ {
    Key_none = 0,
    Key_up = 72,
    Key_down = 80,
    Key_left = 75,
    Key_right = 77,
    Key_enter = 13,
};

// �޴� �׸� ����ü
typedef struct MenuItem {
    int code;                       //�޴� �׸� �ڵ�
    char name[MAX_LINE_LENGTH];     //�޴� �׸� �̸�
    int price;                      //�޴� �׸� ����
    struct MenuItem* next;          //���� �޴� �׸��� ����Ű�� ������
} MenuItem;

// �ֹ� �׸� ����ü
typedef struct Order {
    MenuItem* menuItem;
    struct Order* next;
} Order;

// ���� Ÿ�� ������
enum ColorType {
    BLACK, darkBLUE, DarkGreen, darkSkyBlue, DarkRed, DarkPurple, DarkYellow,
    GRAY, DarkGray, BLUE, GREEN, SkyBlue, RED, PURPLE, YELLOW, WHITE
} COLOR;

// Ŀ�� Ÿ�� ������
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

// Ŀ�� ���� �Լ�
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

// �ܼ� Ŀ�� ��ġ ���� �Լ�
bool gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
    return true;
}

// �ؽ�Ʈ ���� �Լ�
void Text_select(const char* _Format, bool selected, int x, int y) {
    if (gotoxy(x, y)) {
        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK);
        printf(_Format);
        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE);
    }
}

// �޴� �׸� �߰� �Լ�
MenuItem* addMenuItem(MenuItem* head, int code, const char* name, int price) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));    //�� �޴� �׸��� ���� �޸� �Ҵ�
    newItem->code = code;                                       //�� �׸��� �̸� ����
    strcpy(newItem->name, name);                                //�� �׸��� ���� ����
    newItem->price = price;                                     //�� �׸��� ���� ����
    newItem->next = NULL;                                       //�� �׸��� ���� �׸� �����͸� NULL�� �ʱ�ȭ

    //���� ����Ʈ�� ���ĵ� ��ġ�� �� �׸� ����
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

// Ű �Է� �Լ�
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

// �޴� ���� �ε� �Լ�
MenuItem* loadMenu(const char* filename, MenuItem* sections[]) {
    FILE* file = fopen(filename, "r"); //������ �б� ���� ����
    if (!file) {
        perror("������ �� �� �����ϴ�.");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int section = 0;

    // ������ �� �پ� �о�ͼ� �� ���ǿ� �°� �޴� �׸��� �߰�
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "1. �޴�")) {
            section = 1;
            continue;
        }
        else if (strstr(line, "2. �� ���� ����")) {
            section = 2;
            continue;
        }
        else if (strstr(line, "3. �� ���� ����")) {
            section = 3;
            continue;
        }
        else if (strstr(line, "4. ġ�� ���� ����")) {
            section = 4;
            continue;
        }
        else if (strstr(line, "5. ��� �߰�")) {
            section = 5;
            continue;
        }
        else if (strstr(line, "6. �� ���� ����")) {
            section = 6;
            continue;
        }
        else if (strstr(line, "7. ��ä ����")) {
            section = 7;
            continue;
        }
        else if (strstr(line, "8. �ҽ� ����")) {
            section = 8;
            continue;
        }
        else if (strstr(line, "9. ��Ʈ ����")) {
            section = 9;
            continue;
        }

        // �޴� �׸��� �Ľ��Ͽ� �� ���ǿ� �߰�
        char name[MAX_LINE_LENGTH];
        int code, price;
        price = 0;  // ������ ���� ��츦 ���� �ʱ�ȭ
        if (sscanf(line, "%d\t%[^\t\n]%d��", &code, name, &price) == 3 || sscanf(line, "%d\t%[^\t\n]", &code, name) == 2) {
            if (section >= 1 && section <= 10) {
                sections[section - 1] = addMenuItem(sections[section - 1], code, name, price);
            }
        }
    }

    fclose(file);
    return NULL;
}

// ���� �޴� ��� �Լ�
void printSectionMenu(MenuItem* head, const char* title) {
    printf("\n%s\n", title);
    while (head) {
        printf("%d: %s (����: %d��)\n", head->code, head->name, head->price);
        head = head->next;
    }
}

// �޴� �׸� �˻� �Լ�
MenuItem* findMenuItem(MenuItem* head, int code) {
    while (head) {
        if (head->code == code) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

// �ֹ� �׸� �߰� �Լ�
Order* addOrderItem(Order* head, MenuItem* menuItem) {
    Order* newOrder = (Order*)malloc(sizeof(Order));    //�� �ֹ� �׸��� ���� �޸� �Ҵ�
    newOrder->menuItem = menuItem;                      //�� �׸��� �޴� �׸� ������ ����
    newOrder->next = NULL;                              //�� �׸��� ���� �׸� �����͸� NULL�� �ʱ�ȭ

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

// �ֹ� �׸� ���� �Լ�
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

// �� ���� ��� �Լ�
int calculateTotalPrice(Order* order, MenuItem* menuSection) {
    int total = 0;
    bool is30cmSelected = false;
    bool isMultiplierApplied = false; // �ߺ� ������ ���� �÷���

    // "30cm" ���� ���� Ȯ��
    Order* tempOrder = order;
    while (tempOrder) {
        if (strcmp(tempOrder->menuItem->name, "30cm") == 0) {
            is30cmSelected = true;
            break;
        }
        tempOrder = tempOrder->next;
    }

    // ���� ���
    while (order) {
        int price = order->menuItem->price;

        // '1. �޴�' ������ �׸����� Ȯ�� �� �� ��� ���� ���� Ȯ��
        if (is30cmSelected && !isMultiplierApplied) {
            MenuItem* currentItem = menuSection;
            while (currentItem) {
                if (currentItem->code == order->menuItem->code) {
                    price *= 2;
                    isMultiplierApplied = true; // �� ���� �� ��� �����ϵ��� �÷��� ����
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



// �ֹ� ���� ��� �Լ�
void printOrder(Order* order, MenuItem* menuSection) {
    gotoxy(36, 0);
    printf("|   ====== �ֹ� ���� ======\n");
    Order* headOrder = order;
    int y = 0;
    while (order) {
        gotoxy(36, ++y);
        printf("|    - %s (�ڵ�: %d, ����: %d��)\n", order->menuItem->name, order->menuItem->code, order->menuItem->price);
        order = order->next;
    }
    int totalPrice = calculateTotalPrice(headOrder, menuSection);

    gotoxy(36, ++y);
    printf("|   ============ �� �ݾ� : % d��\n", totalPrice);
}

// �ֹ� �׸� �޸� ���� �Լ�
void freeOrders(Order* order) {
    while (order) {
        Order* temp = order;
        order = order->next;
        free(temp);
    }
}

// �޴� ȭ�� ǥ�� �Լ� ����
void showMenu(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order);
void showModifyMenu(Order** order);

// ���� ȭ�� ǥ�� �Լ�
void showEntrance(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order) {
    int itemIndex = 0;
    Key key;

    while (1) {
        system("cls");
        Text_select("������� Ű����ũ", false, 0, 0);      //���� �ؽ�Ʈ ���
        Text_select("1. �ֹ��ϱ�", itemIndex == 0, 0, 1);   //�޴� �׸� ���ý� �ش� �ؽ�Ʈ ���
        Text_select("2. �����ϱ�", itemIndex == 1, 0, 2);   //�޴� �׸� ���ý� �ش� �ؽ�Ʈ ���

        key = input_key();                                  //Ű �Է¹ޱ�
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + 2) % 2;            //�� ����Ű �Է� �� �ε��� ����
        }
        else if (key == Key_down) {                         //�Ʒ� ����Ű �Է� �� �ε��� ����
            itemIndex = (itemIndex + 1) % 2;
        }
        else if (key == Key_enter) {
            if (itemIndex == 0) {
                showMenu(sections, sectionTitles, sectionCount, order);     //�ֹ��ϱ� ���� �� �޴� ȭ�� ǥ��
                return;
            }
            else if (itemIndex == 1) {
                exit(0);                                    //�����ϱ� ���� �� ���α׷� ���� 
            }
        }
    }
}

// �޴� ���� ȭ�� ǥ�� �Լ�
void showMenu(MenuItem* sections[], const char* sectionTitles[], int sectionCount, Order** order) {
    int sectionIndex = 0;
    Key key;

    while (1) {
        system("cls");                                                  //ȭ�� �����
        for (int i = 0; i < sectionCount; ++i) {
            Text_select(sectionTitles[i], i == sectionIndex, 0, i);
        }
        printOrder(*order, sections[0]);                                //�ֹ� ���� ���

        key = input_key();                                              //Ű �Է� �ޱ�
        if (key == Key_up) {
            sectionIndex = (sectionIndex - 1 + sectionCount) % sectionCount;    // �� ����Ű �Է� �� ���� �ε��� ����
        }
        else if (key == Key_down) {
            sectionIndex = (sectionIndex + 1) % sectionCount;                   //�Ʒ� ����Ű �Է½� ���� �ε��� ����
        }
        else if (key == Key_enter) {
            if (sectionIndex == sectionCount - 1) {
                showModifyMenu(order);                                          //�����ϱ� ���� �� �ֹ� ���� ȭ�� ǥ��
            }
            else if (sectionIndex == sectionCount - 2) {
                system("cls");                                                  //ȭ�� �����
                printOrder(*order, sections[0]);                                //�ֹ� ���� ���
                printf("\n�ֹ��� �Ϸ�Ǿ����ϴ�. �����մϴ�!\n");               //�ֹ� �Ϸ� �޽��� ���

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

    while (1) {                                                                 //ȭ�� �����
        system("cls");
        printf("%s\n", sectionTitles[sectionIndex]);                            //���� Ÿ��Ʋ ���
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
                    printf("%d��", item->price);
                }
                if (i == itemIndex) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE);
            }
        }
        Text_select("[�ڷΰ���]", i == itemIndex, 0, i + 1);                    //�ڷΰ��� �ɼ� ���

        printOrder(*order, sections[0]);                                        //�ֹ� ���� ���

        key = input_key();                                                      //Ű �Է� �ޱ�
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + i + 1) % (i + 1);                      //�� ����Ű �Է� �� �׸� �ε��� ����
        }
        else if (key == Key_down) {
            itemIndex = (itemIndex + 1) % (i + 1);                              //�Ʒ� ����Ű �Է� �� �׸� �ε��� ����
        }
        else if (key == Key_enter) {
            if (itemIndex == i) {
                showMenu(sections, sectionTitles, sectionCount, order);         //�ڷ� ���� ���ý� �޴� ȭ�� ǥ��
                return;
            }
            break;
        }
    }

    currentItem = currentSection;
    for (int i = 0; i < itemIndex; ++i) {
        currentItem = currentItem->next;
    }

    *order = addOrderItem(*order, currentItem);                                 //�ֹ� �׸� �߰�

    gotoxy(0, 0);
    showMenu(sections, sectionTitles, sectionCount, order);
}


// �ֹ� ���� ȭ�� ǥ�� �Լ�
void showModifyMenu(Order** order) {
    int itemIndex = 0;
    Key key;

    while (1) {
        system("cls");                                                         //ȭ�� �����
        printf("������ �׸��� �������ּ���:\n");
        int i = 0;
        for (Order* ord = *order; ord != NULL; ord = ord->next, ++i) {
            Text_select(ord->menuItem->name, i == itemIndex, 0, i + 1);
        }
        Text_select("[�ڷΰ���]", i == itemIndex, 0, i + 1);

        key = input_key();
        if (key == Key_up) {
            itemIndex = (itemIndex - 1 + i + 1) % (i + 1);                      //�� ����Ű �Է½� �ε��� ����
        }
        else if (key == Key_down) {
            itemIndex = (itemIndex + 1) % (i + 1);                              //�Ʒ� ����Ű �Է� �� �ε��� ����
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
                *order = removeOrderItem(*order, current->menuItem);            //�ֹ� �׸� ����
                break;
            }
        }
    }
    gotoxy(0, 0);
    showModifyMenu(order);
}

// ���� �Լ�
int main() {
    //�ʱ� ���� 
    setcursortype(NOCURSOR);                // Ŀ�� ��Ȱ��ȭ
    MenuItem* sections[10] = { NULL };      //�� ������ �޴� �׸��� ������ ������ �迭
    Order* order = NULL;                    //�ֹ� �׸��� ������ ������
    const char* sectionTitles[] = {
        "1. �޴�",
        "2. �� ���� ����",
        "3. �� ���� ����",
        "4. ġ�� ���� ����",
        "5. ��� �߰�",
        "6. �� ���� ����",
        "7. ��ä ����",
        "8. �ҽ� ����",
        "9. ��Ʈ ����",
        "[���� �Ϸ�]",
        "[���� �ϱ�]"
    };
    int sectionCount = sizeof(sectionTitles) / sizeof(sectionTitles[0]);


    loadMenu("Subwaydata.txt", sections);                               //�޴� ������ ���� �ε�
    showEntrance(sections, sectionTitles, sectionCount, &order);        //����ȭ�� ǥ��

    return 0;
}