import openai
import json

openai.api_base = "https://api.chatanywhere.tech/v1"  # 设置API基础URL

with open('api_key.json', 'r') as f:  # 读取API密钥
    api_key = json.load(f)['api_key']

openai.api_key = api_key


def chat(messages: list):
    """为提供的对话消息创建新的回答 (流式传输)

    Args:
        messages (list): 完整的对话消息
        api_key (str): OpenAI API 密钥

    Returns:
        tuple: (results, error_desc)
    """
    try:
        response = openai.ChatCompletion.create(
            model='gpt-3.5-turbo',
            messages=messages,
            stream=True,
        )
        completion = {'role': '', 'content': ''}
        for event in response:
            if event['choices'][0]['finish_reason'] == 'stop':
                # print(f'收到的完成数据: {completion}')
                break
            for delta_k, delta_v in event['choices'][0]['delta'].items():
                # print(f'{delta_v}')  # 流式输出
                completion[delta_k] += delta_v
        messages.append(completion)  # 直接在传入参数 messages 中追加消息
        return completion['content']
    except Exception as err:
        return (False, f'OpenAI API 异常: {err}')


def extract_info(question: str) -> str:
    with open('prompt_template.txt', 'r', encoding='utf-8') as f:
        prompt_template = f.read()

    # 使用我的信息替换提示词中的`<question>`字符串
    prompt = prompt_template.replace('<question>', question)

    messages = [{'role': 'user', 'content': prompt}, ]
    info = chat(messages)

    return info


if __name__ == '__main__':
    question = '我要从天津出发'

    print(extract_info(question))

    # with open('prompt_template.txt', 'r', encoding='utf-8') as f:
    #     prompt_template = f.read()
    #
    # # 使用我的信息替换提示词中的`<question>`字符串
    # prompt = prompt_template.replace('<question>', question)
    #
    # messages = [{'role': 'user', 'content': prompt}, ]
    # info = extract_info(messages)
    # print(info)
