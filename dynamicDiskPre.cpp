#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// 磁盘调度算法枚举
enum DiskSchedulingAlgorithm {
    FCFS, // 先来先服务法
    SSTF, // 最短寻道时间优先法
    SCAN  // 电梯算法
};

// 计算磁头移动的总磁道数
int calculateTotalSeekTime(const vector<int>& sequence) {
    int totalSeekTime = 0;
    for (int i = 1; i < sequence.size(); i++) {
        totalSeekTime += abs(sequence[i] - sequence[i - 1]);
    }
    return totalSeekTime;
}

// 先来先服务法（FCFS）
vector<int> fcfsScheduling(const vector<int>& sequence, int initialPosition) {
    vector<int> scheduledSequence(sequence);
    scheduledSequence.insert(scheduledSequence.begin(), initialPosition);
    return scheduledSequence;
}

// 最短寻道时间优先法（SSTF）
vector<int> sstfScheduling(const vector<int>& sequence, int initialPosition) {
    vector<int> scheduledSequence;
    vector<int> remainingRequests(sequence);
    int currentPosition = initialPosition;

    while (!remainingRequests.empty()) {
        
        vector<int>::iterator minDistanceIt = min_element(remainingRequests.begin(), remainingRequests.end(),
            [currentPosition](int a, int b) {
                return abs(a - currentPosition) < abs(b - currentPosition);
            });

        int nextRequest = *minDistanceIt;
        scheduledSequence.push_back(nextRequest);
        currentPosition = nextRequest;

        remainingRequests.erase(minDistanceIt);
    }

    return scheduledSequence;
}

// 电梯算法（SCAN）
vector<int> scanScheduling(const vector<int>& sequence, int initialPosition) {
    vector<int> scheduledSequence;
    vector<int> lowerSequence;
    vector<int> upperSequence;
    int currentPosition = initialPosition;

    for (int request : sequence) {
        if (request < currentPosition) {
            lowerSequence.push_back(request);
        } else {
            upperSequence.push_back(request);
        }
    }

    sort(lowerSequence.begin(), lowerSequence.end(), greater<int>());
    sort(upperSequence.begin(), upperSequence.end());

    scheduledSequence.push_back(currentPosition);

    // 磁头向较小磁道移动
    for (int i = 0; i < lowerSequence.size(); i++) {
        scheduledSequence.push_back(lowerSequence[i]);
        currentPosition = lowerSequence[i];
    }

    // 磁头返回较大磁道移动
    for (int i = 0; i < upperSequence.size(); i++) {
        scheduledSequence.push_back(upperSequence[i]);
        currentPosition = upperSequence[i];
    }

    return scheduledSequence;
}

int main() {
    int initialPosition;
    int requestCount;
    vector<int> sequence;

    // 输入当前磁头位置、磁头移动方向、磁道访问请求序列等
    cout << "请输入当前磁头位置：";
    cin >> initialPosition;

    cout << "请输入磁头移动方向（1表示向内，-1表示向外）：";
    int direction;
    cin >> direction;

    cout << "请输入磁道访问请求序列的磁道数：";
    cin >> requestCount;

    cout << "请输入磁道访问请求序列：" << endl;
    for (int i = 0; i < requestCount; i++) {
        int request;
        cin >> request;
        sequence.push_back(request);
    }

    // 模拟先来先服务法（FCFS）
    vector<int> fcfsSequence = fcfsScheduling(sequence, initialPosition);
    int fcfsTotalSeekTime = calculateTotalSeekTime(fcfsSequence);

    // 模拟最短寻道时间优先法（SSTF）
    vector<int> sstfSequence = sstfScheduling(sequence, initialPosition);
    int sstfTotalSeekTime = calculateTotalSeekTime(sstfSequence);

    //电梯算法（SCAN）
    vector<int> scanSequence = scanScheduling(sequence, initialPosition);
    int scanTotalSeekTime = calculateTotalSeekTime(scanSequence);

    // 显示磁盘调度结果
    cout << "先来先服务法（FCFS）的磁道访问顺序：" << endl;
    for (int i = 1; i < fcfsSequence.size(); i++) {
        cout << fcfsSequence[i] << " ";
    }
    cout << endl;
    cout << "总磁道数：" << fcfsTotalSeekTime << endl;

    cout << "最短寻道时间优先法（SSTF）的磁道访问顺序：" << endl;
    for (int i = 1; i < sstfSequence.size(); i++) {
        cout << sstfSequence[i] << " ";
    }
    cout << endl;
    cout << "总磁道数：" << sstfTotalSeekTime << endl;

    cout << "电梯算法（SCAN）的磁道访问顺序：" << endl;
    for (int i = 1; i < scanSequence.size(); i++) {
        cout << scanSequence[i] << " ";
    }
    cout << endl;
    cout << "总磁道数：" << scanTotalSeekTime << endl;

    return 0;
}