#include<Algorithm.h>

int Which_Step(deque<float>& AngleVelocityQueue){

    int Value=0;
    for(int i=0;i<MAX_DATA_LENGTH;i++){
        if(AngleVelocityQueue[i]<0) Value++;
    }
    if(Value==MAX_DATA_LENGTH){
        return is_d1Step;
    }
    else if (Value==0){
        return is_d1Step;
    }

}

// 双端队列数据更新
void deque_DataUpdate(deque<float>& dataQueue,float data) {
    // 如果队列长度达到限制，则移除最前端的数据
    if (dataQueue.size() == MAX_DATA_LENGTH) {
        dataQueue.pop_front();
    }
    // 在队列后端添加新的数据
    dataQueue.push_back(data);
}

    