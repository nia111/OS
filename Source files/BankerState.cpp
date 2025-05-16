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

// ��ʼ������
void initialize() {
    printf("�����������: ");
    scanf("%d", &process_num);
    printf("��������Դ������: ");
    scanf("%d", &resource_num);

    printf("����������������max(%d��%d):\n", process_num, resource_num);
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("�������ѷ������Allocation(%d��%d):\n", process_num, resource_num);
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    printf("�����������Դ����Available(%d):\n", resource_num);
    for (int j = 0; j < resource_num; j++) {
        scanf("%d", &available[j]);
    }
}

// ����Need����
void calculate_need() {
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// ��ʾ��ǰϵͳ״̬
void display_status() {
    printf("\n��ǰϵͳ״̬:\n");
    
    printf("Max����:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }
    
    printf("\nAllocation����:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
    
    printf("\nNeed����:\n");
    for (int i = 0; i < process_num; i++) {
        for (int j = 0; j < resource_num; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    
    printf("\n������Դ����: ");
    for (int j = 0; j < resource_num; j++) {
        printf("%d ", available[j]);
    }
    printf("\n");
}

// ��ȫ�Լ���㷨
bool safety_check() {
    // ��ʼ��work��finish
    for (int j = 0; j < resource_num; j++) {
        work[j] = available[j];
    }
    for (int i = 0; i < process_num; i++) {
        finish[i] = false;
    }

    int safe_seq[MAX_PROCESS];
    int count = 0;
    bool found;

    printf("\n��ȫ�Լ�����:\n");
    
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
                    printf("�ҵ���ִ�н���P%d\n", i);
                    printf("ִ��P%d���ͷ���Դ: ", i);
                    for (int j = 0; j < resource_num; j++) {
                        work[j] += allocation[i][j];
                        printf("%d ", allocation[i][j]);
                    }
                    printf("\n��ǰ��������: ");
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
        printf("\nϵͳ���ڰ�ȫ״̬����ȫ����Ϊ: ");
        for (int i = 0; i < process_num; i++) {
            printf("P%d", safe_seq[i]);
            if (i != process_num - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
        return true;
    } else {
        printf("\nϵͳ���ڲ���ȫ״̬�����ܵ��������Ľ���: ");
        for (int i = 0; i < process_num; i++) {
            if (!finish[i]) {
                printf("P%d ", i);
            }
        }
        printf("\n");
        
        // ��������
        printf("\n��������:\n");
        printf("1. ��Щ�����޷����ִ�У���Ϊ���ǵ���Դ�����޷�����ǰ������Դ���㡣\n");
        printf("2. ��Щ���̿������ڵȴ��˴��ͷ���Դ���γ���ѭ���ȴ�������\n");
        printf("3. ���ܵĽ����������:\n");
        printf("   - ��ֹһ�������������ͷ���Դ\n");
        printf("   - ��ĳЩ��������ռ��Դ\n");
        printf("   - ����ϵͳ������Դ����\n");
        
        return false;
    }
}

// ��Դ������
void resource_request() {
    int process_id;
    int request[MAX_RESOURCE];
    
    printf("\n������������Դ�Ľ��̺�(0-%d): ", process_num - 1);
    scanf("%d", &process_id);
    
    printf("�������������Դ����(%d������): ", resource_num);
    for (int j = 0; j < resource_num; j++) {
        scanf("%d", &request[j]);
    }
    
    // ��������Ƿ񳬹������������������
    for (int j = 0; j < resource_num; j++) {
        if (request[j] > need[process_id][j]) {
            printf("����: �������Դ�����˽����������������\n");
            return;
        }
    }
    
    // ���ϵͳ�Ƿ����㹻��Դ
    for (int j = 0; j < resource_num; j++) {
        if (request[j] > available[j]) {
            printf("����: �������Դ������ϵͳ��ǰ������Դ�����̱���ȴ���\n");
            return;
        }
    }
    
    // ���Է�����Դ
    printf("\n���Է�����Դ...\n");
    for (int j = 0; j < resource_num; j++) {
        available[j] -= request[j];
        allocation[process_id][j] += request[j];
        need[process_id][j] -= request[j];
    }
    
    display_status();
    
    // �������ϵͳ�Ƿ�ȫ
    if (safety_check()) {
        printf("��Դ����ɹ���ϵͳ�Դ��ڰ�ȫ״̬��\n");
    } else {
        // �ع�����
        printf("��Դ����ᵼ��ϵͳ����ȫ��ִ�лع�����...\n");
        for (int j = 0; j < resource_num; j++) {
            available[j] += request[j];
            allocation[process_id][j] -= request[j];
            need[process_id][j] += request[j];
        }
        printf("�ѻָ�����ǰ��״̬��\n");
    }
}

// �������м��㷨
void explain_algorithm() {
    printf("\n���м��㷨����:\n");
    printf("1. Need�������: Need[i][j] = Max[i][j] - Allocation[i][j]\n");
    printf("   - ��ʾ����i����Ҫ������Դj�������ִ��\n");
    printf("2. ��ȫ�Լ���㷨����:\n");
    printf("   a. ��ʼ��Work = Available, Finish = false\n");
    printf("   b. Ѱ��һ��Finish[i]=false��Need[i]<=Work�Ľ���i\n");
    printf("   c. ����ҵ������������ִ�У��ͷ�����Դ: Work += Allocation[i]\n");
    printf("   d. �ظ���������ֱ�����н��̶����(��ȫ)���Ҳ������������Ľ���(����ȫ)\n");
    printf("3. ��Դ������:\n");
    printf("   - ��������Ƿ񳬹������������������\n");
    printf("   - ���ϵͳ�Ƿ����㹻������Դ\n");
    printf("   - ���Է��䲢��鰲ȫ��\n");
    printf("   - �����ȫ����ɷ��䣬����ع�\n");
}

int main() {
    int choice;
    
    printf("���м��㷨ģ����\n");
    initialize();
    calculate_need();
    
    while (1) {
        printf("\n�˵�:\n");
        printf("1. ��ʾ��ǰϵͳ״̬\n");
        printf("2. ִ�а�ȫ�Լ��\n");
        printf("3. ģ����Դ����\n");
        printf("4. �������м��㷨\n");
        printf("5. �˳�\n");
        printf("��ѡ�����: ");
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
                printf("��Чѡ�����������롣\n");
        }
    }
    
    return 0;
}