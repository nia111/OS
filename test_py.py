import subprocess
import sys
import os

sys.stdout.reconfigure(encoding='utf-8')

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
script_path = os.path.join(BASE_DIR, "BankerState_with_ai.py")

inputs = [
    '''4
3
3 2 2 
6 1 3
3 1 4 
4 2 2 
1 0 0
5 1 1 
2 1 1 
0 0 2
1 1 2
1
2
4
0
5''',

    '''4
3
3 2 2 
6 1 3
3 1 4 
4 2 2 
1 0 0
5 1 1 
2 1 1 
0 0 2
0 0 1
1
2
0
5''',

    '''4
3
3 2 2 
6 1 3
3 1 4 
4 2 2 
1 0 0
5 1 1 
2 1 1 
0 0 2
1 1 2
1
2
3
1
1 0 1
4
0
5''',

    '''4
3
3 2 2 
6 1 3
3 1 4 
4 2 2 
1 0 0
5 1 1 
2 1 1 
0 0 2
1 1 2
1
2
3
1
1 1 2
1
2
4
为什么p1不能request 1 1 2
0
5''',

    '''4
3
3 2 2 
6 1 3
3 1 4 
4 2 2 
1 0 0
5 1 1 
2 1 1 
0 0 2
1 1 2
1
2
3
1
2 2 2
1
2
4
为什么p1不能request 2 2 2
0
5'''
]

# 使用系统默认解释器运行 Python 文件
python_exe = sys.executable

for idx, input_data in enumerate(inputs):
    print(f"\n{'=' * 20} 执行第 {idx + 1} 组 {'=' * 20}\n")

    # 运行 Python 脚本
    process = subprocess.Popen(
        [python_exe, script_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='utf-8',
        errors='replace'
    )

    stdout, stderr = process.communicate(input=input_data)

    # 打印结果
    print(stdout)

