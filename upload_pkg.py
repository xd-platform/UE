#! /usr/bin/env python3

from typing import Any
import requests
import sys
import json

token = "f3245375393807b2500e76baaae7d435aa9f0cab"

file_path = str(sys.argv[1])

upload_file = str(sys.argv[2])

# # 上传文件
upload_url = "https://npkg.xindong.com/api/v1/packages/"

payload={'memo': 'TapSDK2-UE'}

files=[
  ('file',(upload_file,open(f'{file_path}','rb'),'application/octet-stream'))
]
headers = {
  'Authorization': f'Token {token}'
}

response = requests.post(upload_url, headers=headers, data=payload, files=files)

jsonObject = json.loads(response.text)

if 'id' in jsonObject:
    print(jsonObject['id'])
else:
    print(jsonObject['message'][-5:])
    
