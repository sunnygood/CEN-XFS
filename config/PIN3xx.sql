PRAGMA encoding = "UTF-8"; 
PRAGMA auto_vacuum = 1;

/* DROP TABLE and VIEW; */
DROP TABLE IF EXISTS PIN3xx;

/* CREATE TABLE and VIEW; */
CREATE TABLE IF NOT EXISTS PIN3xx (
name VARCHAR NOT NULL, 
value VARCHAR NOT NULL, 
description VARCHAR, 
PRIMARY KEY (Name));


begin; 

/* FILL TABLE DATA */

INSERT INTO PIN3xx values ('Class',  'PIN', '类名称');
INSERT INTO PIN3xx values ('Type',   'EPP|EDM|HSM',   'PIN的类型');
INSERT INTO PIN3xx values ('XFSModule', 'XFSPINBase.dll', 'PIN的标准模块名称');
/*INSERT INTO PIN3xx values ('CombineSrvcName', 'VDM3xx', '组合模块的服务名称'); */

commit; 
