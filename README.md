ubuntu16.04 & ros kinetic

usage

1. clone tesseract  
git clone https://github.com/tesseract-ocr/tesseract

2. build   
./autogen.sh  
./configure  
make  
sudo make install  
sudo ldconfig  

3. clone tessdata  
git clone https://github.com/tesseract-ocr/tessdata

4. set environment variable  
export TESSDATA_PREFIX=/some/path/to/tessdata  
(ex. tessdata location : /home/username/tesseract/tessdata  
TESSDATA_PREFIX=/home/username/tesseract)

5. launch
roslaunch text_detection text_detection.launch
(need usb_cam)

