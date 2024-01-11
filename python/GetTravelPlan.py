import requests
from requests import Response


def get_travel_plan(info: str) -> str:
    # 需要请求的URL
    url: str = 'http://0.0.0.0:5000'
    # 把INFO转换为DICT格式格式
    params: dict = eval(info)
    # 发送GET请求
    response: Response = requests.get(url, params=params)
    return response.text
