#! /usr/bin/python3

import requests
from threading import Thread



url = 'http://localhost:3000/api'
request_data = ''
with open('test_tournament.json', 'r') as infile:
    request_data = infile.read()

def send_tournament_request():
    s = requests.Session()
    res = s.post(f'{url}/tournament', data=request_data)
    #print(res.status_code)
    #print(res.json()['id'])
    print(res.json()['message'])

threads: list[Thread] = []

for i in range(100):
    threads.append(Thread(target = send_tournament_request))
    threads[i].start()

for thread in threads:
    thread.join()