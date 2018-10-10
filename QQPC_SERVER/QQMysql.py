#encoding:utf-8
import mysql.connector

class QQMysql():
    # 初始化
    def __init__(self):
        # 配置
        config = {'host': '127.0.0.1',
                  'user': 'root',
                  'password': '',
                  'port': '3306',
                  'database': 'qqmgr',
                  'charset': 'utf8'}
        print('正在连接数据库...')
        # 连接
        self.conn = mysql.connector.connect(**config)
        print('连接数据库成功！')

    # 析构
    def __del__(self):
        self.conn.close()

    # 查询
    def query(self, szSql, param=None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = (cursor.fetchall(), cursor.rowcount)
            cursor.close()
            return result
        #异常处理
        except:
            cursor.close()
            # 错误回滚
            self.conn.rollback()
            return None

    # 插入/删除
    def DML(self, szSql, param=None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        #异常处理
        except:
            # 错误回滚
            cursor.close()
            self.conn.rollback()
            return None
