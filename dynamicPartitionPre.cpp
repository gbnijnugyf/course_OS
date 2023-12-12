#include <iostream>
#include <vector>
#include <algorithm>

struct Process {
    int id;
    int size;
    int start;
};

struct MemoryBlock {
    int start;
    int size;
    int processId;
};

std::vector<MemoryBlock> memory;

// ������Ӧ��
int firstFit(int size) {
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].size >= size) {
            int start = memory[i].start;
            memory[i].start += size;
            memory[i].size -= size;
            if (memory[i].size == 0) {
                memory.erase(memory.begin() + i);
            }
            return start;
        }
    }
    return -1;  // �ڴ治��
}

// �����Ӧ��
int bestFit(int size) {
    int bestFitIdx = -1;
    int minFreeSize = INT_MAX;
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].size >= size && memory[i].size < minFreeSize) {
            bestFitIdx = i;
            minFreeSize = memory[i].size;
        }
    }
    if (bestFitIdx != -1) {
        int start = memory[bestFitIdx].start;
        memory[bestFitIdx].start += size;
        memory[bestFitIdx].size -= size;
        if (memory[bestFitIdx].size == 0) {
            memory.erase(memory.begin() + bestFitIdx);
        }
        return start;
    }
    return -1;  // �ڴ治��
}

// ���Ӧ��
int worstFit(int size) {
    int worstFitIdx = -1;
    int maxFreeSize = -1;
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].size >= size && memory[i].size > maxFreeSize) {
            worstFitIdx = i;
            maxFreeSize = memory[i].size;
        }
    }
    if (worstFitIdx != -1) {
        int start = memory[worstFitIdx].start;
        memory[worstFitIdx].start += size;
        memory[worstFitIdx].size -= size;
        if (memory[worstFitIdx].size == 0) {
            memory.erase(memory.begin() + worstFitIdx);
        }
        return start;
    }
    return -1;  // �ڴ治��
}

void allocateMemory(int id, int size, int algorithm) {
    int start;
    if (algorithm == 1) {
        start = firstFit(size);
    } else if (algorithm == 2) {
        start = bestFit(size);
    } else if (algorithm == 3) {
        start = worstFit(size);
    }
    
    if (start == -1) {
        std::cout << "�޷������ڴ������ " << id << std::endl;
    } else {
        MemoryBlock block;
        block.start = start;
        block.size = size;
        block.processId = id;
        memory.push_back(block);
        std::cout << "���� " << id << " �����ڴ�ɹ�����ʼ��ַ��" << start << std::endl;
    }
}

void deallocateMemory(int id) {
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].processId == id) {
            memory.erase(memory.begin() + i);
            std::cout << "���� " << id << " �ͷ��ڴ�ɹ�" << std::endl;
            return;
        }
    }
    std::cout << "���� " << id << " δ�ҵ������ڴ�" << std::endl;
}

void displayMemory() {
    std::cout << "�ڴ�ʹ�������" << std::endl;
    for (const auto& block : memory) {
        std::cout << "��ʼ��ַ��" << block.start << "����С��" << block.size
                  << "������ID��" << block.processId << std::endl;
    }
}
int main() {
    int memorySize;
    int processCount;
    std::cout << "�����ڴ��С��";
    std::cin >> memorySize;
    std::cout << "�����������";
    std::cin >> processCount;
    
    // 初�?�化整个内存为一�?空闲�?
    MemoryBlock block;
    block.start = 0;
    block.size = memorySize;
    block.processId = -1;
    memory.push_back(block);
    
    int choice;
    int id, size, algorithm;
    
    while (true) {
        std::cout << "\n1. �����ڴ�\n2. �ͷ��ڴ�\n3. ��ʾ�ڴ�ʹ�����\n4. �˳�\n";
        std::cout << "ѡ�������";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "�������ID��";
                std::cin >> id;
                std::cout << "������������ڴ��С��";
                std::cin >> size;
                std::cout << "ѡ������㷨��1-������Ӧ����2-�����Ӧ����3-���Ӧ������";
                std::cin >> algorithm;
                allocateMemory(id, size, algorithm);
                break;
            case 2:
                std::cout << "����Ҫ�ͷ��ڴ�Ľ���ID��";
                std::cin >> id;
                deallocateMemory(id);
                break;
            case 3:
                displayMemory();
                break;
            case 4:
                return 0;
            default:
                std::cout << "��Ч��ѡ��" << std::endl;
                break;
        }
    }

    return 0;
}