#include<cmath>
#include <deque>
#include<vector>

using namespace std;

const int is_d1Step=1;
const int is_d2Step=2;
const int MAX_DATA_LENGTH=9;

void deque_DataUpdate(deque<float>& dataQueue,float data);
int Which_Step(deque<float>& AngleVelocityQueue);

/**
 * @brief Class for detecting peaks and troughs in a stream of data.
 *
 * This class implements a peak and trough detection algorithm using a sliding window approach.
 * It keeps track of a set of recent data points and determines if the latest point is a peak or a trough
 * compared to the preceding points within a specified window size.
 */
class PeakTroughDetector {
public:
/**
     * @brief Constructor for PeakTroughDetector.
     * 
     * Initializes a new instance of the PeakTroughDetector class with a specified window size.
     * The window size determines how many of the most recent data points will be considered
     * when checking for peaks and troughs.
     *
     * @param windowSize The number of data points to consider for detecting peaks and troughs.
     */
    PeakTroughDetector(int windowSize) : windowSize(windowSize) {}
    std::deque<float> dataQueue; 
    size_t windowSize; 


    /**
     * @brief Checks for peaks or troughs in the current data window.
     * 
     * This method analyzes the current set of data in the queue to determine if the latest data point
     * is a peak or a trough. A peak is detected if the latest data point is greater than all previous points
     * in the window. Similarly, a trough is detected if it is smaller than all previous points.
     *
     * @return An integer indicating the status: 0 for no extremum, 1 for peak, and 2 for trough.
     */
    int checkForExtrema(int windowSize)// 构造函数
    {
        if (dataQueue.size() < windowSize) {
            return 0; // 数据不足以进行判断
        }
        float lastData = dataQueue.back();
        bool isPeak = true, isTrough = true;
        // 检查是否是高峰或低峰
        for (size_t i = 0; i < dataQueue.size() - 1; ++i) {
            if (lastData <= dataQueue[i]) {
                isPeak = false;
            }
            if (lastData >= dataQueue[i]) {
                isTrough = false;
            }
        }
        if (isPeak) {
            return 1;
        }
        if (isTrough) {
            return 2;
        }
        return 0;
    }

    /**
     * @brief Updates the detector with a new data point and checks for extrema.
     * 
     * This method adds a new data point to the data queue and checks if this new point
     * forms a peak or trough with respect to the existing points in the window.
     *
     * @param newData The new data point to be added.
     * @return An integer indicating the status: 0 for no extremum, 1 for peak, and 2 for trough.
     */
    int updateData(float newData) // 更新数据的方法
    {
        int DataStatus;
        dataQueue.push_back(newData);
        if (dataQueue.size() > windowSize) {
            dataQueue.pop_front();
        }
        DataStatus=checkForExtrema(windowSize);
        return DataStatus; 
    }

private:
};