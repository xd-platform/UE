#! /usr/bin/env python3

import requests
import os
import json


class Uploader(object):
    token = "f3245375393807b2500e76baaae7d435aa9f0cab"
    url = "https://npkg.xindong.com/api/v1/packages/"
    file_path = ""
    memo = ""

    def __init__(self, file_path: str, memo: str = ""):
        self.file_path = file_path
        self.memo = memo

    def upload(self):
        if not os.path.exists(self.file_path):
            print("上传的文件不存在")
            return
        print(f"{self.file_path}上传中.....")
        payload = {
            'memo': self.memo
        }
        headers = {
            'Authorization': f'Token {self.token}'
        }
        files = [
            ('file', (os.path.basename(self.file_path), open(f'{self.file_path}', 'rb'), 'application/octet-stream'))
        ]
        response = requests.post(self.url, headers=headers, data=payload, files=files)
        print(f"上报结果：{response.text}")
        try:
            json_object = json.loads(response.text)
            if 'id' in json_object and json_object['id'] > 0:
                print(f"{self.file_path} 上传成功")
                return f"https://npkg.xindong.com/install/{json_object['id']}"
            else:
                print(f"{self.file_path} 上传失败：{json_object['message']}")
        except Exception as e:
            print(f"请求结果格式化错误:{e}")
# print(os.path.basename("/Users/huangyifeng/Work/XDSDK/xdsdk-6.0-ue/Product/IOS/Distro_XDSDK.ipa"))
# print(Uploader("/Users/huangyifeng/Work/XDSDK/xdsdk-6.0-ue/Product/IOS/Distro_XDSDK.ipa").upload())
# print(Uploader("/Users/huangyifeng/Work/XDSDK/xdsdk-6.0-ue/Product/Mac/XDSDK_Mac.zip").upload())


