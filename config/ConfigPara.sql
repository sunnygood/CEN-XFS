PRAGMA encoding = "UTF-8"; 
PRAGMA auto_vacuum = 1;

/* DROP TABLE and VIEW; */

DROP TABLE IF EXISTS BaseSystem;
DROP TABLE IF EXISTS BasePort;

DROP TABLE IF EXISTS XFSSPManage;

/* CREATE TABLE and VIEW; */

CREATE TABLE IF NOT EXISTS BaseSystem (
name VARCHAR NOT NULL, 
value VARCHAR NOT NULL, 
description VARCHAR, 
PRIMARY KEY (Name));

CREATE TABLE IF NOT EXISTS BasePort (
name VARCHAR NOT NULL, 
value VARCHAR NOT NULL, 
description VARCHAR, 
PRIMARY KEY (Name));

CREATE TABLE IF NOT EXISTS XFSSPManage (
name VARCHAR NOT NULL, 
value VARCHAR NOT NULL, 
description VARCHAR, 
PRIMARY KEY (Name));


begin; 

/* FILL TABLE DATA */

INSERT INTO BaseSystem (name, value, description) 
                       values ('LogLevel',    '0', '日志等级');
INSERT INTO BaseSystem values ('LogSaveMode', '-2', '日志模式');
INSERT INTO BaseSystem values ('Extend',      '0', '日志扩展模式');
INSERT INTO BaseSystem values ('LogDir', 'C:/Log/', '日志路径');


INSERT INTO BasePort   values ('DispatchMsgInWait', '0', '阻塞等待时是否分发消息');
INSERT INTO BasePort   values ('WakeMask', '232', 'QS_POSTMESSAGE|QS_PAINT|QS_HOTKEY|QS_SENDMESSAGE');


INSERT INTO XFSSPManage values ('vendor_name', 'ABC', '厂商名称');
INSERT INTO XFSSPManage values ('version', '3.6', 'SP版本');
INSERT INTO XFSSPManage values ('WorkStationName', 'ABC', '工作站名称');
INSERT INTO XFSSPManage values ('SPIVersion', '3.00|3.30', 'SPI支持XFS版本范围');
INSERT INTO XFSSPManage values ('SrvcVersion', '3.00|3.30', 'SP支持XFS版本范围');
INSERT INTO XFSSPManage values ('LogMaxSaveDay', '180', '日志保存时间');
INSERT INTO XFSSPManage values ('Program', 'XFSSPC.exe', 'SP统一的执行程序');

commit; 
