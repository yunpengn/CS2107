#!/usr/bin/python

import sys
from select import select
import json
import random
import os

flag = file("flag").read()
profile = None

def write(data, sep="\n"):
    sys.stdout.write(data + sep)
    sys.stdout.flush()

def readline(prompt):
    timeout = 10
    write(prompt, sep="")
    rlist, _, _ = select([sys.stdin], [], [], timeout)
    if rlist:
        s = sys.stdin.readline()
        return s
    else:
        write("Times up!")
        exit()

def print_menu():
    write("===== Epic Snek Profile Generator =====")
    write("1. Generate your epic profile")
    write("2. Load your epic profile")
    if profile:
        write("3. Print your epic profile")

def generate_profile(name):
    strength = random.randint(1, 100)
    intelligence = random.randint(1, 100)
    agility = random.randint(1, 100)
    speed = random.randint(1, 50)
    level = random.randint(1, 10)
    uid = os.urandom(20).encode("hex")
    profile_object = {'name': name, 'strength': strength,
                      'intelligence': intelligence,
                      'agility': agility,
                      'speed': speed,
                      'level': level,
                      'uid': uid}
    return profile_object

def parse_data(data):
    decoded = data.decode("base64")
    profile_object = json.loads(decoded)
    global profile
    profile_object['flag'] = flag
    profile = profile_object

def print_profile():
    for i in profile.keys():
        if i != 'flag':
            write("%s: %s" % (i.capitalize(), str(profile[i])))
    if 'debug' in profile:
        for i in profile['debug']:
            write(profile.get(i))

def main():
    while True:
        print_menu()
        option = readline("Option: ").strip()
        if option == "1":
            name = readline("Name: ").strip()
            profile_object = generate_profile(name)
            write("Your generated profile data: ")
            data = json.dumps(profile_object).encode('base64').replace("\n", "")
            write(data)
        elif option == "2":
            write("Please provide your profile data: ")
            data = readline("Data: ")
            parse_data(data)
        elif option == "3" and profile is not None:
            print_profile()

if __name__ == "__main__":
    main()
