@if NOT %1=="" set strUsername=%1
@if     %1=="" set strUsername=weblogic
@if NOT %2=="" set strPwd=%2
@if     %2=="" set strPwd=weblogic
@if NOT %3=="" set strServerIp=%3
@if     %3=="" set strServerIp=192.168.0.161
@if NOT %4=="" set strServerPort=%4
@if     %4=="" set strServerPort=7001
@if NOT %5=="" set strTaskType=%5
@if     %5=="" set strTaskType=WlsStatus
@if NOT %6=="" set strTaskParam=%6
@if     %6=="" set strTaskParam=

cd C:\SiteView\SiteView ECC\fcgi-bin

set path=c:\program files\java\jdk1.5.0\bin

set classpath=C:\SiteView\SiteView ECC\fcgi-bin\weblogic_9.jar;C:\Program Files\Java\jdk1.5.0\lib\dt.jar;C:\Program Files\Java\jdk1.5.0\lib\htmlconverter.jar;C:\Program Files\Java\jdk1.5.0\lib\tools.jar;C:\Program Files\Java\jdk1.5.0\lib;C:\Program Files\Java\jdk1.5.0\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\jce.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\rt.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\sunrsasign.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\ext\ldapsec.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.5.0\jre\lib\ext\sunjce_provider.jar;
java GetWeblogicPerf %strUsername% %strPwd% %strServerIp% %strServerPort% %strTaskType% %strTaskParam%