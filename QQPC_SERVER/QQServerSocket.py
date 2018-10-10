#encoding:utf-8
# 导入库
from enum import Enum
from select import select
from socket import *
from QQMysql import *
import struct
import queue
import time


# 枚举类型
class EnumMessageType(Enum):
    VIRUSINFO = 1  #病毒库
    WHITE = 2  # 白名单
    BLACK = 3  # 黑名单
    QUERY=4     #查询数据
    SUBMIT = 5  #提交数据


#socket
class QQServerSocket():
    #构造
    def __init__(self, ip, port):
        ADDR = (ip, port)

        #初始化
        print('正在启动服务器')
        self.socketServer = socket(AF_INET, SOCK_STREAM)
        self.socketServer.bind(ADDR)
        self.socketServer.listen(SOMAXCONN)

        # 设为非阻塞
        self.socketServer.setblocking(False)
        print('服务器启动成功，等待客户端连接...')

        # 监听列表
        self.input_list = [self.socketServer]
        self.output_list = []
        self.client_dict = {}
        self.msg_dict = {}

        #通信列表
        self.recv_list = []
        self.send_list = []
        self.err_list = []

        # 连接数据库
        self.conn = QQMysql()

    # 选择模型
    def ChatSelect(self):
        # 循环接收
        while True:
            # 初始化列表
            self.recv_list, self.send_list, self.err_list = select(self.input_list, self.output_list, self.input_list)
            # 处理客户端
            self.__RecvClient__()
            # 处理消息
            self.__MsgMange__()
            # 处理异常
            self.__Abormal__()

    # 处理接收
    def __RecvClient__(self):

        for sock in self.recv_list:
            # 服务器响应客户端
            if sock is self.socketServer:
                client, addr = sock.accept()
                # 加入客户端列表
                self.input_list.append(client)
                # 设置客户端异步接收信息
                client.setblocking(False)
                print(client.getpeername(), '已上线')
            # 处理客户端消息
            else:
                try:
                    msg = sock.recv(1024)
                    #检测数据是否有效
                    if len(msg)<1024:
                        continue
                    # 判断消息类型
                    type, = struct.unpack("i", msg[:4])

                    # 过滤无效信息
                    if type == 0:
                        pass
                    # 其他消息存入字典待处理
                    else:
                        # 创建消息队列
                        self.msg_dict[sock] = queue.Queue()
                        self.msg_dict[sock].put(msg)
                        # 放入监听发送消息列表
                        if sock not in self.output_list:
                            self.output_list.append(sock)
                # 异常处理
                except ConnectionResetError:
                    # 客户端断开，从监听列表移除
                    self.input_list.remove(sock)
                    print(sock.getpeername(), '已下线')
                    # 将客户端从监听列表移除
                    if sock in self.output_list:
                        self.output_list.remove(sock)
                    # 移除客户端消息队列
                    if sock in self.msg_dict:
                        del self.msg_dict[sock]

                    # 移除客户端在线状态
                    for key, val in self.client_dict.items():
                        if self.client_dict[key] == sock:
                            del self.client_dict[key]
                            break

    # 转发消息处理
    def __MsgMange__(self):

        for sock in self.send_list:
            try:
                # 队列中有消息获取消息
                if not self.msg_dict[sock].empty():
                    # 提取消息
                    msg = self.msg_dict[sock].get()
                    # 判断消息类型
                    type, = struct.unpack("i", msg[:4])
                    # 分配至各个函数
                    self.dictFun[type](self, sock, msg)
                else:
                    # 移除监听等待下一次
                    self.output_list.remove(sock)
            # 处理异常
            except ConnectionResetError:
                # 删除消息队列，设为下线
                del self.msg_dict[sock]
                self.output_list.remove(sock)
                print(sock.getpeername(), '已下线')

    # 异常处理
    def __Abormal__(self):
        for sock in self.err_list:
            print(sock.getpeername(), '出现异常请尽快处理')
            # 队列中删除
            if sock in self.input_list:
                self.input_list.remove(sock)
            if sock in self.output_list:
                self.output_list.remove(sock)
            # 消息队列中删除
            if sock in self.msg_dict:
                del self.msg_dict[sock]


    # 更新病毒数据库到本地
    def __UpdataDatabase__(self, s, msg):
        # 消息处理
        type, = struct.unpack("i", msg[:4])
        if type == EnumMessageType.VIRUSINFO.value:
            result = self.conn.query("SELECT * from `database`",)
            message_type = EnumMessageType.VIRUSINFO
        if type == EnumMessageType.WHITE.value:
            result = self.conn.query("SELECT * from `whitelist`",)
            message_type = EnumMessageType.WHITE
        if type == EnumMessageType.BLACK.value:
            result = self.conn.query("SELECT * from `blacklist`",)
            message_type = EnumMessageType.BLACK

        # 没有就发送结束标志
        if result == None or result[1] == 0:
            database = "~~~end~~~".encode('gbk')
            message_send = struct.pack("l2048s", message_type.value, database)
            s.sendall(message_send)
            return

        # 发送群列表
        for i in range(result[1]):
            database = (result[0][i][1]).encode('utf8').decode('utf8')
            message_send = struct.pack("l2048s", message_type.value, database.encode('gbk'))
            s.sendall(message_send)

        # 发送结束标志
        database = "~~~end~~~".encode('gbk')
        message_send = struct.pack("l2048s", message_type.value, database)
        s.sendall(message_send)

    # 更新白名单
    def __UpdataWhiteList__(self, s, msg):
        pass
    # 更新黑名单
    def __UpdataBlackList__(self, s, msg):
        pass
    #查询MD5
    def __UpdataQuery__(self, s, msg):
        # 消息处理
        strmd5, = struct.unpack("50s", msg[4:54])
        strmd5 = strmd5.decode('gbk').rstrip('\0')
        # 查询
        result = self.conn.query("SELECT * from `database` where `md5` = %s", (strmd5,))

        # 返回结果
        if result == None or result[1] == 0:
            message = "000".encode('gbk')
        else:
            message = "111".encode('gbk')

        message_type = EnumMessageType.QUERY
        message_send = struct.pack("l2048s", message_type.value, message)
        s.sendall(message_send)
    #提交数据
    def __UpdataSUBMIT__(self, s, msg):
          # 消息处理
        name, = struct.unpack("50s", msg[4:54])
        name = name.decode('gbk').rstrip('\0')

        # 查询
        result = self.conn.query("SELECT * from `blacklist` where `md5` = %s", (name,))

        # 返回登录结果
        if result == None or result[1] == 0:
            # 提交数据库
            result = self.conn.DML("INSERT INTO `blacklist` (`md5`) VALUES (%s)", (name,))
            # 返回结果

            if result == None:
                message = "".encode('gbk')
            else:
                message = "插入成功".encode('gbk')

                message_type = EnumMessageType.SUBMIT
                message_send = struct.pack("l2048s", message_type.value, message)

                s.sendall(message_send)
        else:
            print("插入失败")
            return

    # 函数字典
    dictFun = {
        EnumMessageType.VIRUSINFO.value: __UpdataDatabase__,         #病毒库1
        EnumMessageType.WHITE.value: __UpdataDatabase__,            #白名单2
        EnumMessageType.BLACK.value: __UpdataDatabase__,            #黑名单3
        EnumMessageType.QUERY.value: __UpdataQuery__,               # 查询4
        EnumMessageType.SUBMIT.value: __UpdataSUBMIT__,             # 提交5
    }

    BUFSIZE = 2048 + 4
    dictClient = {}

# 主函数
if __name__ == '__main__':
    HOST = '127.0.0.1'  # 服务器ip
    PORT = 1234
    server = QQServerSocket(HOST, PORT)
    # 选择模型
    server.ChatSelect()
