import shutil
import os
import re
import sys


class AndroidAAr(object):
    def __init__(self, name, target_plugin):
        self.name = name
        self.target_plugin = target_plugin
        # self.source_path = os.path.join(root, file_name)
        # self.source_name = file_name
        # self.source_version = os.path.splitext(file_name)[0].split("_")[1]
        # return

    def update(self):
        source = self.search_aar(android_aar_path)
        dest_path = os.path.join(project_path,
                                 "Plugins",
                                 self.target_plugin,
                                 "Source",
                                 "ThirdParty",
                                 "Android",
                                 "libs")
        origin = self.search_aar(dest_path)
        shutil.copy(os.path.join(source[0], source[1]), dest_path)
        if origin is not None and origin[1] != source[1]:
            self.replace_version(os.path.join(project_path, "Plugins", self.target_plugin, "Source"),
                                 os.path.splitext(origin[1])[0], os.path.splitext(source[1])[0])
            os.remove(os.path.join(origin[0], origin[1]))
            # self.replace_version(os.path.join(project_path, "Plugins", self.target_plugin, "Source"),
            #                      "XDGAccount_6.6.1", "XDGAccount_6.7.2")
        print(f"{self.name} 更新成功")

    def search_aar(self, path):
        for root, dirs, files in os.walk(path, topdown=False):
            for file_name in files:
                result = re.match(self.name, file_name)
                if result is not None:
                    return root, file_name

    def replace_version(self, path, originVer, destVer):
        for root, dirs, files in os.walk(path, topdown=False):
            for file_name in files:
                if file_name.endswith(".xml"):
                    file_path = os.path.join(root, file_name)
                    # 以读写模式打开文件
                    with open(file_path, 'r+') as f:
                        # 读取文件数据并将其存储在文件变量中
                        file = f.read()

                        # 用文件数据中的字符串替换模式
                        result = re.subn(originVer, destVer, file)
                        # print(result)
                        if result[1] > 0:
                            # 设置位置到页面顶部插入数据
                            f.seek(0)

                            # 在文件中写入替换数据
                            f.write(result[0])

                            # 截断文件大小
                            f.truncate()

                            return


android_aar_path = sys.argv[1]
project_path = os.path.dirname(__file__)
project_path = os.path.join(project_path, "XDSDK")
# android_aar_path = "/Users/sisiliu/Downloads/Products"
# project_path = "/Users/sisiliu/Documents/hyfwork/xdsdk-6.0-ue/XDSDK"

AndroidAAr("XDGAccount", "XDGAccount").update()
AndroidAAr("XDGPayment", "XDGPayment").update()
AndroidAAr("XDGCommon", "XDGCommon").update()
AndroidAAr("XDGThirdLogin", "XDGCommon").update()
AndroidAAr("XDGThirdTrack", "XDGCommon").update()
