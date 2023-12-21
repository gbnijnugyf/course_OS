#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// ���̵����㷨ö��
enum METHOD
{
    FCFS, // �����ȷ���
    SSTF, // ���Ѱ��ʱ�����ȷ�
    SCAN  // �����㷨
};
// csdn https://blog.csdn.net/badbad_boy/article/details/4313645 ˵�ŵ�0��������Ȧ
enum DIRECT
{
    OUT = -1, // ����,�ȷ��ʴŵ��Ÿ�С��
    IN = 1    // ����,�ȷ��ʴŵ��Ÿ����
};
// �����ͷ�ƶ����ܴŵ���
int calculateTotalSeekTime(const vector<int> &sequence)
{
    int totalSeekTime = 0;
    for (int i = 1; i < sequence.size(); i++)
    {
        totalSeekTime += abs(sequence[i] - sequence[i - 1]);
    }
    return totalSeekTime;
}
// Ѱ����pos����ĵ�����
vector<int>::iterator findMinDistance(vector<int> &seq, int pos)
{
    return min_element(seq.begin(), seq.end(),
                       [pos](int a, int b)
                       {
                           return abs(a - pos) < abs(b - pos);
                       });
}
// �����ȷ��񷨣�FCFS��
vector<int> fcfsMethod(const vector<int> &sequence, int initialPosition, int d)
{
    vector<int> result(sequence);
    // �����ȷ���˳����ȣ�����ǰλ�ò���ͷ�����ؼ���
    result.insert(result.begin(), initialPosition);
    return result;
}
// ���Ѱ��ʱ�����ȷ���SSTF��
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
// �����㷨
vector<int> scanMethod(const vector<int> &sequence, int initialPosition, int d)
{
    vector<int> result;
    result.push_back(initialPosition);
    vector<int> inToCurrent;  // ����ڵ�ǰλ��������ģ��ŵ��Ÿ����
    vector<int> outToCurrent; // ����ڵ�ǰλ��������ģ��ŵ��Ÿ�С��
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
    case IN: // �����ȷ��ʸ���Ĳ���
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
    case OUT: // �����ȷ��ʸ�С�Ĳ���
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
        cout << "���������" << endl;
        return result;
    }
    return result;
}

typedef vector<int> (*FunctionPtr)(const vector<int> &, int, int);
// ͬһ�����Ӻ���
void handle(int m, FunctionPtr func, const vector<int> &sequence, int initialPosition, int direct)
{
    vector<int> resultSequence = func(sequence, initialPosition, direct);
    int totalSeekTime = calculateTotalSeekTime(resultSequence);
    // FCFS, // �����ȷ���
    // SSTF, // ���Ѱ��ʱ�����ȷ�
    // SCAN  // �����㷨
    string mstr = "";
    switch (m)
    {
    case FCFS: // �����ȷ���
        mstr = "�����ȷ��񷨣�FCFS��";
        break;
    case SSTF: // ���Ѱ��ʱ�����ȷ�
        mstr = "���Ѱ��ʱ�����ȷ���SSTF��";
        break;
    case SCAN: // �����㷨
        mstr = "�����㷨��SCAN��";
        break;
    default:
        cout << "�����������" << endl;
        return;
    }
    // ��ʾ���̵��Ƚ��
    cout << mstr << "�Ĵŵ�����˳��" << endl;
    for (int i = 1; i < resultSequence.size(); i++)
    {
        cout << resultSequence[i] << " ";
    }
    cout << endl;
    cout << "�ܴŵ�����" << totalSeekTime << endl;
}

int main()
{
    int currentPos = -1;    // ��ǰλ��
    int currentDir = 0;     // �ƶ�����
    vector<int> responeSeq; // ��������
                            // ���뵱ǰ��ͷλ�á���ͷ�ƶ����򡢴ŵ������������е�
    cout << "�����뵱ǰ��ͷλ�ã�";
    cin >> currentPos;

    do
    {
        cout << "�������ͷ�ƶ�����1��ʾ���ڣ�-1��ʾ���⣬�����ظ���һ���裩��";
        cin >> currentDir;
    } while (!(currentDir == -1 || currentDir == 1));

    int resNum = 0;
    cout << "������ŵ������������еĴŵ�����";
    cin >> resNum;

    cout << "������ŵ������������У�" << endl;
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