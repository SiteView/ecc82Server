@if NOT %1=="" set strUsername=%1
@if     %1=="" set strUsername=administrator
@if NOT %2=="" set strPwd=%2
@if     %2=="" set strPwd=12345678
@if NOT %3=="" set strServerIp=%3
@if     %3=="" set strServerIp=t3://192.168.0.161:8001
@if NOT %4=="" set strTaskType=%4
@if     %4=="" set strTaskType=WlsStatus
@if NOT %5=="" set strTaskParam=%5
@if     %5=="" set strTaskParam=null

cd C:\SiteView\SiteView ECC\fcgi-bin\

set path=D:\oracle\product\10.1.0\Db_1\jdk\bin;c:\windows\system32;c:\windows;c:\windows\system32\wbem

set classpath=C:\SiteView\SiteView ECC\fcgi-bin\weblogic.jar;C:\Program Files\Java\jdk1.5.0_05\lib\dt.jar;C:\Program Files\Java\jdk1.5.0_05\lib\htmlconverter.jar;C:\Program Files\Java\jdk1.5.0_05\lib\tools.jar;C:\Program Files\Java\jdk1.5.0_05\lib;C:\Program Files\Java\jdk1.5.0_05\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\jce.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\rt.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\sunrsasign.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\ext\ldapsec.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.5.0_05\jre\lib\ext\sunjce_provider.jar;
java getActiveDomainAndServers -U %strUsername% -P %strPwd% -I %strServerIp% -T %strTaskType%  -C %strTaskParam%