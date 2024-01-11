from fastapi import FastAPI, Request
import uvicorn
import json

import requests as req

from ExtractInfo import extract_info
from GetTravelPlan import get_travel_plan

app = FastAPI()


# 处理一个GET请求
@app.get("/")
async def run(request: Request):
    params = dict(request.query_params)
    print('question: ' + params['question'])  # 打印请求头中的question字段

    info: str = extract_info(params['question'])  # 调用提取信息函数

    print('extracted travel info:')
    print(info)
    travel_plan: str = get_travel_plan(info)
    print(travel_plan)

    return info


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5001)  # 在这里指定端口号
