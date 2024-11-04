#! /usr/bin/python3

import requests
from threading import Thread



url = 'http://localhost:3000/api'
request_data = ''

def send_tournament_request():
    global request_data
    s = requests.Session()
    res = s.post(f'{url}/tournament', data=request_data)
    #print(res.status_code)
    #print(res.json()['id'])
    print(res.json()['message'])

def flood_with_tournaments():
    global request_data
    threads: list[Thread] = []
    with open('test_tournament.json', 'r') as infile:
        request_data = infile.read()

    for i in range(100):
        threads.append(Thread(target = send_tournament_request))
        threads[i].start()

    for thread in threads:
        thread.join()

def bad_auth_request():
    res = requests.get(f'{url}/user/admin', headers={"Authorization": "hello plz let me in"})
    print(res.text)

def get_admin_token():
    body = {
        'username': 'admin',
        'password': 'admin'
    }
    res = requests.post(f'{url}/auth', json=body)
    print(res.text)

bad_auth_request()