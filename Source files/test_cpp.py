import subprocess
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
exe_path = os.path.join(BASE_DIR, "BankerState.exe")
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
4
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
1
2
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
5'''
]

for idx, input_data in enumerate(inputs):
    print(f"\n========= 执行第 {idx + 1} 组 =========\n")

    process = subprocess.Popen(
        exe_path,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    stdout, stderr = process.communicate(input=input_data)

    print(stdout)

    if stderr:
        print(f"\n⚠️ 第 {idx + 1} 组发生错误:\n", stderr)
