###  RoboMaster开发C板 学习记录

```c
HAL_Init();
```

调用内容为$stm32f4xx\_hal.c$​文件中的函数。​​注释说明如下：

```c
此函数用于初始化HAL库；它必须是主程序中要执行的第一条指令（在调用任何其他HAL函数之前），它执行以下操作：配置闪存预取、指令和数据缓存。将SysTick配置为每1毫秒生成一个中断，该中断由HSI计时（在此阶段，时钟尚未配置，因此系统正在以16 MHz的频率从内部HSI运行）。将NVIC组优先级设置为4。调用用户文件“stm32f4xx_HAL_msp.c”中定义的HAL_MspInit（）回调函数来执行全局低级别硬件初始化。
SysTick用作HAL_Delay（）函数的时基，应用程序需要确保SysTick时基始终设置为1毫秒，以便进行正确的HAL操作。
```

