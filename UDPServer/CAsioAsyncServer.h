#ifndef CASIOASYNCSERVER_H
#define CASIOASYNCSERVER_H

/* Class Name: CAsioAsyncServer;
 * Description:
 * This class is for receiving Message using ASIO's async UDP protocol;
 * The Message is defined as bellow:
 * struct MData
 * {
 *     u_short imageNumber;
 *     float   dataX;
 *     float   dataY;
 *     float   dataZ;
 * };
 * struct Message {
 *     u_char frameSyn[2];//= 0x5218;
 *     u_short frameLength;
 *     u_char frameType;
 *     u_short messageCount;
 *     u_char checkByte;
 *     u_int frame;
 *     MData mData;
 * };
 * Function:
 * 1. Define the port(e.g. 8001) to receive data;
 * 2. Convert the cirbuffer's pointer to receive data;
 * Tips: No
*/
#include<boost/asio.hpp>
#include<string>
//#include <QDebug>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>
#include<boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <MyOpenglWidget/Clines.h>
#include <MyOpenglWidget/QOpenglMain.h>
//class QOpenglMain;
using boost::asio::ip::udp;
using namespace std;

struct MData
{
    u_short imageNumber;
    u_short lineNumber;
    float   dataX;
    float   dataY;
    float   dataZ;
};
struct Message {
    u_char frameSyn[2];//= 0x5218;
    u_short frameLength;
    u_char frameType;
    u_short messageCount;
    u_char checkByte;
    u_int frame;
    MData mData;
};
typedef boost::circular_buffer<Message> CirBufferLL;

class CAsioAsyncServer : public QObject
{
    Q_OBJECT
public :
    void close();
signals:
    void asioClose();
private slots:
    void onAsioClose();
public:
    CAsioAsyncServer(boost::asio::io_service &io_service, unsigned short port);
    ~CAsioAsyncServer();
    /* 函数: run
     * 函数描述: 开始监听，将数据保存在CirBufferLL中, 调用startRecv;
    */
    void run();
    /* 函数:  virtual Message getMessage(list<QOpenglMain *> thecharts)
     * 函数描述:该函数被定义来对数据进行处理，在读取数据过程中，使用互斥锁来对数据进行锁定，可以被重新写;
    */
//    virtual void convertMessageToQopenglMainWidget(list<QOpenglMain *> thecharts);
    void getCirBuffer(CirBufferLL* cirBuffer);
protected:
    void startRecv();
    virtual void handleRecv(const boost::system::error_code &error, std::size_t recvSize);



private:
    bool closeMark = false;
    udp::socket m_socketRecv;   //socket for receiving
    udp::endpoint m_recvEp;     //
    Message* m_messageRecv;     //storage for the receiving Message;
    int m_lastFrame;            //for couting lost rate;
    int m_coutlost;
    CirBufferLL* m_recvMsgs;    //the cirbuffer's pointer
    boost::mutex m_mutex;       //lock for seperating the reading and writing process;
    CirBufferLL m_cirBuffer;
};

#endif // CASIOASYNCSERVER_H
