#### 前言

zbar是一个开源的条形码、二维码的检测识别项目。本demo就是基于它来编写的程序，用于识别图片中的条形码（barcode）和二维码（qrcode）。


### 1、下载安装

[https://sourceforge.net/projects/zbar/files/zbar/0.10/](https://sourceforge.net/projects/zbar/files/zbar/0.10/)

```bash
tar xjf zbar-0.10.tar.bz2 
cd zbar-0.10/
./configure --prefix=$PWD/_install --without-gtk --without-qt --without-imagemagick --without-python --disable-video
# ./configure --prefix=$PWD/_install --without-gtk --without-qt --without-imagemagick --without-python --disable-video  --host=arm-linux-gnueabihf CC=arm-linux-gnueabihf-gcc
make CFLAGS=""    # 不指定CFLAGS可能会报错
make install
```

编译完成之后将安装目录`_install`下的`include/zbar.h`和`lib/libzbar.a`拷贝过来就可以使用了。

如果使用的是c++项目，则还可以拷贝`include/zbar/`目录下的其他头文件过来一起使用，这点可以通过`zbar.h`可以知道：

```c
...
#ifdef __cplusplus
    }
}

# include "zbar/Exception.h"
# include "zbar/Decoder.h"
# include "zbar/Scanner.h"
# include "zbar/Symbol.h"
# include "zbar/Image.h"
# include "zbar/ImageScanner.h"
# include "zbar/Video.h"
# include "zbar/Window.h"
# include "zbar/Processor.h"
#endif
```

### 2、demo使用示例

```bash
$ ./zbar_demo 
Usage: 
   ./zbar_demo <pgm_file> <width> <height>
   ./zbar_demo <yuv_file> <width> <height>
   (note: only support ["P5" pgm] and ["Planar/Semi-Planar" YUV] in this demo.)
examples: 
   ./zbar_demo ./pics/barcode.yuv 1120 730
   ./zbar_demo ./pics/qrcode.pgm 250 250
   ./zbar_demo ./pics/multi.pgm 2792 1648
   ./zbar_demo ./pics/nothing.pgm 2792 1648
```

### 3、参考文章

 - [使用zbar识别二维码\_li\_huaqingfeng的博客-CSDN博客\_zbar\ 内存泄漏](https://blog.csdn.net/li_huaqingfeng/article/details/118195587)
 - [https://pan.baidu.com/s/1Xj7yZ2MpP64Ui9DAFUu9gA (提取码：xbtv)](https://pan.baidu.com/s/1Xj7yZ2MpP64Ui9DAFUu9gA )
 - [海思3559平台移植二维码解码库zbar\_重拾程序之路-CSDN博客](https://blog.csdn.net/zhenglie110/article/details/81012351)
 - [基于Zbar条形码图像识别实现.doc-全文可读](https://max.book118.com/html/2018/0828/6131054214001213.shtm)
 - [PGM格式图像详解\_aoanng的博客-CSDN博客\_pgm文件](https://blog.csdn.net/colourful_sky/article/details/70244485)
 - [YUV 格式详解，只看这一篇就够了 - 简书](https://www.jianshu.com/p/538ee63f4c1c)

转换工具：

 - [https://convertio.co/zh/](https://convertio.co/zh/)
 - [https://www.aconvert.com/cn/image/](https://www.aconvert.com/cn/image/)


### 附录（demo架构）

```bash
$ tree
.
├── docs
│   ├── PGM格式图像详解_aoanng的博客-CSDN博客_pgm文件.mhtml
│   ├── YUV 格式详解，只看这一篇就够了 - 简书.mhtml
│   ├── 使用zbar识别二维码_li_huaqingfeng的博客-CSDN博客_zbar 内存泄漏.mhtml
│   ├── 基于Zbar条形码图像识别实现.doc-全文可读.mhtml
│   └── 海思3559平台移植二维码解码库zbar_重拾程序之路-CSDN博客.mhtml
├── include
│   └── zbar.h
├── lib
│   └── libzbar.a
├── main.c
├── Makefile
├── pics
│   ├── barcode.yuv
│   ├── multi.pgm
│   ├── nothing.pgm
│   ├── qrcode.pgm
│   └── Visible
│       ├── barcode.png
│       ├── multi.png
│       ├── nothing.png
│       └── qrcode.png
├── README.md
└── reference_src
    ├── scan_image.c
    ├── test1.c
    └── test2.c
```

