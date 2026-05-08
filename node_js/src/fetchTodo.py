#fetch todo or calendar from app
 #can use google calendar api, see link below
#https://developers.google.com/workspace/calendar
#export async function fetchDate() {
import os.path

from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
import json

from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
import warnings
warnings.filterwarnings("ignore")

SCOPES = ['https://www.googleapis.com/auth/tasks.readonly']

def fetch_google_tasks():
    creds = None

    # 如果 token.json 存在，就直接读取
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file(
            'token.json',
            SCOPES
        )

    # 如果没有有效登录状态
    if not creds or not creds.valid:

        # token 过期但可以 refresh
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())

        # 第一次登录
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json',
                SCOPES
            )
            creds = flow.run_local_server(port=0)

        # 保存 token
        with open('token.json', 'w') as token:
            token.write(creds.to_json())

    service = build('tasks', 'v1', credentials=creds)

    result = []

    tasklists = service.tasklists().list().execute()

    for tasklist in tasklists.get('items', []):
        list_name = tasklist['title']
        list_id = tasklist['id']

        tasks = service.tasks().list(tasklist=list_id).execute()

        for task in tasks.get('items', []):
            result.append({
                "list": list_name,
                "title": task.get("title"),
                "status": task.get("status"),
            })

    return result


if __name__ == "__main__":
    tasks = fetch_google_tasks()
    print(json.dumps(tasks))