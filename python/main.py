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

    # 如果确实关键信息
    if 'unknown' in info:
        return 'error'

    travel_plan: str = get_travel_plan(info)
    print(travel_plan)

    # 提取travel_plan中的内容变成回答
    travel_plan_dict: dict = json.loads(travel_plan)

    answer: str = ''
    # 添加时间
    answer += 'time: '
    answer += str(travel_plan_dict['distance'])
    answer += '\n'
    answer += 'money: '
    # 添加需要的金钱显示
    answer += str(travel_plan_dict['money'])
    answer += '\n'
    # 添加路径显示
    answer += 'path: '
    answer += travel_plan_dict['path']
    answer += '\n'

    return answer


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5001)  # 在这里指定端口号
