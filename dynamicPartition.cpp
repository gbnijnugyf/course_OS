#include <iostream>
#include <vector>
using namespace std;

enum METHOD
{
    FF = 1, // ������Ӧ
    BF,     // �����Ӧ
    WF      // ���Ӧ
};
enum OPERATE
{
    allocate = 1, // �����ڴ�
    release       // �ͷ��ڴ�
};
typedef struct memoryPartition
{
    int begin;     // ��ʼ��ַ
    int space;     // ��С
    int processID; // ����id
} memoryPartition;
vector<memoryPartition> globeMemoryStatus;

// �ͷ��ڴ�
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
        if (releaseTemp.processID == -2) // û�иý�����
        {
            break;
        }
        // �÷���ǰ�Ƿ��п���
        for (int i = 0; i < globeMemoryStatus.size(); i++)
        {
            if (globeMemoryStatus[i].processID == -1 && (globeMemoryStatus[i].begin + globeMemoryStatus[i].space) == releaseTemp.begin)
            { // �п���
                releaseTempPre = globeMemoryStatus[i];
                globeMemoryStatus.erase(globeMemoryStatus.begin() + i);
                break;
            }
        }
        // �÷������Ƿ��п���
        for (int i = 0; i < globeMemoryStatus.size(); i++)
        {
            if (globeMemoryStatus[i].processID == -1 && (releaseTemp.begin + releaseTemp.space) == globeMemoryStatus[i].begin)
            { // �п���
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
    cout << "�ͷ����" << endl;
}
// ����
void allocateMemory(int idx, int proID, int size)
{
    int temp = globeMemoryStatus[idx].space - size; // ���������С
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
// ������Ӧ��
bool firstFit(int proID, int size)
{
    int fitIdx = -1;
    int minAddress = INT_MAX;
    // Ѱ���ܹ���������ַ��С
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
// �����Ӧ��
bool bestFit(int proID, int size)
{
    int bestFitIdx = -1;
    int minFreeSize = INT_MAX;
    // Ѱ���ܹ���������Ƭ��С
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
// ���Ӧ��
bool worstFit(int proID, int size)
{
    int worstFitIdx = -1;
    int maxFreeSize = -1;
    // Ѱ���ܹ�������ʣ�����
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

// ��ʾ�ڴ�
void displayMemory(int memory, vector<memoryPartition> memoryStatus)
{
    cout << "�ڴ�ʹ�������" << endl;
    for (int i = 0; i < memoryStatus.size(); i++)
    {
        cout << "��ʼ��ַ��" << memoryStatus[i].begin << ", "
             << "��С��"
             << memoryStatus[i].space << ", ";
        if (memoryStatus[i].processID == -1)
        {
            cout << "����";
        }
        else
        {
            cout << "����ID��" << memoryStatus[i].processID;
        }
        cout << endl;
    }
}

// �û���������
void userChoose(int memory, int processNum, bool (*func)(int, int))
{
    int choice = 0;
    int processID = -1;
    int size = 0;
    while (true)
    {
        cout << "\n1. �����ڴ�\n2. �ͷ��ڴ�\n3. ��ʾ�ڴ�ʹ�����\n4. �˳�\n";
        cout << "ѡ�������";
        cin >> choice;

        switch (choice)
        {
        case allocate:
            cout << "�������ID��";
            cin >> processID;
            if (processID < 0 || processID >= processNum)
            {
                cout << "����ID����" << endl;
                continue;
            }
            cout << "������������ڴ��С��";
            cin >> size;
            if (func(processID, size))
            {
                cout << "����ɹ�" << endl;
            }
            else
            {
                cout << "����ʧ��" << endl;
            }
            break;
        case release:
            cout << "�������ID��";
            cin >> processID;
            if (processID < 0 || processID >= processNum)
            {
                cout << "����ID����" << endl;
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
            cout << "��Ч��ѡ��" << endl;
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
    int memory = 0;     // �ڴ��С
    int processNum = 0; // ������
    int method = 0;     // �����㷨
    cout << "ָ���ڴ��С����������" << endl;
    cin >> memory;
    cout << "ָ������������������" << endl;
    cin >> processNum;

    vector<int> numbers(processNum);
    // �������
    for (int i = 0; i < processNum; i++)
    {
        numbers[i] = i;
    }
    cout << "����ID˳����Ϊ��" << endl;
    for (int i = 0; i < processNum; i++)
    {
        cout << numbers[i] << " ";
    }
    memoryPartition temp;
    temp.begin = 0;
    temp.space = memory;
    temp.processID = -1;
    globeMemoryStatus.push_back(temp); // ��ʼ����������ڴ����
    cout << endl;
    cout << "ѡ��˴�ģ��ϵͳ�ķ����㷨" << endl
         << "1.������Ӧ��" << endl
         << "2.�����Ӧ��" << endl
         << "3.���Ӧ��" << endl;
    cin >> method;
    allocateMemorySystem(method, memory, processNum, globeMemoryStatus);
    return 0;
}
