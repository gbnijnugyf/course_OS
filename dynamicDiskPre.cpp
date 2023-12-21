#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// ���̵����㷨ö��
enum DiskSchedulingAlgorithm {
    FCFS, // �����ȷ���
    SSTF, // ���Ѱ��ʱ�����ȷ�
    SCAN  // �����㷨
};

// �����ͷ�ƶ����ܴŵ���
int calculateTotalSeekTime(const vector<int>& sequence) {
    int totalSeekTime = 0;
    for (int i = 1; i < sequence.size(); i++) {
        totalSeekTime += abs(sequence[i] - sequence[i - 1]);
    }
    return totalSeekTime;
}

// �����ȷ��񷨣�FCFS��
vector<int> fcfsScheduling(const vector<int>& sequence, int initialPosition) {
    vector<int> scheduledSequence(sequence);
    scheduledSequence.insert(scheduledSequence.begin(), initialPosition);
    return scheduledSequence;
}

// ���Ѱ��ʱ�����ȷ���SSTF��
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

// �����㷨��SCAN��
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

    // ��ͷ���С�ŵ��ƶ�
    for (int i = 0; i < lowerSequence.size(); i++) {
        scheduledSequence.push_back(lowerSequence[i]);
        currentPosition = lowerSequence[i];
    }

    // ��ͷ���ؽϴ�ŵ��ƶ�
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

    // ���뵱ǰ��ͷλ�á���ͷ�ƶ����򡢴ŵ������������е�
    cout << "�����뵱ǰ��ͷλ�ã�";
    cin >> initialPosition;

    cout << "�������ͷ�ƶ�����1��ʾ���ڣ�-1��ʾ���⣩��";
    int direction;
    cin >> direction;

    cout << "������ŵ������������еĴŵ�����";
    cin >> requestCount;

    cout << "������ŵ������������У�" << endl;
    for (int i = 0; i < requestCount; i++) {
        int request;
        cin >> request;
        sequence.push_back(request);
    }

    // ģ�������ȷ��񷨣�FCFS��
    vector<int> fcfsSequence = fcfsScheduling(sequence, initialPosition);
    int fcfsTotalSeekTime = calculateTotalSeekTime(fcfsSequence);

    // ģ�����Ѱ��ʱ�����ȷ���SSTF��
    vector<int> sstfSequence = sstfScheduling(sequence, initialPosition);
    int sstfTotalSeekTime = calculateTotalSeekTime(sstfSequence);

    //�����㷨��SCAN��
    vector<int> scanSequence = scanScheduling(sequence, initialPosition);
    int scanTotalSeekTime = calculateTotalSeekTime(scanSequence);

    // ��ʾ���̵��Ƚ��
    cout << "�����ȷ��񷨣�FCFS���Ĵŵ�����˳��" << endl;
    for (int i = 1; i < fcfsSequence.size(); i++) {
        cout << fcfsSequence[i] << " ";
    }
    cout << endl;
    cout << "�ܴŵ�����" << fcfsTotalSeekTime << endl;

    cout << "���Ѱ��ʱ�����ȷ���SSTF���Ĵŵ�����˳��" << endl;
    for (int i = 1; i < sstfSequence.size(); i++) {
        cout << sstfSequence[i] << " ";
    }
    cout << endl;
    cout << "�ܴŵ�����" << sstfTotalSeekTime << endl;

    cout << "�����㷨��SCAN���Ĵŵ�����˳��" << endl;
    for (int i = 1; i < scanSequence.size(); i++) {
        cout << scanSequence[i] << " ";
    }
    cout << endl;
    cout << "�ܴŵ�����" << scanTotalSeekTime << endl;

    return 0;
}