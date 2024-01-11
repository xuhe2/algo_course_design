from fastapi import FastAPI, Request
import uvicorn
import json

import requests as req

from ExtractInfo import extract_info

app = FastAPI()


# 处理一个GET请求
@app.get("/")
async def run(request: Request):
    headers = dict(request.headers)
    print('question: ' + headers['question'])  # 打印请求头中的question字段

    info = extract_info(headers['question'])  # 调用提取信息函数

    info_json = json.loads(info)  # 将提取的信息转换为JSON格式

    return info


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5001)  # 在这里指定端口号
