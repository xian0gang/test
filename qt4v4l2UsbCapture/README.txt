本例子在如下条件下测试成功
1、主机环境ubuntu12.04LTS
2、编译之前，先执行如下命令安装所需组件
apt-get update
apt-get install linux-source
apt-get install g++
apt-get install libqt4-dev libqt4-gui libqt4-sql qt4-dev-tools qt4-doc qt4-designer qt4-qtconfig
apt-get install libv4l-dev
apt-get install libjpeg-dev
3、源代码目录下执行make all，如果编译通过，执行目标文件qtv4lCapture即可看到效果
4、enjoy!