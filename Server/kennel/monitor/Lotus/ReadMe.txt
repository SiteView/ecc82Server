为了更好的对Domino设备实施监控请仔细阅读此说明。

    对Domino设备首先要安装Notes Client Domino Designer v6.5的客户端程序，然后更换部分文件，最后安装Domino模块。实现对Domino设备的监控。
    具体安装步骤如下： 

	1、在装有siteview的主机上安装Notes Client Domino Designer v6.5的客户端程序。
	2、将nextpwd.dll替换到装有Domino客户端目录下的notes目录中，将起原文件替换。
	3、在装有Domino客户端目录下的notes\data目录下的notes.ini文件中加入一行“EXTMGR_ADDINS=nextpwd”，可加在“[Notes]”下或文件的最后（位置不限，但要单独一行）。
	4、将Lotus.dll拷贝到\SITEVIEW6\SiteView.DataCollection.WebService\MonitorManager目录下。
        5、执行安装Domino模块程序“\Domino\增加监测器\AddMonitorCS.exe”，安装Domino模块。

    通过以上四步可实现对Domino设备的监测了。
