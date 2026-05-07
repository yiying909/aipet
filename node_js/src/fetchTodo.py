#fetch todo or calendar from app
 #can use google calendar api, see link below
#https://developers.google.com/workspace/calendar
#export async function fetchDate() {
import json

from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build

SCOPES = ['https://www.googleapis.com/auth/tasks.readonly']

def fetch_google_tasks():
    flow = InstalledAppFlow.from_client_secrets_file(
        'credentials.json', SCOPES
    )
    creds = flow.run_local_server(port=0)

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
    for t in tasks:
        # print(t)
        print(json.dumps(tasks))