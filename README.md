### 指纹模块测试
- 平台：stm32f103vct6
- 编辑软件：KEIL for ARM
- 下载器：ST-LINK
- 模块型号：FM-206系列光学指纹模块

#### 1. 接线示意(stm32 --> 指纹模块)
---
- PA2----RXD
- PA3----TXD
---

#### 2. 串口命令
```
finger in       //指纹录入
finger confirm  //指纹识别 
finger num      //已录入个数
finger empty    //清空指纹库
```
#### 3. 说明
模块使用串口命令控制指纹的录入和识别，例如：电脑用串口助手发送指令‘finger in’,将手指放到指纹模块上录入指纹，等待串口手返回‘Store Ok’ 表示录入成功。

