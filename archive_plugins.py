import os
import shutil

import tds_zip

project_dir = os.path.dirname(__file__)
archive_dir = os.path.join(project_dir, "Product")


# 获取Plugins路径
plugins_dir = ""
for root, dirs, files in os.walk(project_dir, topdown=False):
    for dir_name in dirs:
        if dir_name == "Plugins":
            temp_dir = os.path.join(root, dir_name)
            # 最短的路径，就是想要的Plugins路径
            if len(plugins_dir) == 0 or len(temp_dir) < len(plugins_dir):
                plugins_dir = temp_dir

print(f"Plugins路径: {plugins_dir}")
if len(plugins_dir) == 0:
    raise Exception("Plugins路径没有找到")

# 去掉Plugins中的缓存
for root, dirs, files in os.walk(plugins_dir, topdown=False):
    for dir_name in dirs:
        if dir_name == "Binaries" or dir_name == "Intermediate":
            cache_dir = os.path.join(root, dir_name)
            print(f"删除缓存：{cache_dir}")
            shutil.rmtree(cache_dir)


# 压缩Plugins目录到Product中
if not os.path.exists(archive_dir):
    os.makedirs(archive_dir)
target_zip = os.path.join(archive_dir, "Plugins.zip")
print(f"{target_zip}压缩中....")
tds_zip.zipDir(plugins_dir, target_zip)
print(f"{target_zip}压缩成功")