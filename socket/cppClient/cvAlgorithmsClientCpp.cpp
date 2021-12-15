// include the std cpp header files
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// include the opencv header files
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "base64Converter.h"

using std::cout;
using std::cin;
using std::endl;
using std::cerr;

using namespace cv;

#define DEST_IP "10.1.100.66"
#define DEST_PORT 8004
#define RECEIVE_SIZE 20

int main(int argc, char **argv){
    
    // read data from files
    cv::Mat imgColor, imgDepth;
    imgColor = cv::imread("../../frames/colorImage.png", IMREAD_COLOR);
    imgDepth = cv::imread("../../frames/depthImage.png", IMREAD_ANYDEPTH);

    cv::imshow("imgColor", imgColor);
    cv::imshow("imgDepth", imgDepth);
    cv::waitKey(300);

    std::fstream intrinsFile, accelFile, gyroFile;
    intrinsFile.open("../../frames/intrins.txt", std::ios::in);
    accelFile.open("../../frames/vecAccel.txt", std::ios::in);
    gyroFile.open("../../frames/vecGyro.txt", std::ios::in);

    std::string strCurr;
    std::vector<float> vecIntrinsics, vecAccel, vecGyro;
    while(intrinsFile >> strCurr){
        std::istringstream issCurr(strCurr);
        float fCurr;
        issCurr >> fCurr;
        //cout << fCurr << endl;
        vecIntrinsics.push_back(fCurr);
    }
    intrinsFile.close();

    while(accelFile >> strCurr){
        std::istringstream issCurr(strCurr);
        float fCurr;
        issCurr >> fCurr;
        //cout << fCurr << endl;
        vecAccel.push_back(fCurr);
    }
    accelFile.close();

    while(gyroFile >> strCurr){
        std::istringstream issCurr(strCurr);
        float fCurr;
        issCurr >> fCurr;
        //cout << fCurr << endl;
        vecGyro.push_back(fCurr);
    }
    gyroFile.close();

    // send to python socket server
    /****************Convert cv::Mat instances to base64 data string****************/
    // depth image
    std::vector<int> vecParams;
    vecParams.push_back(CV_16U);
    std::vector<uchar> bufDepth;
    cv::imencode(".png", imgDepth, bufDepth, vecParams);
    auto *encMsgDepth = reinterpret_cast<unsigned char*>(bufDepth.data());
    std::string depthEncoded = base64::base64_encode(encMsgDepth, bufDepth.size());

    // color image
    std::vector<uchar> bufColor;
    cv::imencode(".png", imgColor, bufColor);
    auto *encMsgColor = reinterpret_cast<unsigned char*>(bufColor.data());
    std::string colorEncoded = base64::base64_encode(encMsgColor, bufColor.size());

    std::string strDepthEncodedLength = std::to_string(depthEncoded.size());
    std::string strColorEncodedLength = std::to_string(colorEncoded.size());
    int numSpaces = 16 - strDepthEncodedLength.size();
    for (int i = 0; i < numSpaces; ++i){
        strDepthEncodedLength += " ";
    }

    numSpaces = 16 - strColorEncodedLength.size();
    for (int i = 0; i < numSpaces; ++i){
        strColorEncodedLength += " ";
    }

    // construct list Accel
    std::string strListAccel = "";
    strListAccel += std::to_string(vecAccel[0]) + ", " + std::to_string(vecAccel[1]) + ", " + std::to_string(vecAccel[2]);
    strListAccel = "[" + strListAccel + "]";
    strListAccel = "\"listAccel\": " + strListAccel;

    // construct list Gyro
    std::string strListGyro = "";
    strListGyro += std::to_string(vecGyro[0]) + ", " + std::to_string(vecGyro[1]) + ", " + std::to_string(vecGyro[2]);
    strListGyro = "[" + strListGyro + "]";
    strListGyro = "\"listGyro\": " + strListGyro;

    // construct list instrins
    std::string strListIntrins = "";
    strListIntrins += std::to_string(vecIntrinsics[0]) + ", " + std::to_string(vecIntrinsics[1]) + ", " + std::to_string(vecIntrinsics[2]) + ", " + std::to_string(vecIntrinsics[3]);
    strListIntrins = "[" + strListIntrins + "]";
    strListIntrins = "\"listIntrins\": " + strListIntrins;

    std::string strJson = "{" + strListAccel + ", " + strListGyro + ", " + strListIntrins + "}";
    //cout << strJson << endl;
    std::string strJsonLength = std::to_string(strJson.size());
    numSpaces = 16 - strJsonLength.size();
    for (int i = 0; i < numSpaces; ++i){
        strJsonLength += " ";
    }
    /****************Convert cv::Mat instances to base64 data string****************/

    /**********************************send bytes***********************************/
    // init the socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1){
        cout << "Error: socket" << std::endl;
    }

    // connect
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEST_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(DEST_IP);
    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "Error: connect" << endl;
        return 0;
    }
    
    cout << "...connect" << endl;
    send(client, strDepthEncodedLength.c_str(), 16, 0);
    send(client, depthEncoded.c_str(), (int)depthEncoded.size(), 0);
    send(client, strColorEncodedLength.c_str(), 16, 0);
    send(client, colorEncoded.c_str(), (int)colorEncoded.size(), 0);
    send(client, strJsonLength.c_str(), 16, 0);
    send(client, strJson.c_str(), (int)strJson.size(), 0);
    char *cResults = new char[RECEIVE_SIZE], \
         *para1 = new char[RECEIVE_SIZE], \
         *para2 = new char[RECEIVE_SIZE], \
         *para3 = new char[RECEIVE_SIZE];
    recv(client, cResults, RECEIVE_SIZE, 0);
    std::string strNumResults(cResults, RECEIVE_SIZE);
    std::istringstream ssNumResults(strNumResults);
    int nResutls = 0;
    ssNumResults >> nResutls;
    for (int i = 0; i < nResutls; ++i){
        recv(client, para1, RECEIVE_SIZE, 0);
        recv(client, para2, RECEIVE_SIZE, 0);
        recv(client, para3, RECEIVE_SIZE, 0);
        std::string strPara1(para1, RECEIVE_SIZE), strPara2(para2, RECEIVE_SIZE), strPara3(para3, RECEIVE_SIZE);
        std::istringstream ssPara1(strPara1), ssPara2(strPara2), ssPara3(strPara3);
        float fPara1, fPara2, fPara3;
        ssPara1 >> fPara1, ssPara2 >> fPara2, ssPara3 >> fPara3;
        cout << fPara1 << endl;
        cout << fPara2 << endl;
        cout << fPara3 << endl;
    }
    delete[] cResults, para1, para2, para3;
    // send depth image
    close(client);
    
    /**********************************send bytes***********************************/

    return 0;
}
