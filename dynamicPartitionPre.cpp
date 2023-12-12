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

// 最先适应法
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
    return -1;  // 内存不足
}

// 最佳适应法
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
    return -1;  // 内存不足
}

// 最坏适应法
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
    return -1;  // 内存不足
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
        std::cout << "无法分配内存给进程 " << id << std::endl;
    } else {
        MemoryBlock block;
        block.start = start;
        block.size = size;
        block.processId = id;
        memory.push_back(block);
        std::cout << "进程 " << id << " 分配内存成功，起始地址：" << start << std::endl;
    }
}

void deallocateMemory(int id) {
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].processId == id) {
            memory.erase(memory.begin() + i);
            std::cout << "进程 " << id << " 释放内存成功" << std::endl;
            return;
        }
    }
    std::cout << "进程 " << id << " 未找到分配内存" << std::endl;
}

void displayMemory() {
    std::cout << "内存使用情况：" << std::endl;
    for (const auto& block : memory) {
        std::cout << "起始地址：" << block.start << "，大小：" << block.size
                  << "，进程ID：" << block.processId << std::endl;
    }
}
int main() {
    int memorySize;
    int processCount;
    std::cout << "输入内存大小：";
    std::cin >> memorySize;
    std::cout << "输入进程数：";
    std::cin >> processCount;
    
    // 鍒濆?嬪寲鏁翠釜鍐呭瓨涓轰竴涓?绌洪棽鍧?
    MemoryBlock block;
    block.start = 0;
    block.size = memorySize;
    block.processId = -1;
    memory.push_back(block);
    
    int choice;
    int id, size, algorithm;
    
    while (true) {
        std::cout << "\n1. 分配内存\n2. 释放内存\n3. 显示内存使用情况\n4. 退出\n";
        std::cout << "选择操作：";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "输入进程ID：";
                std::cin >> id;
                std::cout << "输入进程所需内存大小：";
                std::cin >> size;
                std::cout << "选择分配算法（1-最先适应法，2-最佳适应法，3-最坏适应法）：";
                std::cin >> algorithm;
                allocateMemory(id, size, algorithm);
                break;
            case 2:
                std::cout << "输入要释放内存的进程ID：";
                std::cin >> id;
                deallocateMemory(id);
                break;
            case 3:
                displayMemory();
                break;
            case 4:
                return 0;
            default:
                std::cout << "无效的选择" << std::endl;
                break;
        }
    }

    return 0;
}