ubuntu16.04 & ros kinetic

usage

1. clone tesseract  
$ git clone https://github.com/tesseract-ocr/tesseract

2. build   
$ ./autogen.sh  
$ ./configure  
$ make  
$ sudo make install  
$ sudo ldconfig  

3. clone tessdata  
$ git clone https://github.com/tesseract-ocr/tessdata

4. set environment variable ( <- not need ???)  
$ export TESSDATA_PREFIX=/home/username/tesseract   
 ex) tessdata location : /home/username/tesseract/tessdata  


5. launch
$ roslaunch text_detection text_detection.launch
please change paramater yourself

$ roslaunch text_detection text_detection_usbcam.launch
(required usb_cam)

