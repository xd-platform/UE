#!/usr/bin/python3
import os
# Import WebClient from Python SDK (github.com/slackapi/python-slack-sdk)
from slack_sdk import WebClient
from slack_sdk.errors import SlackApiError
import logging

logging.basicConfig(level=logging.DEBUG)

# WebClient instantiates a client that can call API methods
# When using Bolt, you can use either `app.client` or the `client` passed to listeners.

slack_token = os.environ["SLACK_TOKEN"]
slack_channel_id = os.environ['SLACK_CHANNEL_ID']

print(f"token-- {slack_token.split('-')}")
client = WebClient(token=slack_token, timeout=300)

def sendMessage(content=None, thread=0):
    if content is None:
        return

    try:
        response = client.chat_postMessage(channel=slack_channel_id, text=content, as_user=True,
                                           initial_comment="this is a file", thread_ts=thread)
        # print(response)
        return response["ts"]
    except SlackApiError as e:
        # You will get a SlackApiError if "ok" is False
        assert e.response["ok"] is False
        assert e.response["error"]  # str like 'invalid_auth', 'channel_not_found'
        print(f"Got an error: {e.response['error']}")
        return ""


def uploadFile(file, thread=0):
    try:
        response = client.files_upload_v2(channel=slack_channel_id, file=file, thread_ts=thread)
        # print(response)
        return response["ok"]
    except SlackApiError as e:
        # You will get a SlackApiError if "ok" is False
        assert e.response["ok"] is False
        assert e.response["error"]  # str like 'invalid_auth', 'channel_not_found'
        print(f"Got an error: {e.response['error']}")
        return ""


def deleteMessage(thread):
    try:
        response = client.chat_delete(channel=slack_channel_id, ts=thread)
    except SlackApiError as e:
        # You will get a SlackApiError if "ok" is False
        assert e.response["ok"] is False
        assert e.response["error"]  # str like 'invalid_auth', 'channel_not_found'
        print(f"Got an error: {e.response['error']}")


if __name__ == '__main__':
    pass
    # thread_id = sendMessage("test")
    # print(thread_id)
    # thread_id = uploadFile("/Users/huangyifeng/Work/XDSDK/xdsdk-6.0-ue/Product/Mac/XDSDK_Mac.zip",
    #                        thread_id)
    # print(thread_id)


