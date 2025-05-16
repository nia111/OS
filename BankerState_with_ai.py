# -*- coding: utf-8 -*-
import sys
import io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

from sparkai.llm.llm import ChatSparkLLM, ChunkPrintHandler
from sparkai.core.messages import ChatMessage

MAX_PROCESS = 10
MAX_RESOURCE = 10

# 初始化全局变量
process_num = 0
resource_num = 0
max_matrix = []
allocation = []
need = []
available = []

# 控制台日志记录
console_log = []

def log_console(message):
    print(message)
    console_log.append(message)

def initialize():
    global process_num, resource_num, max_matrix, allocation, need, available

    process_num = int(input("请输入进程数: "))
    resource_num = int(input("请输入资源类型数: "))

    max_matrix.clear()
    allocation.clear()
    need.clear()

    log_console(f"请输入最大需求矩阵Max({process_num}×{resource_num}):")
    for i in range(process_num):
        while True:
            row = list(map(int, input(f"第{i}行: ").split()))
            if len(row) != resource_num:
                log_console(f"错误：需要输入{resource_num}个数字。请重试。")
            else:
                max_matrix.append(row)
                break

    log_console(f"请输入已分配矩阵Allocation({process_num}×{resource_num}):")
    for i in range(process_num):
        while True:
            row = list(map(int, input(f"第{i}行: ").split()))
            if len(row) != resource_num:
                log_console(f"错误：需要输入{resource_num}个数字。请重试。")
            else:
                allocation.append(row)
                need.append([max_matrix[i][j] - row[j] for j in range(resource_num)])
                break

    while True:
        available = list(map(int, input(f"请输入可用资源向量Available({resource_num}): ").split()))
        if len(available) != resource_num:
            log_console(f"错误：需要输入{resource_num}个数字。请重试。")
        else:
            break

def calculate_need():
    global need
    need = [[max_matrix[i][j] - allocation[i][j] for j in range(resource_num)] for i in range(process_num)]

def display_status():
    log_console("\n当前系统状态:")
    log_console("Max矩阵:")
    for row in max_matrix:
        log_console(" ".join(map(str, row)))

    log_console("\nAllocation矩阵:")
    for row in allocation:
        log_console(" ".join(map(str, row)))

    log_console("\nNeed矩阵:")
    for row in need:
        log_console(" ".join(map(str, row)))

    log_console("\n可用资源向量: " + " ".join(map(str, available)))

def safety_check():
    work = available[:]
    finish = [False] * process_num
    safe_seq = []

    log_console("\n安全性检测过程:")

    while len(safe_seq) < process_num:
        found = False
        for i in range(process_num):
            if not finish[i] and all(need[i][j] <= work[j] for j in range(resource_num)):
                log_console(f"找到可执行进程P{i}")
                log_console(f"执行P{i}后释放资源: {' '.join(map(str, allocation[i]))}")
                for j in range(resource_num):
                    work[j] += allocation[i][j]
                log_console("当前工作向量: " + " ".join(map(str, work)))
                finish[i] = True
                safe_seq.append(i)
                found = True
                break
        if not found:
            break

    if len(safe_seq) == process_num:
        log_console("\n系统处于安全状态。安全序列为: " + " -> ".join(f"P{i}" for i in safe_seq))
        return True
    else:
        unfinished = [f"P{i}" for i, done in enumerate(finish) if not done]
        log_console("\n系统处于不安全状态。可能导致死锁的进程: " + " ".join(unfinished))
        log_console("\n自动调用AI分析死锁原因和解决方案...\n")
        explain_with_spark()
        return False

def resource_request():
    process_id = int(input(f"\n请输入请求资源的进程号(0-{process_num - 1}): "))
    request = list(map(int, input(f"请输入请求的资源向量({resource_num}个数字): ").split()))

    if len(request) != resource_num:
        log_console(f"错误：请求向量长度应为{resource_num}")
        return

    if any(request[j] > need[process_id][j] for j in range(resource_num)):
        log_console("错误: 请求的资源超过了进程声明的最大需求。")
        return

    if any(request[j] > available[j] for j in range(resource_num)):
        log_console("错误: 请求的资源超过了系统当前可用资源。进程必须等待。")
        return

    log_console("\n尝试分配资源...")
    for j in range(resource_num):
        available[j] -= request[j]
        allocation[process_id][j] += request[j]
        need[process_id][j] -= request[j]

    display_status()

    if safety_check():
        log_console("资源分配成功，系统仍处于安全状态。")
    else:
        log_console("资源分配会导致系统不安全，执行回滚操作...")
        for j in range(resource_num):
            available[j] += request[j]
            allocation[process_id][j] -= request[j]
            need[process_id][j] += request[j]
        log_console("已恢复分配前的状态。")

def format_matrix_with_triplet():
    """
    返回字符串，显示每个进程对应的 Max/Allocation/Need 三元组，方便对比
    """
    lines = ["【进程资源状态 Max / Allocation / Need】:"]
    for i in range(process_num):
        triplet = []
        for j in range(resource_num):
            triplet.append(f"{max_matrix[i][j]}/{allocation[i][j]}/{need[i][j]}")
        lines.append(f"  P{i}: {' '.join(triplet)}  (格式: Max/Alloc/Need)")
    return "\n".join(lines)
#ai解释部分
def explain_with_spark():
    messages = []

    def format_matrix(title, matrix):
        lines = [f"{title}:"]
        for i, row in enumerate(matrix):
            lines.append(f"  P{i}: {' '.join(map(str, row))}")
        return "\n".join(lines)

    state_description = "\n".join([
        "以下是银行家算法当前系统状态，请结合算法规则进行严谨分析：",
        format_matrix("【最大需求矩阵 Max】", max_matrix),
        format_matrix("【已分配矩阵 Allocation】", allocation),
        format_matrix("【需求矩阵 Need】", need),
        format_matrix_with_triplet(),
        "【可用资源向量 Available】：" + " ".join(map(str, available)),
        "",
        "请严格基于银行家算法的基本规则分析：",
        "- Max 是最大资源需求",
        "- Allocation 是当前分配资源",
        "- Need = Max - Allocation",
        "- 进程仅当 Need <= Work 时可以执行",
        "- 若所有进程能依次完成释放资源，则系统处于安全状态",
        "- 若无进程满足 Need <= Work，且仍有进程未完成，系统处于不安全状态",
        "- 结合进程执行顺序动态更新Work,如果之前判断不可执行，更新work后记得返回验证。",
        "",
        "请按照以下格式输出每个进程的分析：",
        "- P0：Need = [...], Work = [...], 判断是否满足 Need <= Work？说明原因",
        "- 如果满足，执行后释放资源 Allocation = [...]，更新 Work 向量",
        "- 若系统无法继续，指出导致停滞的进程及其 Need 和当前 Work",
        "",
        "若存在死锁，请分析是否形成循环等待（可构造资源分配图或等待图简要说明），并至少提供三条可行解决方案：",
        "- 终止某些进程",
        "- 强制抢占资源",
        "- 增加可用资源",
    ])

    context_log = "\n".join(console_log[-50:])
    combined_prompt = "\n".join([
        "请基于以下控制台日志判断当前系统状态并提出建议：",
        context_log,
        "\n当前系统状态：",
        state_description
    ])

    messages.append(ChatMessage(role="user", content=combined_prompt))

    spark = ChatSparkLLM(
        spark_api_url='wss://spark-api.xf-yun.com/v4.0/chat',
        spark_app_id='8a544194',
        spark_api_key='2ed096f501e1956119b3c06205ca766d',
        spark_api_secret='NGE0ZGVkZmE5OWE4NWJlNjdmNWQ3M2Y5',
        spark_llm_domain='4.0Ultra',
        streaming=False
    )

    handler = ChunkPrintHandler()
    while True:
        response = spark.generate([messages], callbacks=[handler])
        if response and response.generations:
            reply = response.generations[0][0].text
            log_console("\nAI回复: " + reply)
            messages.append(ChatMessage(role="assistant", content=reply))
        else:
            log_console("AI未能生成有效回复。")

        user_input = input("\n你可以继续追问（输入0退出AI对话）：\n> ")
        if user_input.strip() == "0":
            log_console("退出AI解释模式。")
            break
        messages.append(ChatMessage(role="user", content=user_input))

def main():
    initialize()
    calculate_need()

    while True:
        print("\n菜单:")
        print("1. 显示当前系统状态")
        print("2. 执行安全性检测")
        print("3. 模拟资源请求")
        print("4. AI解释")
        print("5. 退出")
        choice = input("请选择操作: ")

        if choice == '1':
            display_status()
        elif choice == '2':
            safety_check()
        elif choice == '3':
            resource_request()
        elif choice == '4':
            explain_with_spark()
        elif choice == '5':
            sys.exit()
        else:
            log_console("无效选择，请重新输入。")

if __name__ == "__main__":
    main()
