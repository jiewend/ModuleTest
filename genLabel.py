# -*- coding: UTF-8 -*-  
import os  
  
def genLabel(images_path, txt_save_path):
    fw = open(txt_save_path,"w")  
    images_Name = os.listdir(images_path)  
    #遍历所有文件名  
    i = 0
    for eachname in images_Name:  
        image_list = os.listdir(images_path + eachname)
        i = i + 1
        for eachImage in image_list:
            imageFinalPath = (images_path + eachname + '/' + eachImage)
            #  print(imageFinalPath)
            print(imageFinalPath)
            fw.writelines(str(i) + ' ')
    print "生成txt文件成功"  
    fw.close()  
  
if __name__ == '__main__':  
    images_path = '/home/ucc/Downloads/lfw/'  
    txt_save_path = './onlylabel.txt'  
    genLabel(images_path, txt_save_path)  
