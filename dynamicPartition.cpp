#include <iostream>
#include <vector>
using namespace std;

enum METHOD
{
    FF = 1, // 最先适应
    BF,     // 最佳适应
    WF      // 最坏适应
};
enum OPERATE
{
    allocate = 1, // 分配内存
    release       // 释放内存
};
typedef struct memoryPartition//用于存放所有分区信息
{
    int begin;     // 开始地址
    int space;     // 大小
    int processID; // 进程id
} memoryPartition;
typedef struct memoryPartition_sort//用于排序空闲分区
{
    int begin;     // 开始地址
    int space;     // 大小
    int idx;       // 在全局分区情况中的位置
} memoryPartition_sort;
vector<memoryPartition> globeMemoryStatus;//所有分区信息
vector<memoryPartition_sort> firstFree; // 最先适应空闲分区
vector<memoryPartition_sort> bestFree;  // 最佳适应空闲分区
vector<memoryPartition_sort> worstFree; // 最坏适应空闲分区

// 释放内存
void releaseMemory(int proID)
{
    memoryPartition releaseTemp;
    memoryPartition releaseTempPre;
    memoryPartition releaseTempBack;
    while (1)
    {
        releaseTempBack.processID = releaseTempPre.processID = releaseTemp.processID = -2;
        for (int i = 0; i < globeMemoryStatus.size(); i++)
        {
            if (globeMemoryStatus[i].processID == proID)
            {
                releaseTemp = globeMemoryStatus[i];
                releaseTemp.processID = -1;
                globeMemoryStatus.erase(globeMemoryStatus.begin() + i);
                break;
            }
        }
        if (releaseTemp.processID == -2) // 没有该进程了
        {
            break;
        }
        // 该分区前是否有空闲
        for (int i = 0; i < globeMemoryStatus.size(); i++)
        {
            if (globeMemoryStatus[i].processID == -1 && (globeMemoryStatus[i].begin + globeMemoryStatus[i].space) == releaseTemp.begin)
            { // 有空闲
                releaseTempPre = globeMemoryStatus[i];
                globeMemoryStatus.erase(globeMemoryStatus.begin() + i);
                break;
            }
        }
        // 该分区后是否有空闲
        for (int i = 0; i < globeMemoryStatus.size(); i++)
        {
            if (globeMemoryStatus[i].processID == -1 && (releaseTemp.begin + releaseTemp.space) == globeMemoryStatus[i].begin)
            { // 有空闲
                releaseTempBack = globeMemoryStatus[i];
                globeMemoryStatus.erase(globeMemoryStatus.begin() + i);
                break;
            }
        }
        if (releaseTempPre.processID == -1 || releaseTempBack.processID == -1)
        {
            if (releaseTempPre.processID == -1 && releaseTempBack.processID == -1)
            {
                releaseTemp.begin = releaseTempPre.begin;
                releaseTemp.space += (releaseTempPre.space + releaseTempBack.space);
            }
            else if (releaseTempPre.processID == -1)
            {
                releaseTemp.begin = releaseTempPre.begin;
                releaseTemp.space += releaseTempPre.space;
            }
            else
            {
                releaseTemp.space += releaseTempBack.space;
            }
        }
        globeMemoryStatus.push_back(releaseTemp);
    }
    cout << "释放完成" << endl;
}
// 分配
void allocateMemory(int idx, int proID, int size)
{
    int temp = globeMemoryStatus[idx].space - size; // 计算分配后大小
    memoryPartition newMemory;
    newMemory.begin = globeMemoryStatus[idx].begin;
    newMemory.processID = proID;
    newMemory.space = size;
    globeMemoryStatus.push_back(newMemory);
    if (temp > 0)
    {
        globeMemoryStatus[idx].begin = newMemory.begin + newMemory.space;
        globeMemoryStatus[idx].space = temp;
    }
    else
    {
        globeMemoryStatus.erase(globeMemoryStatus.begin() + idx);
        idx--;
    }
}

// 排序算法
void sortBeforeChoose(METHOD m, vector<memoryPartition_sort> &globeFree)
{
    vector<memoryPartition_sort> temp;
    for (int i = 0; i < globeMemoryStatus.size(); i++)
    {
        if (globeMemoryStatus[i].processID == -1)
        {
            memoryPartition_sort t;
            t.begin = globeMemoryStatus[i].begin;
            t.space = globeMemoryStatus[i].space;
            t.idx = i;
            temp.push_back(t);
        }
    }
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = i; j < temp.size(); j++)
        {
            int compare1, compare2;
            if (m == FF)
            {
                compare1 = temp[i].begin;
                compare2 = temp[j].begin;
            }
            else
            {
                compare1 = temp[i].space;
                compare2 = temp[j].space;
            }
            if (m == WF)
            {
                if (compare1 < compare2) // 大分区在前
                {
                    memoryPartition_sort t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = temp[i];
                }
            }
            else
            {
                if (compare1 > compare2) // 小分区/小地址在前
                {
                    memoryPartition_sort t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = temp[i];
                }
            }
        }
    }
    globeFree = temp;
}

// 最先适应法/排序版！
bool firstFit_sort(int proID, int size)
{
    sortBeforeChoose(FF, firstFree);
    for (int i = 0; i < firstFree.size(); i++)
    {
        if (firstFree[i].space >= size)
        {
            allocateMemory(firstFree[i].idx, proID, size);
            return true;
        }
    }
    return false;
}
// 最佳适应法/排序版！
bool bestFit_sort(int proID, int size)
{
    sortBeforeChoose(BF, bestFree);
    for (int i = 0; i < firstFree.size(); i++)
    {
        if (bestFree[i].space >= size)
        {
            allocateMemory(firstFree[i].idx, proID, size);
            return true;
        }
    }
    return false;
}
// 最坏适应法/排序版！
bool worstFit_sort(int proID, int size)
{
    sortBeforeChoose(WF, worstFree);
    if (worstFree[1].space >= size)
    {
        allocateMemory(1, proID, size);
        return true;
    }
    return false;
}

// 最先适应法
bool firstFit(int proID, int size)
{
    int fitIdx = -1;
    int minAddress = INT_MAX;
    // 寻找能够分配且首址最小
    for (int i = 0; i < globeMemoryStatus.size(); i++)
    {
        if (globeMemoryStatus[i].processID == -1 && globeMemoryStatus[i].space >= size && globeMemoryStatus[i].begin < minAddress)
        {
            fitIdx = i;
            minAddress = globeMemoryStatus[i].begin;
        }
    }
    if (fitIdx != -1)
    {
        allocateMemory(fitIdx, proID, size);
        return true;
    }

    return false;
}
// 最佳适应法
bool bestFit(int proID, int size)
{
    int bestFitIdx = -1;
    int minFreeSize = INT_MAX;
    // 寻找能够分配且碎片最小
    for (int i = 0; i < globeMemoryStatus.size(); i++)
    {
        if (globeMemoryStatus[i].processID == -1 && globeMemoryStatus[i].space >= size && globeMemoryStatus[i].space < minFreeSize)
        {
            bestFitIdx = i;
            minFreeSize = globeMemoryStatus[i].space;
        }
    }
    if (bestFitIdx != -1)
    {
        allocateMemory(bestFitIdx, proID, size);
        return true;
    }
    return false;
}
// 最坏适应法
bool worstFit(int proID, int size)
{
    int worstFitIdx = -1;
    int maxFreeSize = -1;
    // 寻找能够分配且剩余最多
    for (int i = 0; i < globeMemoryStatus.size(); i++)
    {
        if (globeMemoryStatus[i].processID == -1 && globeMemoryStatus[i].space >= size && globeMemoryStatus[i].space > maxFreeSize)
        {
            worstFitIdx = i;
            maxFreeSize = globeMemoryStatus[i].space;
        }
    }
    if (worstFitIdx != -1)
    {
        allocateMemory(worstFitIdx, proID, size);
        return true;
    }
    return false;
}

// 显示内存
void displayMemory(int memory, vector<memoryPartition> memoryStatus)
{
    cout << "内存使用情况：" << endl;
    for (int i = 0; i < memoryStatus.size(); i++)
    {
        cout << "起始地址：" << memoryStatus[i].begin << ", "
             << "大小："
             << memoryStatus[i].space << ", ";
        if (memoryStatus[i].processID == -1)
        {
            cout << "空闲";
        }
        else
        {
            cout << "进程ID：" << memoryStatus[i].processID;
        }
        cout << endl;
    }
}

// 用户操作界面
void userChoose(int memory, int processNum, bool (*func)(int, int))
{
    int choice = 0;
    int processID = -1;
    int size = 0;
    while (true)
    {
        cout << "\n1. 分配内存\n2. 释放内存\n3. 显示内存使用情况\n4. 退出\n";
        cout << "选择操作：";
        cin >> choice;

        switch (choice)
        {
        case allocate:
            cout << "输入进程ID：";
            cin >> processID;
            if (processID < 0 || processID >= processNum)
            {
                cout << "进程ID错误" << endl;
                continue;
            }
            cout << "输入进程所需内存大小：";
            cin >> size;
            if (func(processID, size))
            {
                cout << "分配成功" << endl;
            }
            else
            {
                cout << "分配失败" << endl;
                // exit(0);//一旦分配失败整个程序退出
            }
            break;
        case release:
            cout << "输入进程ID：";
            cin >> processID;
            if (processID < 0 || processID >= processNum)
            {
                cout << "进程ID错误" << endl;
                continue;
            }
            releaseMemory(processID);
            break;
        case 3:
            displayMemory(memory, globeMemoryStatus);
            break;
        case 4:
            exit(0);
        default:
            cout << "无效的选择" << endl;
            break;
        }
    }
}

void allocateMemorySystem(int method, int memory, int processNum, vector<memoryPartition> memoryStatus)
{
    switch (method)
    {
    case FF:
        userChoose(memory, processNum, firstFit);
        break;
    case BF:
        userChoose(memory, processNum, bestFit);
        break;
    case WF:
        userChoose(memory, processNum, worstFit);
        break;
    default:
        break;
    }
}

int main()
{
    int memory = 0;     // 内存大小
    int processNum = 0; // 进程数
    int method = 0;     // 分配算法
    cout << "指定内存大小（整数）：" << endl;
    cin >> memory;
    cout << "指定进程数量（整数）" << endl;
    cin >> processNum;

    vector<int> numbers(processNum);
    // 填充数组
    for (int i = 0; i < processNum; i++)
    {
        numbers[i] = i;
    }
    cout << "进程ID顺序编号为：" << endl;
    for (int i = 0; i < processNum; i++)
    {
        cout << numbers[i] << " ";
    }
    memoryPartition temp;
    temp.begin = 0;
    temp.space = memory;
    temp.processID = -1;
    globeMemoryStatus.push_back(temp); // 初始情况，整个内存空闲
    cout << endl;
    cout << "选择此次模拟系统的分配算法" << endl
         << "1.最先适应法" << endl
         << "2.最佳适应法" << endl
         << "3.最坏适应法" << endl;
    cin >> method;
    allocateMemorySystem(method, memory, processNum, globeMemoryStatus);
    return 0;
}
