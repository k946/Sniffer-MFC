# Sniffer-MFC 1.0
基于winpcap的MFC窗口应用嗅探软件
## 功能：
1.0：   
  嗅探捕获数据包   
  解析数据包的头部（可解析的协议：ETHERNET,802.3,IP,IPv6,ARP,ICMP,TCP,UDP,IGMP,OSPF）  
  以十六进制和ASCII形式显示数据包  
  编辑数据包  
  发送数据包（还没有重新计算验证和的功能）  
  过滤器（可过滤IP地址和协议）  
## 缺点：  
  存在内存泄漏  
   
   
## 运行截图：  
<img src="https://github.com/k946/Sniffer-MFC/blob/master/Sniffer-WindowsMFC/runtimeText/主体.PNG" width="350" height="350" alt="图片加载失败时，显示这段字"/>  
<img src="https://github.com/k946/Sniffer-MFC/blob/master/Sniffer-WindowsMFC/runtimeText/重新发送ping包.PNG" width="350" height="150" alt="图片加载失败时，显示这段字"/>  
