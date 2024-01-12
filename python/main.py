from fastapi import FastAPI, Request
import uvicorn
import json
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from pathlib import Path
from fastapi.middleware.cors import CORSMiddleware

from ExtractInfo import extract_info
from GetTravelPlan import get_travel_plan

app = FastAPI()

# 将'/front-end'路径映射到Vue项目的dist文件夹
app.mount("/front-end", StaticFiles(directory="../front-end"), name="front-end  ")

# 允许所有域的跨域请求
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# 处理一个GET请求
@app.get("/function")
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


@app.get("/")
async def root():
    # 返回main.html
    return FileResponse(Path("../front-end/main.html"))


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5001)  # 在这里指定端口号
