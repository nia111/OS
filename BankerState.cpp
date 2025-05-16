#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESS 10
#define MAX_RESOURCE 10

int process_num, resource_num;
int max[MAX_PROCESS][MAX_RESOURCE];
int allocation[MAX_PROCESS][MAX_RESOURCE];
int need[MAX_PROCESS][MAX_RESOURCE];
int available[MAX_RESOURCE];
int work[MAX_RESOURCE];
bool finish[MAX_PROCESS];

// 初始化数据
void initialize() {
    printf("请输入进程数: ");
    scanf("%d", &process_num);
    printf("请输入资源类型数: ");
    scanf("%d", &resource_num);

    printf("请输入最大需求矩阵max(%d×%d):\n", process_num, resource_num);
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("请输入已分配矩阵Allocation(%d×%d):\n", process_num, resource_num);
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    printf("请输入可用资源向量Available(%d):\n", resource_num);
    for (int j = 0; j < resource_num; j++) {
        scanf("%d", &available[j]);
    }
}

// 计算Need矩阵
void calculate_need() {
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// 显示当前系统状态
void display_status() {
    printf("\n当前系统状态:\n");
    
    printf("Max矩阵:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }
    
    printf("\nAllocation矩阵:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
    
    printf("\nNeed矩阵:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    
    printf("\n可用资源向量: ");
    for (int j = 0; j < resource_num; j++) {
        printf("%d ", available[j]);
    }
    printf("\n");
}

// 安全性检测算法
bool safety_check() {
    // 初始化work和finish
    for (int j = 0; j < resource_num; j++) {
        work[j] = available[j];
    }
    for (int i = 0; i < process_num; i++) {
        finish[i] = false;
    }

    int safe_seq[MAX_PROCESS];
    int count = 0;
    bool found;

    printf("\n安全性检测过程:\n");
    
    while (count < process_num) {
        found = false;
        
        for (int i = 0; i < process_num; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < resource_num; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                
                if (can_allocate) {
                    printf("找到可执行进程P%d\n", i);
                    printf("执行P%d后释放资源: ", i);
                    for (int j = 0; j < resource_num; j++) {
                        work[j] += allocation[i][j];
                        printf("%d ", allocation[i][j]);
                    }
                    printf("\n当前工作向量: ");
                    for (int j = 0; j < resource_num; j++) {
                        printf("%d ", work[j]);
                    }
                    printf("\n");
                    
                    finish[i] = true;
                    safe_seq[count++] = i;
                    found = true;
                    break;
                }
            }
        }
        
        if (!found) {
            break;
        }
    }
    
    if (count == process_num) {
        printf("\n系统处于安全状态。安全序列为: ");
        for (int i = 0; i < process_num; i++) {
            printf("P%d", safe_seq[i]);
            if (i != process_num - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
        return true;
    } else {
        printf("\n系统处于不安全状态。可能导致死锁的进程: ");
        for (int i = 0; i < process_num; i++) {
            if (!finish[i]) {
                printf("P%d ", i);
            }
        }
        printf("\n");
        
        // 死锁分析
        printf("\n死锁分析:\n");
        printf("1. 这些进程无法完成执行，因为它们的资源需求无法被当前可用资源满足。\n");
        printf("2. 这些进程可能正在等待彼此释放资源，形成了循环等待条件。\n");
        printf("3. 可能的解决方案包括:\n");
        printf("   - 终止一个或多个进程以释放资源\n");
        printf("   - 从某些进程中抢占资源\n");
        printf("   - 增加系统可用资源数量\n");
        
        return false;
    }
}

// 资源请求处理
void resource_request() {
    int process_id;
    int request[MAX_RESOURCE];
    
    printf("\n请输入请求资源的进程号(0-%d): ", process_num - 1);
    scanf("%d", &process_id);
    
    printf("请输入请求的资源向量(%d个数字): ", resource_num);
    for (int j = 0; j < resource_num; j++) {
        scanf("%d", &request[j]);
    }
    
    // 检查请求是否超过进程声明的最大需求
    for (int j = 0; j < resource_num; j++) {
        if (request[j] > need[process_id][j]) {
            printf("错误: 请求的资源超过了进程声明的最大需求。\n");
            return;
        }
    }
    
    // 检查系统是否有足够资源
    for (int j = 0; j < resource_num; j++) {
        if (request[j] > available[j]) {
            printf("错误: 请求的资源超过了系统当前可用资源。进程必须等待。\n");
            return;
        }
    }
    
    // 尝试分配资源
    printf("\n尝试分配资源...\n");
    for (int j = 0; j < resource_num; j++) {
        available[j] -= request[j];
        allocation[process_id][j] += request[j];
        need[process_id][j] -= request[j];
    }
    
    display_status();
    
    // 检查分配后系统是否安全
    if (safety_check()) {
        printf("资源分配成功，系统仍处于安全状态。\n");
    } else {
        // 回滚分配
        printf("资源分配会导致系统不安全，执行回滚操作...\n");
        for (int j = 0; j < resource_num; j++) {
            available[j] += request[j];
            allocation[process_id][j] -= request[j];
            need[process_id][j] += request[j];
        }
        printf("已恢复分配前的状态。\n");
    }
}

// 解释银行家算法
void explain_algorithm() {
    printf("\n银行家算法解析:\n");
    printf("1. Need矩阵计算: Need[i][j] = Max[i][j] - Allocation[i][j]\n");
    printf("   - 表示进程i还需要多少资源j才能完成执行\n");
    printf("2. 安全性检测算法步骤:\n");
    printf("   a. 初始化Work = Available, Finish = false\n");
    printf("   b. 寻找一个Finish[i]=false且Need[i]<=Work的进程i\n");
    printf("   c. 如果找到，假设它完成执行，释放其资源: Work += Allocation[i]\n");
    printf("   d. 重复上述步骤直到所有进程都完成(安全)或找不到符合条件的进程(不安全)\n");
    printf("3. 资源请求处理:\n");
    printf("   - 检查请求是否超过进程声明的最大需求\n");
    printf("   - 检查系统是否有足够可用资源\n");
    printf("   - 尝试分配并检查安全性\n");
    printf("   - 如果安全则完成分配，否则回滚\n");
}

int main() {
    int choice;
    
    printf("银行家算法模拟器\n");
    initialize();
    calculate_need();
    
    while (1) {
        printf("\n菜单:\n");
        printf("1. 显示当前系统状态\n");
        printf("2. 执行安全性检测\n");
        printf("3. 模拟资源请求\n");
        printf("4. 解释银行家算法\n");
        printf("5. 退出\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                display_status();
                break;
            case 2:
                safety_check();
                break;
            case 3:
                resource_request();
                break;
            case 4:
                explain_algorithm();
                break;
            case 5:
                exit(0);
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
    
    return 0;
}