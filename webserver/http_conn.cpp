#include "http_conn.h"
//对静态变量初始化
int http_conn::m_epollfd = -1;  
int http_conn::m_users_count = 0;

//全局函数
int setnonblocking( int fd ) {
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

// 向epoll中添加需要监听的文件描述符
void addfd( int epollfd, int fd, bool one_shot ) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET |EPOLLRDHUP;
    if(one_shot) 
    {
        // 防止同一个通信被不同的线程处理
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    // 设置文件描述符非阻塞
    setnonblocking(fd);  
}

// 从epoll中移除监听的文件描述符
void removefd( int epollfd, int fd ) {
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close(fd);
}

// 修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev |EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

// 成员函数
// 关闭连接
void http_conn::close_conn() {
    if(m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_users_count--; // 关闭一个连接，将客户总数量-1
    }
}

// 初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in& addr){
    m_sockfd = sockfd;
    m_address = addr;
    
    // 端口复用
    int reuse = 1;
    setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    addfd( m_epollfd, sockfd, true );
    m_users_count++;
}


// 循环读取客户数据，直到无数据可读或者对方关闭连接
bool http_conn::read() {
    //特殊情况之->缓冲区大小不够接受客户端数据
    if(m_read_idx >= READ_BUFFER_SIZE){
        return false;
    }
    int bytes_read = 0;
    while(1){
        bytes_read = recv(m_sockfd,m_read_buf+m_read_idx,READ_BUFFER_SIZE-m_read_idx,0);
        if(bytes_read == -1) {
            if(errno == EAGAIN || errno == EWOULDBLOCK ){
                //没有数据
                break;
            }
            return false;
        } else if(bytes_read == 0) {    //对方关闭连接或者读到末尾
            // removefd(m_epollfd,m_sockfd);
            // close_conn();  
            return false;
        } else if(bytes_read > 0) {
            m_read_idx += bytes_read;
        }
    }
    printf("读取到了数据:\n%s\n",m_read_buf);
    return true;
}

// 写HTTP响应
bool http_conn::write()
{
    printf("write ing\n");
    return true;
}

// 由线程池中的工作线程调用，这是解析HTTP请求的入口函数
void http_conn::process() {
    //解析HTTP请求——使用有限状态机模型
    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST){
        //请求不完整，需要继续读取客户数据
        modfd(m_epollfd,m_sockfd,EPOLLIN);  //修改监听事件，并重新注册oneshot
        return;
    }else if(read_ret == GET_REQUEST){
        //表示获得了一个完整的客户请求
    }else if(read_ret == BAD_REQUEST){
        //客户端请求语法错误
    }
    printf("parse request，create response\n");
}
//主状态机
http_conn::HTTP_CODE http_conn::process_read(){
    LINE_STATUS line_status = LINE_OK;
    while((line_status = parse_line()) == LINE_OK){
        switch (expression)
        {
        case /* constant-expression */:
            /* code */
            break;
        
        default:
            break;
        }
    }
    return GET_REQUEST;
}
//解析http请求中的“行”
http_conn::LINE_STATUS http_conn::parse_line(){
    return LINE_OK;
}
//解析http请求首行
http_conn::HTTP_CODE http_conn::parse_request_line(char* text){
    return GET_REQUEST;
}
//解析http请求头
http_conn::HTTP_CODE http_conn::parse_headers(char* text){
    return GET_REQUEST;
}
//解析http请求体
http_conn::HTTP_CODE http_conn::parse_body(char* text){
    return GET_REQUEST;
}
