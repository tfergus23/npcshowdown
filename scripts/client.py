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
    res = requests.get(f'{url}/user/admin/trainers', headers={})
    print(res.text)

def get_admin_token():
    body = {
        'username': 'admin',
        'password': 'admin'
    }
    res = requests.post(f'{url}/auth', json=body)
    print(res.text)

def try_admin_route():

    res = requests.get(f'{url}/user/toad/errors', headers={'Authorization': 'Bearer 8f12233faf3888bee0aa793f64c45fe8'})
    print (res.text)
    res = requests.get(f'{url}/user/admin/errors?page=0&count=10', headers={'Authorization': 'Bearer 59f5b236b5a8b40fdc8ec011f223520a'})
    print(res.headers)
    print (res.text)

flood_with_tournaments()
#try_admin_route()
#bad_auth_request()
#get_admin_token()
