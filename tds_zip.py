import zipfile
import os
import platform


def zipDir(source_dir, output_filename):
    """
    压缩指定文件夹
    :param source_dir: 目标文件夹路径
    :param output_filename: 压缩文件保存路径+xxxx.zip
    :return: 无
    """
    if os.path.exists(source_dir):
        print(f"{source_dir} 压缩中......")
        if platform.system() == "Darwin":
            ret_value = os.system(f"""
                                cd {os.path.dirname(source_dir)}
                                zip -rq {output_filename} {os.path.basename(source_dir)}
                                """)
            if ret_value == 0:
                print(f"{output_filename} 压缩成功")
                return output_filename
            else:
                print(f"{output_filename} 压缩失败")
        else:
            # compression压缩比例，默认是不压缩，ZIP_LZMA=14级别的压缩，影响的是时间，但是包能更小
            # vue使用级别=9的，var archive = archiver('zip', {zlib: {level: 9}});
            # mac install: brew install xz
            # mode 解压是 r , 压缩是 w 追加压缩是 a
            # compression 为  zipfile.ZIP_DEFLATED，zipfile.ZIP_STORED， zipfile.ZIP_LZMA
            zipf = zipfile.ZipFile(file=output_filename, mode='w', compression=zipfile.ZIP_DEFLATED)
            pre_len = len(os.path.dirname(source_dir))
            for parent, dirnames, filenames in os.walk(source_dir):
                for filename in filenames:
                    pathfile = os.path.join(parent, filename)
                    arcname = pathfile[pre_len:].strip(os.path.sep)  # 相对路径
                    zipf.write(pathfile, arcname)
            zipf.close()
            print(f"{output_filename} 压缩成功")
            return output_filename
    else:
        print(f"{source_dir}不存在")

    return


def unzip_file(zip_src, dst_dir):
    # 解压缩
    r = zipfile.is_zipfile(zip_src)
    if r:
        fz = zipfile.ZipFile(zip_src, 'r')
        for file in fz.namelist():
            fz.extract(file, dst_dir)
    else:
        print('This is not zip')
        return False
    return True
