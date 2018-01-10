#include "UDPServer/CAsioAsyncServer.h"
#include <QDebug>
const long int queueSize = 5000;
CAsioAsyncServer::CAsioAsyncServer(boost::asio::io_service &io_service, unsigned short port)
    : m_socketRecv(io_service, udp::endpoint(udp::v4(), port)),
      m_messageRecv(new Message),
      m_lastFrame(0),
      m_coutlost(0),
      m_recvMsgs(new boost::circular_buffer<Message>(queueSize))
{
    if(false == m_socketRecv.is_open())
        m_socketRecv.open(udp::v4());
    bool isconnect =  connect(this, SIGNAL(asioClose()), this, SLOT(onAsioClose()));

}

CAsioAsyncServer::~CAsioAsyncServer()
{
    m_socketRecv.close();
    delete m_messageRecv;
}

void CAsioAsyncServer::run()
{
    startRecv();
}

//void CAsioAsyncServer::convertMessageToQopenglMainWidget(list<QOpenglMain*> thecharts)
//{
//    float x = 0;
//    float y = 0;
//    float z = 0;
//    vector4f _color;
//    boost::mutex::scoped_lock lock(m_mutex);
//    for(boost::circular_buffer<Message>::iterator it = m_recvMsgs->begin(); it != m_recvMsgs->end(); )
//    {
//        x = it->mData.dataX;
//        y = it->mData.dataY;
//        z = it->mData.dataZ;
//        for (auto obj : thecharts)
//        {
//            //QOpenglMain.addPoint();
//            obj->addPoint(0, { x, y, z}); //
//            _color._r = 1; _color._g = 0;
//            _color._b = 1; _color._a = 1;
//            obj->setColor(0, _color);
//            //obj->GetChart()->addLine(4320000);
//            obj->addPoint(1, { x + 1, y + 1, z + 1}); //{ 0,0,1,1 }
////            //obj->GetChart()->addLine(4320000);
//            _color._r = 0; _color._g = 0;
//            _color._b = 1; _color._a = 1;
//            obj->setColor(1, _color);
//            obj->addPoint(2, { x + 2, y + 2, z + 2}); //{ 0,1,0,1 }
//            _color._r = 0; _color._g = 1;
//            _color._b = 0; _color._a = 1;
//            obj->setColor(2, _color);
//            obj->addPoint(3, { x + 3, y + 3, z + 3}); //{ 0,1,1,1 }
//            _color._r = 1; _color._g = 0;
//            _color._b = 0; _color._a = 1;
//            obj->setColor(3, _color);
//        }
//        ++it;
//        if(m_recvMsgs->size() != 0)
//            m_recvMsgs->pop_front();
//    }
//    lock.unlock();
//}

void CAsioAsyncServer::getCirBuffer(CirBufferLL *cirBuffer)
{
    boost::mutex::scoped_lock lock(m_mutex);
    for(boost::circular_buffer<Message>::iterator it = m_recvMsgs->begin(); it != m_recvMsgs->end(); ++it)
    {
        qDebug() << cirBuffer->size();
        cirBuffer->push_back(*it);
        qDebug() << cirBuffer->size();
        qDebug() << it->mData.dataX << " " << it->mData.dataY << " " << it->mData.dataZ;
    }
    m_recvMsgs->clear();
    lock.unlock();
}

void CAsioAsyncServer::startRecv()
{
    m_socketRecv.async_receive_from(
                boost::asio::buffer(m_messageRecv, sizeof(Message)), m_recvEp,
                boost::bind(&CAsioAsyncServer::handleRecv, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void CAsioAsyncServer::handleRecv(const boost::system::error_code &error, std::size_t recvSize)
{
    if(closeMark)
        return;
    m_socketRecv.async_receive_from(
                boost::asio::buffer(m_messageRecv, sizeof(Message)), m_recvEp,
                boost::bind(&CAsioAsyncServer::handleRecv, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    if((0x18 == m_messageRecv->frameSyn[0]) && (0x52 == m_messageRecv->frameSyn[1]))
    {
        if((int)m_messageRecv->frame > (m_lastFrame + 1))
        {
            m_coutlost += ((int)m_messageRecv->frame - m_lastFrame);
            m_lastFrame = (int)m_messageRecv->frame;
            boost::mutex::scoped_lock lock(m_mutex);
            m_recvMsgs->push_back(*m_messageRecv);
            lock.unlock();
        }
        else if((int)m_messageRecv->frame == (m_lastFrame + 1))
        {
            m_lastFrame ++;
            boost::mutex::scoped_lock lock(m_mutex);
            m_recvMsgs->push_back(*m_messageRecv);
            lock.unlock();
        }
        else
        {
            //throw;
        }
    }
}

void CAsioAsyncServer::close()
{
    emit asioClose();
    closeMark = true;
}


void CAsioAsyncServer::onAsioClose()
{
    closeMark = true;
}
