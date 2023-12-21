#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// 磁盘调度算法枚举
enum METHOD
{
    FCFS, // 先来先服务法
    SSTF, // 最短寻道时间优先法
    SCAN  // 电梯算法
};
// csdn https://blog.csdn.net/badbad_boy/article/details/4313645 说磁道0号在最外圈
enum DIRECT
{
    OUT = -1, // 向外,先访问磁道号更小的
    IN = 1    // 向内,先访问磁道号更大的
};
// 计算磁头移动的总磁道数
int calculateTotalSeekTime(const vector<int> &sequence)
{
    int totalSeekTime = 0;
    for (int i = 1; i < sequence.size(); i++)
    {
        totalSeekTime += abs(sequence[i] - sequence[i - 1]);
    }
    return totalSeekTime;
}
// 寻找离pos最近的迭代器
vector<int>::iterator findMinDistance(vector<int> &seq, int pos)
{
    return min_element(seq.begin(), seq.end(),
                       [pos](int a, int b)
                       {
                           return abs(a - pos) < abs(b - pos);
                       });
}
// 先来先服务法（FCFS）
vector<int> fcfsMethod(const vector<int> &sequence, int initialPosition, int d)
{
    vector<int> result(sequence);
    // 先来先服务按顺序调度，将当前位置插入头部返回即可
    result.insert(result.begin(), initialPosition);
    return result;
}
// 最短寻道时间优先法（SSTF）
vector<int> sstfMethod(const vector<int> &sequence, int initialPosition, int d)
{
    vector<int> result;
    vector<int> seq(sequence);
    result.push_back(initialPosition);
    while (seq.size() > 0)
    {
        int pos = result[result.size() - 1];
        vector<int>::iterator minDistanceIt = findMinDistance(seq, pos);
        result.push_back(*(minDistanceIt));
        seq.erase(minDistanceIt);
    }
    return result;
}
// 电梯算法
vector<int> scanMethod(const vector<int> &sequence, int initialPosition, int d)
{
    vector<int> result;
    result.push_back(initialPosition);
    vector<int> inToCurrent;  // 相对于当前位置在里面的，磁道号更大的
    vector<int> outToCurrent; // 相对于当前位置在外面的，磁道号更小的
    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] > initialPosition)
        {
            inToCurrent.push_back(sequence[i]);
        }
        else if (sequence[i] < initialPosition)
        {
            outToCurrent.push_back(sequence[i]);
        }
        else
        {
            continue;
        }
    }
    switch (d)
    {
    case IN: // 向内先访问更大的部分
        while (inToCurrent.size() > 0)
        {
            int pos = result[result.size() - 1];
            vector<int>::iterator minDistanceIt = findMinDistance(inToCurrent, pos);
            result.push_back(*(minDistanceIt));
            inToCurrent.erase(minDistanceIt);
        }
        while (outToCurrent.size() > 0)
        {
            int pos = result[result.size() - 1];
            vector<int>::iterator minDistanceIt = findMinDistance(outToCurrent, pos);
            result.push_back(*(minDistanceIt));
            outToCurrent.erase(minDistanceIt);
        }
        break;
    case OUT: // 向外先访问更小的部分
        while (outToCurrent.size() > 0)
        {
            int pos = result[result.size() - 1];
            vector<int>::iterator minDistanceIt = findMinDistance(outToCurrent, pos);
            result.push_back(*(minDistanceIt));
            outToCurrent.erase(minDistanceIt);
        }
        while (inToCurrent.size() > 0)
        {
            int pos = result[result.size() - 1];
            vector<int>::iterator minDistanceIt = findMinDistance(inToCurrent, pos);
            result.push_back(*(minDistanceIt));
            inToCurrent.erase(minDistanceIt);
        }
        break;
    default:
        cout << "错误的输入" << endl;
        return result;
    }
    return result;
}

typedef vector<int> (*FunctionPtr)(const vector<int> &, int, int);
// 同一处理子函数
void handle(int m, FunctionPtr func, const vector<int> &sequence, int initialPosition, int direct)
{
    vector<int> resultSequence = func(sequence, initialPosition, direct);
    int totalSeekTime = calculateTotalSeekTime(resultSequence);
    // FCFS, // 先来先服务法
    // SSTF, // 最短寻道时间优先法
    // SCAN  // 电梯算法
    string mstr = "";
    switch (m)
    {
    case FCFS: // 先来先服务法
        mstr = "先来先服务法（FCFS）";
        break;
    case SSTF: // 最短寻道时间优先法
        mstr = "最短寻道时间优先法（SSTF）";
        break;
    case SCAN: // 电梯算法
        mstr = "电梯算法（SCAN）";
        break;
    default:
        cout << "方法输入错误！" << endl;
        return;
    }
    // 显示磁盘调度结果
    cout << mstr << "的磁道访问顺序：" << endl;
    for (int i = 1; i < resultSequence.size(); i++)
    {
        cout << resultSequence[i] << " ";
    }
    cout << endl;
    cout << "总磁道数：" << totalSeekTime << endl;
}

int main()
{
    int currentPos = -1;    // 当前位置
    int currentDir = 0;     // 移动方向
    vector<int> responeSeq; // 请求序列
                            // 输入当前磁头位置、磁头移动方向、磁道访问请求序列等
    cout << "请输入当前磁头位置：";
    cin >> currentPos;

    do
    {
        cout << "请输入磁头移动方向（1表示向内，-1表示向外，错误将重复这一步骤）：";
        cin >> currentDir;
    } while (!(currentDir == -1 || currentDir == 1));

    int resNum = 0;
    cout << "请输入磁道访问请求序列的磁道数：";
    cin >> resNum;

    cout << "请输入磁道访问请求序列：" << endl;
    for (int i = 0; i < resNum; i++)
    {
        int request;
        cin >> request;
        responeSeq.push_back(request);
    }
    handle(FCFS, fcfsMethod, responeSeq, currentPos, currentDir);
    handle(SSTF, sstfMethod, responeSeq, currentPos, currentDir);
    handle(FCFS, scanMethod, responeSeq, currentPos, currentDir);
    return 0;
}