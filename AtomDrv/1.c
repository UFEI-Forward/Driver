/*
ref1: https://blog.csdn.net/qq_42604176/article/details/112535213
ref2: https://blog.csdn.net/qq_42604176/article/details/112535213
ref3: https://zhuanlan.zhihu.com/p/130755604
read the articles above and make notes






Basic Understanding
1.Driver is a file in /dev/xxx with operations, including open, release, read and write
2.Driver should be develped under various driver frameworks in Linux [What is frameworks?]
3.The new kernel supports device tree, which is a .dts file describing the basic info of device hardware
4.Driver cannot directly communicate with registers

Three kinds of driver:
1.Char device driver[Most]
2.Block device driver
3.Network device driver
*/