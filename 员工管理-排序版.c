#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SALESMAN,
    MANAGER,
    SALESMANAGER
} Position;

typedef struct {
    char no[20];
    char name[50];
    int age;
    Position position;
} Staff;


typedef struct {
    Staff staff;
    double sales;
} Salesman;

typedef struct {
    Staff staff;
} Manager;

typedef struct {
    Staff staff;
    double sales;
} SalesManager;

typedef struct Node {
    void* staff;
    int isEmployed;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int staffNum;
} ManagementSystem;

void initManagementSystem(ManagementSystem* system);
void displayAllStaff(Node* head, int numStaff);
void displayEmployeeCount(ManagementSystem* system);
Node* findStaffByNo(ManagementSystem* system, const char* no);
void hire(ManagementSystem* system, const char* no, const char* name, int age, Position position, double sales);
void markForTermination(ManagementSystem* system);
void modifyStaff(ManagementSystem* system);
void terminateStaff(ManagementSystem* system);
void recombineFile(ManagementSystem* system);
void selectFunction(ManagementSystem* system);
void quicksort(Staff** staffList, int left, int right);//快速排序
int partition(Staff** staffList, int left, int right);
void swap(Staff** a, Staff** b);

int main() {
    ManagementSystem ms;
    initManagementSystem(&ms);
    selectFunction(&ms);
    return 0;
}

void initManagementSystem(ManagementSystem* system) {
    system->head = NULL;
    system->staffNum = 0;//当前管理系统中雇佣的员工数量为零
}

void displayAllStaff(Node* head, int numStaff) {
    printf("所有员工信息：\n");
    Node* current = head;
    while (current != NULL) {
        if (current->isEmployed) {//雇佣状态
            Staff* currentStaff = (Staff*)current->staff;//访问当前员工的具体信息
            printf("编号: %s\n", currentStaff->no);
            printf("姓名: %s\n", currentStaff->name);
            printf("年龄: %d\n", currentStaff->age);
            printf("----------------------\n");
        }
        current = current->next;
    }
}

void displayEmployeeCount(ManagementSystem* system) {
     int totalEmployees = 0;
    int totalSalesmen = 0;
    int totalManagers = 0;
    int totalSalesManagers = 0;

    Node* current = system->head;
    while (current != NULL) {
        if (current->isEmployed) {
            totalEmployees++;

            if (current->staff != NULL) {
                void* staff = current->staff;
                if (staff != NULL) {
                    Staff* currentStaff = (Staff*)staff;
                    Position position = currentStaff->position;
                    switch (position) {
                        case SALESMAN:
                            totalSalesmen++;
                            break;
                        case MANAGER:
                            totalManagers++;
                            break;
                        case SALESMANAGER:
                            totalSalesManagers++;
                            break;
                    }
                }
            }
        }
        current = current->next;
    }

    printf("员工总人数：%d\n", totalEmployees);
    printf("销售人员人数：%d\n", totalSalesmen);
    printf("经理人数：%d\n", totalManagers);
    printf("销售员人数：%d\n", totalSalesManagers);
}

Node* findStaffByNo(ManagementSystem* system, const char* no) {
    Node* current = system->head;
    while (current != NULL) {
        if (strcmp(((Staff*)current->staff)->no, no) == 0) {//strcmp 函数将其员工编号与给定的 no 进行比较
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void hire(ManagementSystem* system, const char* no, const char* name, int age, Position position, double sales) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->staff = NULL;
    newNode->isEmployed = 1;
    newNode->next = NULL;

    switch (position) {
        case SALESMAN: {
            Salesman* salesman = (Salesman*)malloc(sizeof(Salesman));
            strcpy(salesman->staff.no, no);
            strcpy(salesman->staff.name, name);
            salesman->staff.age = age;
            salesman->sales = sales;
            salesman->staff.position = SALESMAN;
            newNode->staff = salesman;
            break;
        }
        case MANAGER: {
            Manager* manager = (Manager*)malloc(sizeof(Manager));
            strcpy(manager->staff.no, no);
            strcpy(manager->staff.name, name);
            manager->staff.age = age;
            manager->staff.position = MANAGER;
            newNode->staff = manager;
            break;
        }
        case SALESMANAGER: {
            SalesManager* salesManager = (SalesManager*)malloc(sizeof(SalesManager));
            strcpy(salesManager->staff.no, no);
            strcpy(salesManager->staff.name, name);
            salesManager->staff.age = age;
            salesManager->sales = sales;
            salesManager->staff.position = SALESMANAGER;
            newNode->staff = salesManager;
            break;
        }
    }

    if (system->head == NULL) {
        system->head = newNode;
    }
    else {
        Node* current = system->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    system->staffNum++;
}

void markForTermination(ManagementSystem* system) {
    char no[20];
    printf("请输入要标记解雇的员工编号：");
    scanf("%s", no);

    Node* target = findStaffByNo(system, no);

    if (target == NULL) {
        printf("未找到该员工。\n");
        return;
    }

    target->isEmployed = 0;
    printf("员工%s已被标记为待解雇。\n", no);
}

void modifyStaff(ManagementSystem* system) {
     char no[20];
    printf("请输入要修改信息的员工编号：");
    scanf("%s", no);

    Node* target = findStaffByNo(system, no);

    if (target == NULL) {
        printf("未找到该员工。\n");
        return;
    }

    void* staff = target->staff;

    if (staff == NULL) {
        printf("该员工信息为空。\n");
        return;
    }
}

void terminateStaff(ManagementSystem* system) {
        char no[20];
    printf("请输入要解雇的员工编号：");
    scanf("%s", no);

    Node* target = findStaffByNo(system, no);

    if (target == NULL) {
        printf("未找到该员工。\n");
        return;
    }

    if (!target->isEmployed) {
        printf("该员工已标记为待解雇状态。\n");
        return;
    }

    target->isEmployed = 0;
    printf("员工已标记为待解雇状态。\n");
}

void recombineFile(ManagementSystem* system) {
    FILE* outfile = fopen("staff.txt", "w");
    Node* current = system->head;
    while (current != NULL) {
        if (current->isEmployed) {
            void* staff = current->staff;
            if (staff != NULL) {
                Staff* currentStaff = (Staff*)staff;
                fprintf(outfile, "%s %s %d", currentStaff->no, currentStaff->name, currentStaff->age);
                if (current->staff != NULL) {
                    Position position = *(Position*)current->staff;
                    Staff* currentStaff = (Staff*)current->staff;
                    fprintf(outfile, "%s %s %d", currentStaff->no, currentStaff->name, currentStaff->age);
                    if (position == SALESMAN || position == SALESMANAGER) {
                        if (position == SALESMANAGER) {
                            SalesManager* salesManager = (SalesManager*)current->staff;
                            fprintf(outfile, " %.2f", salesManager->sales);
                        }
                        else {
                            Salesman* salesman = (Salesman*)current->staff;
                            fprintf(outfile, " %.2f", salesman->sales);
                        }
                    }
                }
                fprintf(outfile, "\n");
            }
        }
        current = current->next;
    }
    fclose(outfile);
    printf("文件保存成功，程序退出。\n");
}

int compareSales(const void* a, const void* b) {
    double salesA = 0.0, salesB = 0.0;

    Salesman* salesmanA = (Salesman*)((Node*)a)->staff;
    Salesman* salesmanB = (Salesman*)((Node*)b)->staff;

    if (salesmanA != NULL)
        salesA = salesmanA->sales;

    if (salesmanB != NULL)
        salesB = salesmanB->sales;

    // Sort in descending order
    if (salesA < salesB)
        return 1;
    else if (salesA > salesB)
        return -1;
    else
        return 0;
}



void sortSales(ManagementSystem* system) {
    Node* current = system->head;
    int numStaff = system->staffNum;

    // Create an array of staff nodes
    Node* staffArray = (Node*)malloc(numStaff * sizeof(Node));
    int i = 0;

    while (current != NULL) {
        staffArray[i] = *current;
        current = current->next;
        i++;
    }

    // Sort the staff array based on sales using quicksort
    qsort(staffArray, numStaff, sizeof(Node), compareSales);

    printf("员工销售额排序结果：\n");
    for (int i = 0; i < numStaff; i++) {
        Salesman* salesman = (Salesman*)staffArray[i].staff;
        printf("员工编号：%s\n", salesman->staff.no);
    }

    free(staffArray);
}

void selectFunction(ManagementSystem* system) {
    int choice;
    while (1) {
printf("请选择操作：\n");
        printf("1. 显示所有员工信息\n");
        printf("2. 显示员工总人数及各类员工人数\n");
        printf("3. 按员工号或者姓名索引信息\n");
        printf("4. 录入员工信息\n");
        printf("5. 标记待解雇员工\n");
        printf("6. 修改员工信息\n");
        printf("7. 解雇员工\n");
        printf("8. 重组文件\n");
        printf("9. 员工销售额排序\n");
        printf("0. 退出\n");
        printf("请输入操作编号：");
        scanf("%d", &choice);
        printf("----------------------\n");

        switch (choice) {
        case 1:
            displayAllStaff(system->head, system->staffNum);
            break;
        case 2:
            displayEmployeeCount(system);
            break;
        case 3:{
                char no[20];
                printf("请输入要查找的员工编号：");
                scanf("%s", no);
                Node* foundStaff = findStaffByNo(system, no);
                if (foundStaff != NULL) {
                    Staff* staff = (Staff*)foundStaff->staff;
                    printf("员工信息：\n");
                    printf("编号: %s\n", staff->no);
                    printf("姓名: %s\n", staff->name);
                    printf("年龄: %d\n", staff->age);
                } else {
                    printf("未找到该员工。\n");
                }
                break;
            }
        case 4: {
    char no[20];
    char name[30];
    int age;
    Position position;
    double sales;

    printf("请输入员工编号：");
    scanf("%s", no);
    printf("请输入员工姓名：");
    scanf("%s", name);
    printf("请输入员工年龄：");
    scanf("%d", &age);
    printf("请选择员工职位（1-经理，2-销售员）：");
    scanf("%d", (int*)&position);
    printf("请输入员工销售额：");
    scanf("%lf", &sales);

    hire(system, no, name, age, position, sales);
    break;
}

        case 5:
            markForTermination(system);
            break;
        case 6:
            modifyStaff(system);
            break;
        case 7:
            terminateStaff(system);
            break;
        case 8:
            recombineFile(system);
            return;
        case 9:
            sortSales(system);
            break;
        case 0:
            printf("程序已退出。\n");
            return;
        default:
            printf("输入错误，请重新选择操作。\n");
            break;
        }
        printf("----------------------\n");
    }
}

void quicksort(Staff** staffList, int left, int right) {
    if (left < right) {
        int pivot = partition((Staff**)staffList, left, right);
        quicksort(staffList, left, pivot - 1);
        quicksort(staffList, pivot + 1, right);
    }
}

void swap(Staff** a, Staff** b) {
    Staff* temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Staff** staffList, int left, int right) {
    double pivot = ((Salesman*)staffList[right])->sales;
    int i = left - 1;

    for (int j = left; j <= right - 1; j++) {
        if (((Salesman*)staffList[j])->sales >= pivot) {
            i++;
            swap(&staffList[i], &staffList[j]);
        }
    }

    swap(&staffList[i + 1], &staffList[right]);
    return (i + 1);
}
