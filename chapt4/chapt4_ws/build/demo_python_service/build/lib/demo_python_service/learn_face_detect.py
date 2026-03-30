import face_recognition
import cv2
from ament_index_python.packages import get_package_share_directory

def main():
    # 获取资源文件路径
    defaut_image_path=get_package_share_directory('demo_python_service')+"/resource/ceshi.jpg"
    # 加载默认图像并提取人脸特征
    image=cv2.imread(defaut_image_path)
    #查找图像中的人脸位置
    face_locations=face_recognition.face_locations(image,number_of_times_to_upsample=1,model="hog")
    #绘制人脸位置
    for (top,right,bottom,left) in face_locations:
        cv2.rectangle(image,(left,top),(right,bottom),(0,255,0),4)
    #显示图像
    cv2.imshow("Face Detection",image)
    cv2.waitKey(0)